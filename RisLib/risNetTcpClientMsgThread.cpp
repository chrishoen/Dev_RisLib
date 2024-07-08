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
#include "risNetTcpClientMsgThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TcpClientMsgThread::TcpClientMsgThread(Settings& aSettings)
{
   // Base class variables.
   BaseClass::setThreadName("TcpMsgClient");
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
   mRestartCount = 0;
   mRxCount = 0;
   mTxCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It initializes the socket.

void TcpClientMsgThread::threadInitFunction()
{
   Trc::write(mTI, 0, "TcpClientMsgThread::threadInitFunction");

   // Initialize the socket.
   mSocket.initialize(&mSettings);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// socket connect/receives and then processes them.

void TcpClientMsgThread::threadRunFunction()
{
   Trc::write(mTI, 0, "TcpClientMsgThread::threadRunFunction");

   // Top of the loop.
   mRestartCount = 0;
   mConnectionFlag = false;
   bool tSleepFlag = false;
   bool tShowFlag = true;
   int tRestartCode = 0;
   int tRet = 0;
   ByteContent* tMsg = 0;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Restart.

Restart:

   // Guard.
   if (mTerminateFlag) goto End;

   // Sleep.
   tSleepFlag = true;
   if (tSleepFlag)
   {
      BaseClass::threadSleep(1000);
   }
   tSleepFlag = true;
   Trc::write(mTI, 0, "TcpClientMsgThread restart %3d $ %3d", mRestartCount,tRestartCode);
   if (tShowFlag)
   {
      Prn::print(Prn::Show1, "TcpClientMsgThread restart %3d $ %3d", mRestartCount, tRestartCode);
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
   mSocket.configure(tShowFlag);
   if (!mSocket.mValidFlag)
   {
      // If error then restart.
      tRestartCode = 101;
      goto Restart;
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
      // Connection was not established, restart.
      tSleepFlag = false;
      tShowFlag = false;
      tRestartCode = 102;
      goto Restart;
   }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rereceive.

Receive:
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
      goto Receive;
   }
   else
   {
      // Message was not correctly received,
      // so the cconnection was lost.
      mConnectionFlag = false;

      // Process a session change because a
      // session has been disestablished.
      processSessionChange(false);

      if (mSocket.mValidFlag)
      {
         tSleepFlag = false;
         tShowFlag = false;
         tRestartCode = 103;
         goto Restart;
      }
      else
      {
         tSleepFlag = false;
         tShowFlag = true;
         tRestartCode = 104;
         goto Restart;
      }
   }

   // Done.
End:
   Trc::write(mTI, 0, "TcpClientMsgThread::threadRunFunction done");
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void TcpClientMsgThread::threadExitFunction()
{
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

void TcpClientMsgThread::shutdownThread()
{
   Trc::write(mTI, 1, "TcpClientMsgThread::shutdownThread");

   // Set the terminate flag, close the socket, and wait for the thread
   // to terminate.
   BaseThread::mTerminateFlag = true;
   mSocket.doClose();
   BaseThread::waitForThreadTerminate();

   Trc::write(mTI, 1, "TcpClientMsgThread::shutdownThread done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Notify the parent thread that a session has changed. This is called by
// the threadRunFunction when a new session is established or an existing
// session is disestablished. It invokes the mSessionQCall that is
// registered at initialization.

void TcpClientMsgThread::processSessionChange(bool aEstablished)
{
   // Guard.
   if (!mSessionQCall.isValid())
   {
      printf("ERROR processSessionChange qcall invalid\n");
      return;
   }

   if (aEstablished)
   {
      Trc::write(mTI, 0, "TcpClientMsgThread CONNECTED");
   }
   else
   {
      Trc::write(mTI, 0, "TcpClientMsgThread DISCONNECTED");
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

void TcpClientMsgThread::processRxMsg(Ris::ByteContent* aMsg)
{
   // Guard.
   if (!mRxMsgQCall.isValid()) return;

   // Invoke the receive callback qcall.
   mRxMsgQCall(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the socket to restart the loop. This causes the recv to fail
// and a disconnect to occur.

void TcpClientMsgThread::doRestart()
{
   mSocket.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a transmit message through the socket to the server. It executes a
// blocking send call in the context of the calling thread.

void TcpClientMsgThread::doSendMsg(ByteContent* aMsg)
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
