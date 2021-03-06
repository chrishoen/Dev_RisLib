/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

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

   // Member variables.
   mConnectionFlag=false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It initializes the socket.

void TcpMsgClientThread::threadInitFunction()
{
   // Initialize and configure the socket.
   mSocket.initialize(mSettings);
   mSocket.configure();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server and
// receives messages.

void TcpMsgClientThread::threadRunFunction()
{
   mConnectionFlag = false;
   bool going = true;

   while(going)
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
            //printf("Connected\n");
            mConnectionFlag = true;

            // Process a session change because a
            // new session has been established.
            processSessionChange(true);
         }
         else 
         {
            // Connection was not established.
            //printf("Not Connected\n");

            mConnectionFlag = false;

            // Close the socket and reconfigure.
            mSocket.doClose();
            mSocket.reconfigure();

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
         going=false;
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
   shutdownThreadPrologue();
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
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
