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
#include "risSerialMsgThread.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialMsgThread::SerialMsgThread(SerialSettings& aSettings)
{
   // Set base class thread services.
   BaseClass::setThreadName("SerialMsg");
   BaseClass::setThreadPriority(aSettings.mThreadPriority);

   mSettings = aSettings;
   mSessionQCall = aSettings.mSessionQCall;
   mRxMsgQCall = aSettings.mRxMsgQCall;

   mErrorCount = 0;
   mRestartCount = 0;
   mRxCount = 0;
   mTxCount = 0;
}

SerialMsgThread::~SerialMsgThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// Initialize and open the serial port.

void SerialMsgThread::threadInitFunction()
{
   // Initialize the serial port.
   mSerialMsgPort.initialize(mSettings);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// serial port receives and then processes them. The loop terminates
// when the serial port receive is aborted.

void SerialMsgThread::threadRunFunction()
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
   Prn::print(Prn::Show1, "SerialMsgThread restart %d", mRestartCount);
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

   // Open the serial port.
   if (!mSerialMsgPort.doOpen())
   {
      // If error then restart.
      goto restart;
   }

   // Connection was established.
   mConnectionFlag = true;

   // Process a session change because a
   // new session has been established.
   printf("LINE101\n");
   processSessionChange(true);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to receive strings.

   while (!BaseClass::mTerminateFlag)
   {
      Prn::print(Prn::Show4, "SerialMsgThread read start********************************************** %d", mRxCount++);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Receive.

      // Receive. 
      ByteContent* tMsg = 0;
      tRet = mSerialMsgPort.doReceiveMsg(tMsg);

      // Check for terminate.
      if (BaseClass::mTerminateFlag)
      {
         Prn::print(Prn::Show1, "SerialMsgThread read TERMINATE");
         goto end;
      }

      // Check the return code.
      if (tRet == 0)
      {
         Prn::print(Prn::Show1, "SerialMsgThread read EMPTY");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetError)
      {
         Prn::print(Prn::Show1, "SerialMsgThread read ERROR");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetTimeout)
      {
         Prn::print(Prn::Show1, "SerialMsgThread read TIMEOUT");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetAbort)
      {
         Prn::print(Prn::Show1, "SerialMsgThread read ABORT");
         goto end;
      }
      else if (tRet == Ris::cSerialRetDataError)
      {
         Prn::print(Prn::Show1, "SerialMsgThread read DATA ERROR");
         goto restart;
      }
      // Process the read.
      else
      {
         // Message was correctly received.
         // Invoke the receive qcall callback, passing the received message
         // to the parent thread.
         processRxMsg(tMsg);
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

void SerialMsgThread::threadExitFunction()
{
   printf("SerialMsgThread::threadExitFunction BEGIN\n");

   // Close the serial port.
   mSerialMsgPort.doClose();

   printf("SerialMsgThread::threadExitFunction END\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This is called out of the context of
// this thread. It aborts the serial port receive and waits for the
// thread to terminate after execution of the thread exit function.

void SerialMsgThread::shutdownThread()
{
   printf("SerialMsgThread::shutdownThread BEGIN\n");

   // Set for thread termination.
   BaseClass::mTerminateFlag = true;

   // Abort pending serial port receives
   mSerialMsgPort.doAbort();

   // Wait for thread to terminate.
   BaseClass::shutdownThread();

   printf("SerialMsgThread::shutdownThread END\n");
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

void SerialMsgThread::processSessionChange(bool aEstablished)
{
   // If the callback qcall is valid then invoke it.
   if (mSessionQCall.isValid())
   {
      mSessionQCall(aEstablished);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received message to the parent thread. This is called by the
// threadRunFunction when a message is received. It invokes the
// mRxMsgQCall that is registered at initialization.

void SerialMsgThread::processRxMsg(Ris::ByteContent* aMsg)
{
   // Guard.
   if (!mRxMsgQCall.isValid()) return;

   // Invoke the receive callback qcall.
   mRxMsgQCall(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a transmit message through the socket to the peer. It executes a
// blocking send call in the context of the calling thread. It is protected
// by a mutex semaphore.

void SerialMsgThread::sendMsg (Ris::ByteContent* aMsg)
{
   mSerialMsgPort.doSendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

