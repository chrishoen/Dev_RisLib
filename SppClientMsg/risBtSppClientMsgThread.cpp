/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
#include "prnPrint.h"
#include "trcTrace.h"
#include "risThreadsPriorities.h"

#include "risBtFindAddress.h"
#include "risBtSppClientMsgThread.h"

namespace Ris
{
namespace Bt
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SppClientMsgThread::SppClientMsgThread(Settings& aSettings)
{
   // Base class variables.
   BaseClass::setThreadName("SppMsgClient");
   BaseClass::setThreadPriority(aSettings.mThreadPriority);

   // Store settings.
   mSettings = aSettings;
   mSessionQCall = aSettings.mClientSessionQCall;
   mRxMsgQCall = aSettings.mRxMsgQCall;
   mTI = aSettings.mTraceIndex;

   // Initialize variables.
   mTI = mSettings.mTraceIndex;
   mConnectionFlag = false;
   mErrorCount = 0;
   mFindCount = 0;
   mRestartCount = 0;
   mRxCount = 0;
   mTxCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It initializes the socket.

void SppClientMsgThread::threadInitFunction()
{
   Trc::write(mTI, 0, "SppClientMsgThread::threadInitFunction");

   // Initialize the socket.
   mSocket.initialize(mSettings);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// socket connect/receives and then processes them.

void SppClientMsgThread::threadRunFunction()
{
   Trc::write(mTI, 0, "SppClientMsgThread::threadRunFunction");

   // Top of the loop.
   mFindCount = 0;
   mRestartCount = 0;
   mConnectionFlag = false;
   bool tSleepFlag = false;
   bool tShowFlag = true;
   int tRestartCode = 0;
   int tRet = 0;
   ByteContent* tMsg = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Find remote address.

find:
   // Guard.
   if (mTerminateFlag) goto end;

   if (!Ris::BtSockets::doFindAddressFromName(
      &mRemoteAddress,
      mSettings.mRemoteName,
      false))
   {
      Prn::print(0, "FIND REMOTE FAIL %d %s", mFindCount, mSettings.mRemoteName);
      BaseClass::threadSleep(4000);
      goto find;
   }

   Prn::print(0, "FIND REMOTE PASS %s %012llx %d", 
      mSettings.mRemoteName,
      mRemoteAddress.mBtAddress.btAddr,
      mRemoteAddress.mBtAddress.port);
   mFindCount++;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Restart.

restart:
   // Guard.
   if (mTerminateFlag) goto end;

   // Sleep.
   tSleepFlag = true;
   if (tSleepFlag)
   {
      BaseClass::threadSleep(1000);
   }
   tSleepFlag = true;
   Trc::write(mTI, 0, "SppClientMsgThread restart %3d $ %3d", mRestartCount,tRestartCode);
   if (tShowFlag)
   {
      Prn::print(Prn::Show1, "SppClientMsgThread restart %3d $ %3d", mRestartCount, tRestartCode);
   }
   mRestartCount++;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open device.

   // If the socket is open then close it.
   if (mSocket.mValidFlag)
   {
      Trc::write(mTI, 0, "restart close socket");
      mSocket.doClose();
      mSocket.mValidFlag = false;
   }

   // Configure the message socket.
   mSocket.configure(&mRemoteAddress, tShowFlag);
   if (!mSocket.mValidFlag)
   {
      // If error then restart.
      tRestartCode = 101;
      goto restart;
   }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Reconnect.

   // Guard.
   if (mTerminateFlag) return;
   mConnectionFlag = false;

   // Try to connect.
   if (mSocket.doConnect())
   {
      // Connection was established.
      mConnectionFlag = true;

      // Process a session change because a
      // new session has been established.
      processSessionChange(true);
   }
   else
   {
      tSleepFlag = false;
      tShowFlag = false;
      tRestartCode = 102;
      goto restart;
   }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rereceive.

receive:
   // Guard.
   if (mTerminateFlag) return;

   // Try to receive a message with a blocking receive call.
   // If a message was received then process it.
   // If a message was not received then the connection was lost.  
   if (mSocket.doReceiveMsg(tMsg))
   {
      // Message was correctly received.
      // Process the receive message.
      processRxMsg(tMsg);
      // Repeat.
      goto receive;
   }
   else
   {
      // Message was not correctly received, so
      // Connection was lost.
      mConnectionFlag = false;

      // Process a session change because a
      // session has been disestablished.
      processSessionChange(false);

      if (mSocket.mValidFlag)
      {
         tSleepFlag = false;
         tShowFlag = false;
         tRestartCode = 103;
         goto restart;
      }
      else
      {
         tSleepFlag = false;
         tShowFlag = true;
         tRestartCode = 104;
         goto restart;
      }
   }

   // Done.
end:
   Trc::write(mTI, 0, "SppClientMsgThread::threadRunFunction done");
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void SppClientMsgThread::threadExitFunction()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doRecvMsg then
// closing the socket will cause doRecvMsg to return with false and 
// then the terminate request flag will be polled and the the
// threadRunFunction will exit.

void SppClientMsgThread::shutdownThread()
{
   BaseThread::mTerminateFlag = true;

   mSocket.doClose();

   BaseThread::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Notify the parent thread that a session has changed. This is called by
// the threadRunFunction when a new session is established or an existing
// session is disestablished. It invokes the mSessionQCall that is
// registered at initialization.

void SppClientMsgThread::processSessionChange(bool aEstablished)
{
   // Guard.
   if (!mSessionQCall.isValid())
   {
      printf("ERROR processSessionChange qcall invalid\n");
      return;
   }

   if (aEstablished)
   {
      Trc::write(mTI, 0, "SppClientMsgThread CONNECTED");
   }
   else
   {
      Trc::write(mTI, 0, "SppClientMsgThread DISCONNECTED");
   }

   // Invoke the session qcall to notify that a session has been established
   // or disestablished.
   mSessionQCall(aEstablished);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pass a received message to the parent thread. This is called by the
// threadRunFunction when a message is received. It invokes the
// mRxMsgQCall that is registered at initialization.

void SppClientMsgThread::processRxMsg(Ris::ByteContent* aMsg)
{
   // Guard.
   if (!mRxMsgQCall.isValid()) return;

   // Invoke the receive callback qcall.
   mRxMsgQCall(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a transmit message through the socket to the server. It executes a
// blocking send call in the context of the calling thread. It is protected
// by a mutex semaphore.

void SppClientMsgThread::sendMsg(ByteContent* aMsg)
{
   if (!aMsg) return;

   if (mConnectionFlag)
   {
      mSocket.doSendMsg(aMsg);
   }
   else
   {
      delete aMsg;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
