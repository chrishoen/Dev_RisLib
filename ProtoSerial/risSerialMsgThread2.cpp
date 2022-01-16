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
#include "risSerialMsgThread2.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialMsgThread2::SerialMsgThread2(SerialSettings& aSettings)
{
   // Set base class thread services.
   BaseClass::setThreadName("SerialMsg");
   BaseClass::setThreadPriority(aSettings.mThreadPriority);

   mSettings = aSettings;
   mRxMsgQCall = aSettings.mRxMsgQCall;

   mErrorCount = 0;
   mRestartCount = 0;
   mRxCount = 0;
   mTxCount = 0;
}

SerialMsgThread2::~SerialMsgThread2()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// Initialize and open the serial port.

void SerialMsgThread2::threadInitFunction()
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

void SerialMsgThread2::threadRunFunction()
{
   // Top of the loop.
   mRestartCount = 0;
restart:
   // Guard.
   if (mTerminateFlag) return;
   int tRet = 0;

   // Sleep.
   if (mRestartCount > 0)
   {
      BaseClass::threadSleep(1000);
   }
   Prn::print(Prn::Show1, "Serial restart %d", mRestartCount);
   mRestartCount++;

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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to receive strings.

   while (!BaseClass::mTerminateFlag)
   {
      Prn::print(Prn::Show4, "Serial read start********************************************** %d", mRxCount++);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Receive.

      // Receive. 
      ByteContent* tMsg = 0;
      tRet = mSerialMsgPort.doReceiveMsg(tMsg);

      // Check the return code.
      if (tRet == 0)
      {
         Prn::print(Prn::Show1, "Serial read EMPTY");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetError)
      {
         Prn::print(Prn::Show1, "Serial read ERROR");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetTimeout)
      {
         Prn::print(Prn::Show1, "Serial read TIMEOUT");
         goto restart;
      }
      else if (tRet == Ris::cSerialRetAbort)
      {
         Prn::print(Prn::Show1, "Serial read ABORT");
         goto end;
      }
      // Process the read.
      else
      {
         // Message was correctly received.
         // Invoke the receive qcall callback, passing the received message
         // to the thread owner.
         processRxMsg(tMsg);
//       Prn::print(Prn::Show1, "Serial read  $$$    %d %s", mRxCount, mRxBuffer);
      }
   }

   // Done.
end:
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that receives messages and passes them to the
// attached message handler.

void  SerialMsgThread2::threadRunFunction22()
{
   bool tGoing = mSerialMsgPort.mValidFlag;

   while (tGoing)
   {
      // Try to receive a message with a blocking receive call.
      // If a message was received then process it.
      // If a message was not received then the serial port was closed or 
      // an error occurred.  
      ByteContent* tMsg = 0;
      if (mSerialMsgPort.doReceiveMsg(tMsg))
      {
         // Metrics.
         mRxCount++;
         // Message was correctly received.
         // Invoke the receive qcall callback, passing the received message
         // to the thread owner.
         processRxMsg(tMsg);
      }
      else
      {
         // Message was not correctly received.
         //mRxError++;
      }

      // If termination request, exit the loop.
      // This is set by shutdown, see below.
      if (mTerminateFlag)
      {
         tGoing = false;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It is close the serial port.

void SerialMsgThread2::threadExitFunction()
{
   printf("someSerialThread::threadExitFunction\n");

   // Close the serial port.
   mSerialMsgPort.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. This is called out of the context of
// this thread. It aborts the serial port receive and waits for the
// thread to terminate after execution of the thread exit function.

void SerialMsgThread2::shutdownThread()
{
   printf("someSerialThread::shutdownThread\n");

   // Abort pending serial port receives
   mSerialMsgPort.doAbort();

   // Wait for thread to terminate.
   BaseClass::shutdownThread();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received message to the parent thread. This is called by the
// threadRunFunction when a message is received. It invokes the
// mRxMsgQCall that is registered at initialization.

void SerialMsgThread2::processRxMsg(Ris::ByteContent* aMsg)
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

void SerialMsgThread2::sendMsg (Ris::ByteContent* aMsg)
{
   mSerialMsgPort.doSendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

