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
#include "risNetTcpMsgClientThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TcpMsgClientThread::TcpMsgClientThread(Settings& aSettings)
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

void TcpMsgClientThread::threadInitFunction()
{
   Trc::write(mTI, 1, "TcpMsgClientThread::threadInitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately
// after the thread init function. It runs a loop that blocks on 
// the udp port receives and then processes them. The loop terminates
// when the serial port receive is aborted.

void TcpMsgClientThread::threadRunFunction()
{
   Trc::write(mTI, 0, "TcpMsgClientThread::threadRunFunction");

   // Top of the loop.
   mRestartCount = 0;
   mConnectionFlag = false;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Restart.

restart:
   printf("$$$$$$$$$$$$$$$$$$$$$$ restart\n");
   // Guard.
   if (mTerminateFlag) return;
   int tRet = 0;

   // Sleep.
   if (mRestartCount > 0)
   {
      BaseClass::threadSleep(1000);
   }
   Trc::write(mTI, 0, "TcpMsgClientThread restart %d", mRestartCount);
   Prn::print(Prn::Show1, "TcpMsgClientThread restart %d", mRestartCount);
   mRestartCount++;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open device.

   // If the socket is open then close it.
   if (mSocket.mValidFlag)
   {
      mSocket.doClose();
   }

   // Initialize and configure the message socket.
   mSocket.initialize(mSettings);
   mSocket.configure();
   if (!mSocket.mValidFlag)
   {
      // If error then restart.
      goto restart;
   }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Reconnect.

reconnect:
   printf("$$$$$$$$$$$$$$$$$$$$$$ reconnect\n");
   // Guard.
   if (mTerminateFlag) return;
   mConnectionFlag = false;

   // Try to connect.
   if (mSocket.doConnect())
   {
      // Connection was established.
      Trc::write(mTI, 0, "TcpMsgClientThread CONNECTED");
      mConnectionFlag = true;

      // Process a session change because a
      // new session has been established.
      processSessionChange(true);
   }
   else
   {
      // Connection failed.
      Trc::write(mTI, 0, "TcpMsgClientThread CONNECT FAILED");

      // If error then restart.
      goto restart;
   }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Rereceive.

rereceive:
   printf("$$$$$$$$$$$$$$$$$$$$$$ rereceive\n");
   // Guard.
   if (mTerminateFlag) return;

   // Try to receive a message with a blocking receive call.
   // If a message was received then process it.
   // If a message was not received then the connection was lost.  
   ByteContent* tMsg = 0;
   if (mSocket.doReceiveMsg(tMsg))
   {
      // Message was correctly received.
      // Process the receive message.
      if (tMsg)
      {
         processRxMsg(tMsg);
         // Repeat.
         goto rereceive;
      }
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
         printf("$$$$$$$$$$$$$$$$$$$$$$ receive failed VALID\n");
         goto reconnect;
      }
      else
      {
         printf("$$$$$$$$$$$$$$$$$$$$$$ receive failed INVALID\n");
         goto restart;
      }

   }

   // Done.
end:
   Trc::write(mTI, 0, "TcpMsgClientThread::threadRunFunction done");
   return;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server and
// receives messages.

void TcpMsgClientThread::threadRunFunction22()
{
   Trc::write(mTI, 0, "TcpMsgClientThread::threadRunFunction");

   // Initialize and configure the socket.
   mSocket.initialize(mSettings);
   mSocket.configure();

   mConnectionFlag = false;
   bool tGoing = mSocket.mValidFlag;

   while(tGoing)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // If not connected.

      if (!mConnectionFlag)
      {
         // Try to connect.
         if (mSocket.doConnect())
         {
            // Connection was established.
            Trc::write(mTI, 0, "TcpMsgClientThread CONNECTED");
            mConnectionFlag = true;

            // Process a session change because a
            // new session has been established.
            processSessionChange(true);
         }
         else 
         {
            // Connection was not established.
            Trc::write(mTI, 0, "TcpMsgClientThread DISCONNECTED");

            mConnectionFlag = false;

            // Close the socket and reconfigure.
            mSocket.doClose();
            mSocket.reconfigure();
//          mSocket.configure();

            // Sleep.
            threadSleep(500);
         }
      }

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // If connected.
      else
      {
         // Try to receive a message with a blocking receive call.
         // If a message was received then process it.
         // If a message was not received then the connection was lost.  
         ByteContent* tMsg=0;
         if (mSocket.doReceiveMsg(tMsg))
         {
            // Message was correctly received.
            // Process the receive message.
            if (tMsg)
            {
               processRxMsg(tMsg);
            }
         }
         else
         {
            // Message was not correctly received, so
            // Connection was lost.
            //printf("Recv failed, Connection lost\n");
            mConnectionFlag = false;

            // Process a session change because a
            // session has been disestablished.
            processSessionChange(false);
         }
      }

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // If termination request, exit the loop
      // This is set by shutdown, see below.

      if (mTerminateFlag)
      {
         tGoing=false;
      }  
   }         
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void TcpMsgClientThread::threadExitFunction()
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

void TcpMsgClientThread::shutdownThread()
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

void TcpMsgClientThread::processSessionChange(bool aEstablished)
{
   // Guard.
   if (!mSessionQCall.isValid())
   {
      printf("ERROR processSessionChange qcall invalid\n");
      return;
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

void TcpMsgClientThread::processRxMsg(Ris::ByteContent* aMsg)
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

void TcpMsgClientThread::sendMsg(ByteContent* aMsg)
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
