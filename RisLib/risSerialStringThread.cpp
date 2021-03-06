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

   // Set base class thread services.
   BaseClass::setThreadName(tThreadName);
   BaseClass::setThreadPriority(aSettings.mThreadPriority);

   mSettings = aSettings;
   mSessionQCall = aSettings.mSessionQCall;
   mSessionCallback = aSettings.mSessionCallback;
   mRxStringQCall = aSettings.mRxStringQCall;
   mRxStringCallback = aSettings.mRxStringCallback;

   mConnectionFlag = false;
   mTxCount = 0;
   mTxLength = 0;
   mRxCount = 0;
   mRxError = 0;
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
   // Initialize and open the serial port.
   mSerialPort.initialize(mSettings);
//   mConnectionFlag = mSerialPort.doOpen();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that receives strings and calls the handler.

void  SerialStringThread::threadRunFunction()
{
   bool tGoing = true;

   while (tGoing)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // If not connected.

      if (!mConnectionFlag)
      {
         // Try to connect.
         if (mSerialPort.doOpen())
         {
            // Connection was established.
            mConnectionFlag = true;

            // Process a session change because a
            // new session has been established.
            processSessionChange(true);
         }
         else
         {
            // Connection was not established.
            // If termination request, exit the loop
            // This is set by shutdown, see below.
            if (mTerminateFlag) break;

            // Sleep.
            threadSleep(2000);
         }
      }

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // If connected.

      else
      {
         // Try to receive a string with a blocking receive call.
         // If a string was received then process it.
         // If a string was not received then the serial port was closed or 
         // an error occurred.  
         if (mSerialPort.doReceiveLine(mRxString, cMaxStringSize) > 0)
         {
            // Metrics.
            mRxCount++;
            // Message was correctly received.
            // call the receive callback, passing the received string.
            // to the thread inheritor.
            processRxString(mRxString);
         }
         else
         {
            // If termination request, exit the loop
            // This is set by shutdown, see below.
            if (mTerminateFlag) break;

            // Message was not correctly received, so
            // Connection was lost.
            //printf("serial port Recv failed, Connection lost\n");
            mConnectionFlag = false;

            // Process a session change because a
            // session has been disestablished.
            processSessionChange(false);
         }
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void SerialStringThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the serial port.
//
// If the while loop in the threadRunFunction is blocked on doReceiveString then
// closing the serial port will cause doReceiveString to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void SerialStringThread::shutdownThread()
{
   shutdownThreadPrologue();
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mSerialPort.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
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
// mRxStringQCall that is registered at initialization.

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
// Send a null terminated string via the serial port. A newline terminator
// is appended to the string before transmission. This executes in the
// context of the The calling thread.

void SerialStringThread::sendString(const char* aString, bool aTerminator)
{
   if (aTerminator)
   {
      mSerialPort.doSendLine(aString);
   }
   else
   {
      mSerialPort.doSendBytes(aString, (int)strlen(aString));
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a null terminated string via the serial port. A newline terminator
// is appended to the string before transmission. This executes in the
// context of the The calling thread. The string is deleted after
// transmission.

void SerialStringThread::sendString(std::string* aString, bool aTerminator)
{
   if (aTerminator)
   {
      mSerialPort.doSendLine(aString->c_str());
   }
   else
   {
      mSerialPort.doSendBytes(aString->c_str(), (int)strlen(aString->c_str()));
   }
   delete aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

