/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"
#include "trcTrace.h"

#include "risThreadsPriorities.h"
#include "risNetUdpMsgThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

UdpMsgThread::UdpMsgThread(Settings& aSettings)
{
   // Base class variables.
   BaseClass::setThreadName("UdpMsg");
   BaseClass::setThreadPriority(aSettings.mThreadPriority);

   // Store settings.
   mSettings = aSettings;
   mRxMsgQCall = aSettings.mRxMsgQCall;

   // Initialize variables.
   mTI = mSettings.mTraceIndex;
   mErrorCount = 0;
   mRestartCount = 0;
   mRxCount = 0;
   mTxCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void UdpMsgThread::threadInitFunction()
{
   Trc::write(mTI, 1, "UdpMsgThread::threadInitFunction");

   // Initialize the socket according to the settings.
   mMsgSocket.initialize(&mSettings);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// udp port receives and then processes them. The loop terminates
// when the socket is closed by the shutdown call.

void UdpMsgThread::threadRunFunction()
{
   Trc::write(mTI, 0, "UdpMsgThread::threadRunFunction");

   // Top of the loop.
   mRestartCount = 0;

Restart:

   // Guard.
   if (mTerminateFlag) return;
   int tRet = 0;

   // If not first time then sleep.
   if (mRestartCount > 0)
   {
      BaseClass::threadSleep(1000);
   }
   Trc::write(mTI, 0, "UdpMsgThread restart %d", mRestartCount);
   Prn::print(Prn::Show1, "UdpMsgThread restart %d", mRestartCount);
   mRestartCount++;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open device.

   // Close the socket.
   mMsgSocket.doClose();

   // Configure the socket.
   mMsgSocket.configure();
   if (!mMsgSocket.mValidFlag)
   {
      // If error then restart.
      goto Restart;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop to receive messages.

   while (!BaseClass::mTerminateFlag)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Receive.

      // Try to receive a message with a blocking receive call. If a message
      // was received then process it.
      ByteContent* tMsg = 0;
      if (mMsgSocket.doReceiveMsg(tMsg))
      {
         Trc::write(mTI, 1, "UdpMsgThread::threadRunFunction recv msg");
         // Message was correctly received.
         processRxMsg(tMsg);
      }
      else
      {
         // Message was not correctly received or the socket was
         // closed by the shutdown procedure.
         Trc::write(mTI, 1, "UdpMsgThread::threadRunFunction recv msg ERROR");

         // Check for terminate.
         if (BaseClass::mTerminateFlag)
         {
            // Terminate.
            Trc::write(mTI, 0, "UdpMsgThread read TERMINATE");
            break;
         }
         else
         {
            // Restart.
            goto Restart;
         }
      }
   }

   // Done.
   Trc::write(mTI, 0, "UdpMsgThread::threadRunFunction done");
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void UdpMsgThread::threadExitFunction()
{
   Trc::write(mTI, 1, "UdpMsgThread::threadExitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shutdown, base class overload. This sets the terminate request flag
// and closes the socket.
//
// If the while loop in the thread run function is blocked on receiving a 
// message then closing the socket will cause the receive call to return with
// false and then the terminate request flag will be polled and the
// thread run function will exit.

void UdpMsgThread::shutdownThread()
{
   Trc::write(mTI, 1, "UdpMsgThread::shutdownThread");

   // Set the terminate flag, close the socket, and wait for the thread
   // to terminate.
   BaseThread::mTerminateFlag = true;
   mMsgSocket.doClose();
   BaseThread::waitForThreadTerminate();

   Trc::write(mTI, 1, "UdpMsgThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received message to the parent thread. This is called by the
// thread run function when a message is received. It invokes the
// receive message qcall that is registered at initialization.

void UdpMsgThread::processRxMsg(Ris::ByteContent* aMsg)
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
// send call in the context of the calling thread.

void UdpMsgThread::sendMsg (Ris::ByteContent* aMsg)
{
   // Send the message.
   mMsgSocket.doSendMsg(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

