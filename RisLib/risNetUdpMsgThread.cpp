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
   mConnectionFlag = false;
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
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// the udp port receives and then processes them. The loop terminates
// when the serial port receive is aborted.

void UdpMsgThread::threadRunFunction()
{
   Trc::write(mTI, 0, "UdpMsgThread::threadRunFunction");

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
   Trc::write(mTI, 0, "UdpMsgThread restart %d", mRestartCount);
   Prn::print(Prn::Show1, "UdpMsgThread restart %d", mRestartCount);
   mRestartCount++;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open device.

   // If the socket is open then close it.
   if (mMsgSocket.mValidFlag)
   {
      mMsgSocket.doClose();
   }

   // Initialize and configure the message socket.
   mMsgSocket.initialize(mSettings);
   mMsgSocket.configure();
   if (!mMsgSocket.mValidFlag)
   {
      // If error then restart.
      goto restart;
   }

   // Connection was established.
   mConnectionFlag = true;

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

      // Try to receive a message with a blocking receive call. If a message
      // was received then process it.
      ByteContent* tMsg = 0;
      if (mMsgSocket.doReceiveMsg(tMsg))
      {
         Trc::write(mTI, 1, "UdpMsgThread::threadRunFunction recv msg");
         // Message was correctly received.
         // Call the receive callback qcall.
         processRxMsg(tMsg);
      }
      else
      {
         Trc::write(mTI, 1, "UdpMsgThread::threadRunFunction recv msg ERROR");

         // Check for terminate.
         if (BaseClass::mTerminateFlag)
         {
            Trc::write(mTI, 0, "UdpMsgThread read TERMINATE");
            goto end;
         }
         else
         {
            mMsgSocket.mValidFlag = false;
            goto restart;
         }
      }
   }

   // Done.
end:
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
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doReceiveMsg then
// closing the socket will cause doReceiveMsg to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void UdpMsgThread::shutdownThread()
{
   Trc::write(mTI, 1, "UdpMsgThread::shutdownThread");
   BaseThread::mTerminateFlag = true;

   mMsgSocket.doClose();

   BaseThread::waitForThreadTerminate();
   Trc::write(mTI, 1, "UdpMsgThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received message to the parent thread. This is called by the
// threadRunFunction when a message is received. It invokes the
// mRxMsgQCall that is registered at initialization.

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
// blocking send call in the context of the calling thread.

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

