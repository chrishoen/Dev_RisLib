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
   mSuspendReq = false;
   mSuspendFlag = false;
   mSessionQCall = aSettings.mSessionQCall;
   mRxMsgQCall = aSettings.mRxMsgQCall;

   mTI = aSettings.mTraceIndex;
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
   Trc::write(mTI, 0, "SerialMsgThread::threadInitFunction");

   // Initialize the serial port.
   mSerialMsgPort.initialize(mSettings);

   Trc::write(mTI, 0, "SerialMsgThread::threadInitFunction done");
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
   Trc::write(mTI, 0, "SerialMsgThread::threadRunFunction");

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
   Trc::write(mTI, 0, "SerialMsgThread restart %d", mRestartCount);
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

   // If the serial port is open then close it.
   if (mSerialMsgPort.mOpenFlag)
   {
      mSerialMsgPort.doClose();
   }

   // Open the serial port.
   if (!mSerialMsgPort.doOpen())
   {
      // If error then restart.
      goto restart;
   }

   // Wait for modem valid.
   if (!mSerialMsgPort.doWaitForModemValid())
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
      ByteContent* tMsg = 0;
      tRet = mSerialMsgPort.doReceiveMsg(tMsg);

      // Check for terminate.
      if (BaseClass::mTerminateFlag)
      {
         Trc::write(mTI, 0, "SerialMsgThread read TERMINATE");
         goto end;
      }

      // Check the return code.
      if (tRet == 0)
      {
         Trc::write(mTI, 0, "SerialMsgThread read EMPTY");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetError)
      {
         Trc::write(mTI, 0, "SerialMsgThread read ERROR");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetDataError)
      {
         Trc::write(mTI, 0, "SerialMsgThread read DATA ERROR");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetTimeout)
      {
         Trc::write(mTI, 0, "SerialMsgThread read TIMEOUT");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetAbort)
      {
         Trc::write(mTI, 0, "SerialMsgThread read ABORT");
         if (mSuspendReq)
         {
            // If the abort was caused by a suspension request then
            // close the serial port and wait on the resume semaphore.
            Trc::write(mTI, 0, "SerialMsgThread SUSPENDED");
            Prn::print(0, 0, "SerialMsgThread SUSPENDED");
            mSuspendReq = false;
            mSerialMsgPort.doClose();
            mSuspendFlag = true;
            mResumeSem.get();
            mSuspendFlag = false;
         }
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
   Trc::write(mTI, 0, "SerialMsgThread::threadRunFunction done");
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It is close the serial port.

void SerialMsgThread::threadExitFunction()
{
   Trc::write(mTI, 0, "SerialMsgThread::threadExitFunction");

   // Close the serial port.
   mSerialMsgPort.doClose();

   Trc::write(mTI, 0, "SerialMsgThread::threadExitFunction done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This is called out of the context of
// this thread. It aborts the serial port receive and waits for the
// thread to terminate after execution of the thread exit function.

void SerialMsgThread::shutdownThread()
{
   Trc::write(mTI, 0, "SerialMsgThread::shutdownThread");

   // Set for thread termination.
   BaseClass::mTerminateFlag = true;

   // Abort pending serial port receives
   mSerialMsgPort.doAbort();

   // Wait for thread to terminate.
   BaseClass::shutdownThread();

   Trc::write(mTI, 0, "SerialMsgThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort the serial message port blocked receive call.

void SerialMsgThread::doAbort()
{
   mSerialMsgPort.doAbort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the suspension request flag and abort the pending receive. This
// will cause the thread to close the serial port and then block on the
// resume semaphore.

void SerialMsgThread::doSuspend()
{
   mSuspendReq = true;
   mSerialMsgPort.doAbort();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Post to the resume semaphore to wake up the thread and enter the 
// restart loop.

void SerialMsgThread::doResume()
{
   mResumeSem.put();
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
// Send a message via the serial port. Return the number of bytes
// transferred or a negative error code.

int SerialMsgThread::doSendMsg (Ris::ByteContent* aMsg)
{
   return mSerialMsgPort.doSendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

