/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
#include "risPortableCalls.h"
#include "prnPrint.h"
#include "trcTrace.h"

#include "risThreadsPriorities.h"
#include "risSerialStringThread.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialStringThread::SerialStringThread(SerialSettings& aSettings)
{
   // Names.
   char tThreadName[40];
   if (aSettings.mIdent < 0)
   {
      sprintf(tThreadName, "SerialString");
   }
   else
   {
      sprintf(tThreadName, "SerialString%d", aSettings.mIdent);
   }

   // Set base class variables.
   BaseClass::setThreadName(tThreadName);
   BaseClass::setThreadPriority(aSettings.mThreadPriority);

   // Set member variables.
   mSettings = aSettings;
   mSessionQCall = aSettings.mSessionQCall;
   mSessionCallback = aSettings.mSessionCallback;
   mRxStringQCall = aSettings.mRxStringQCall;
   mRxStringCallback = aSettings.mRxStringCallback;

   mConnectionFlag = false;
   mErrorCount = 0;
   mRestartCount = 0;
   mRxCount = 0;
   mTxCount = 0;
}

SerialStringThread::~SerialStringThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// Initialize and open the serial port.

void SerialStringThread::threadInitFunction()
{
   Trc::write(mTI, 0, "SerialStringThread::threadInitFunction");

   // Initialize the serial port.
   mSerialStringPort.initialize(mSettings);

   Trc::write(mTI, 0, "SerialStringThread::threadInitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// serial port receives and then processes them. The loop terminates
// when the serial port receive is aborted.

void SerialStringThread::threadRunFunction()
{
   Trc::write(mTI, 0, "SerialStringThread::threadRunFunction");

   // Top of the loop.
   mRestartCount = 0;
   mConnectionFlag = false;

restart:
   // Guard.
   if (mTerminateFlag) return;
   int tRet = 0;

   // Sleep.
   if (mRestartCount > 0)
   {
      BaseClass::threadSleep(1000);
   }
   Trc::write(mTI, 0, "SerialStringThread restart %d", mRestartCount);
   Prn::print(Prn::Show1, "SerialStringThread restart %d", mRestartCount);
   mRestartCount++;

   // Test if a session is established.
   if (mConnectionFlag)
   {
      // Connection was disestablished.
      mConnectionFlag = false;

      // Process a session change because a
      // session has been disestablished.
      processSessionChange(false);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open device.

   // If the serial port is open then close it.
   if (mSerialStringPort.mOpenFlag)
   {
      mSerialStringPort.doClose();
   }

   // Open the serial port.
   if (!mSerialStringPort.doOpen())
   {
      // If error then restart.
      goto restart;
   }

   // Wait for modem valid.
   if (!mSerialStringPort.doWaitForModemValid())
   {
      // If error then restart.
      goto restart;
   }

   // Connection was established.
   mConnectionFlag = true;

   // Process a session change because a
   // new session has been established.
   processSessionChange(true);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to receive strings.

   while (!BaseClass::mTerminateFlag)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Receive.

      // Receive. 
      tRet = mSerialStringPort.doReceiveString(mRxString, cMaxStringSize);

      // Check for terminate.
      if (BaseClass::mTerminateFlag)
      {
         Trc::write(mTI, 0, "SerialStringThread read TERMINATE");
         goto end;
      }

      // Check the return code.
      if (tRet == 0)
      {
         Trc::write(mTI, 0, "SerialStringThread read EMPTY");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetError)
      {
         Trc::write(mTI, 0, "SerialStringThread read ERROR");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetTimeout)
      {
         Trc::write(mTI, 0, "SerialStringThread read TIMEOUT");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetAbort)
      {
         Trc::write(mTI, 0, "SerialStringThread read ABORT");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetDataError)
      {
         Trc::write(mTI, 0, "SerialStringThread read DATA ERROR");
         goto restart;
      }
      // Process the read.
      else
      {
         // String was correctly received.
         // Invoke the receive qcall callback, passing the received string
         // to the parent thread.
         processRxString(mRxString);
      }
   }

   // Done.
end:
   Trc::write(mTI, 0, "SerialStringThread::threadRunFunction done");
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It is close the serial port.

void SerialStringThread::threadExitFunction()
{
   Trc::write(mTI, 0, "SerialStringThread::threadExitFunction");

   // Close the serial port.
   mSerialStringPort.doClose();

   Trc::write(mTI, 0, "SerialStringThread::threadExitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This is called out of the context of
// this thread. It aborts the serial port receive and waits for the
// thread to terminate after execution of the thread exit function.

void SerialStringThread::shutdownThread()
{
   Trc::write(mTI, 0, "SerialStringThread::shutdownThread");

   // Set for thread termination.
   BaseClass::mTerminateFlag = true;

   // Abort pending serial port receives
   mSerialStringPort.doAbort();

   // Wait for thread to terminate.
   BaseClass::shutdownThread();

   Trc::write(mTI, 0, "SerialStringThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Notify the parent thread that a session has changed. This is called by
// the threadRunFunction when a new session is established or an existing
// session is disestablished. It invokes the mSessionQCall that is
// registered at initialization. The session is disestablished if the 
// serial port is closed because of an error and it is established if
// it is successfully reopened.

void SerialStringThread::processSessionChange(bool aEstablished)
{
   // If the callback function pointer is valid then call it.
   if (mSessionCallback)
   {
      mSessionCallback(aEstablished);
   }

   // If the callback qcall is valid then invoke it.
   if (mSessionQCall.isValid())
   {
      mSessionQCall(aEstablished);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received string to the parent thread. This is called by the
// threadRunFunction when a string is received. It invokes the
// mRxMsgQCall that is registered at initialization.

void SerialStringThread::processRxString(char* aString)
{
   // If the receive callback function pointer is valid then call it.
   if (mRxStringCallback)
   {
      mRxStringCallback(new std::string(aString));
   }

   // If the receive callback qcall is valid then invoke it.
   if (mRxStringQCall.isValid())
   {
      mRxStringQCall(new std::string(aString));
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. Append terminator
// bytes as specified in the settings. Return the number of bytes 
// transferred or a negative error code.

int SerialStringThread::doSendString(const char* aString)
{
   return mSerialStringPort.doSendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. Append terminator
// bytes as specified in the settings. The string is deleted after
// transmission. Return the number of bytes transferred or a negative
// error code.

int SerialStringThread::doSendString(std::string* aString)
{
   int tRet = mSerialStringPort.doSendString(aString->c_str());
   delete aString;
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

