/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
#include "risPortableCalls.h"
#include "prnPrint.h"

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
   // Initialize the serial port.
   mSerialStringPort.initialize(mSettings);
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
      Prn::print(Prn::Show4, "SerialStringThread read start********************************************** %d", mRxCount++);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Receive.

      // Receive. 
      tRet = mSerialStringPort.doReceiveString(mRxString, cMaxStringSize);

      // Check for terminate.
      if (BaseClass::mTerminateFlag)
      {
         Prn::print(Prn::Show1, "SerialStringThread read TERMINATE");
         goto end;
      }

      // Check the return code.
      if (tRet == 0)
      {
         Prn::print(Prn::Show1, "SerialStringThread read EMPTY");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetError)
      {
         Prn::print(Prn::Show1, "SerialStringThread read ERROR");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetTimeout)
      {
         Prn::print(Prn::Show1, "SerialStringThread read TIMEOUT");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetAbort)
      {
         Prn::print(Prn::Show1, "SerialStringThread read ABORT");
         goto end;
      }
      else if (tRet == Ris::cSerialRetDataError)
      {
         Prn::print(Prn::Show1, "SerialStringThread read DATA ERROR");
         goto restart;
      }
      // Process the read.
      else
      {
         // Message was correctly received.
         // Invoke the receive qcall callback, passing the received string
         // to the parent thread.
         processRxString(mRxString);
      }
   }

   // Done.
end:
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It is close the serial port.

void SerialStringThread::threadExitFunction()
{
   printf("SerialStringThread::threadExitFunction BEGIN\n");

   // Close the serial port.
   mSerialStringPort.doClose();

   printf("SerialStringThread::threadExitFunction END\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This is called out of the context of
// this thread. It aborts the serial port receive and waits for the
// thread to terminate after execution of the thread exit function.

void SerialStringThread::shutdownThread()
{
   printf("SerialStringThread::shutdownThread BEGIN\n");

   // Set for thread termination.
   BaseClass::mTerminateFlag = true;

   // Abort pending serial port receives
   mSerialStringPort.doAbort();

   // Wait for thread to terminate.
   BaseClass::shutdownThread();

   printf("SerialStringThread::shutdownThread END\n");
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
// bytes as specified in the settings.

void SerialStringThread::sendString(const char* aString)
{
   mSerialStringPort.doSendString(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. Append terminator
// bytes as specified in the settings. The string is deleted after
// transmission.

void SerialStringThread::sendString(std::string* aString)
{
   mSerialStringPort.doSendString(aString->c_str());
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

