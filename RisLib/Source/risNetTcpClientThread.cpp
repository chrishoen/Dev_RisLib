/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "ris_priorities.h"
#include "risNetTcpClientThread.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TcpClientThread::TcpClientThread()
{
   mThreadPriority = get_default_tcp_client_thread_priority();

   mConnectionFlag=false;
   mFlags=0;
}

//******************************************************************************
// Configure:

void TcpClientThread::configure(
   char*                      aServerIpAddr,
   int                        aServerIpPort,
   BaseMessageParserCreator*  aMessageParserCreator,
   SessionQCall*              aSessionQCall,
   RxMsgQCall*                aRxMsgQCall,
   int                        aFlags) 
{
   Prn::print(Prn::SocketInit,Prn::Init1, "TcpClientThread::configure");

   mConnectionFlag=false;
   mFlags=aFlags;
   mSocketAddress.set(aServerIpAddr,aServerIpPort);
   mMessageParserCreator = aMessageParserCreator;

   mSessionQCall = *aSessionQCall;
   mRxMsgQCall   = *aRxMsgQCall;
}

//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void TcpClientThread::threadInitFunction()
{
   Prn::print(Prn::SocketInit,Prn::Init1, "TcpClientThread::threadInitFunction BEGIN");

   // Configure the socket
   mSocket.configure(mSocketAddress,mMessageParserCreator);

   Prn::print(Prn::SocketInit,Prn::Init1, "TcpClientThread::threadInitFunction END");
}

//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server
// and receives messages.

void TcpClientThread::threadRunFunction()
{
   Prn::print(Prn::SocketRun,Prn::Run1, "TcpClientThread::threadRunFunction");
   
   //-----------------------------------------------------------
   // Loop

   mConnectionFlag=false;

   bool going=true;

   while(going)
   {
      //-----------------------------------------------------------
      // If no connection
      if (!mConnectionFlag)
      {
         // Try to connect
         if (mSocket.doConnect())
         {
            // Connection was established
            Prn::print(Prn::SocketRun,Prn::Run1, "Connected");
            mConnectionFlag = true;

            // process a session change because a
            // new session has been established
            processSessionChange(true);
         }
         else 
         {
            // Connection was not established
            Prn::print(Prn::SocketRun,Prn::Run3, "Not Connected");

            mConnectionFlag = false;

            // Close socket
            mSocket.doClose();
            mSocket.reconfigure();

            // Sleep
            threadSleep(500);
         }
      }
      //-----------------------------------------------------------
      // If connection
      else
      {
         // Try to receive a message with a blocking receive call
         // If a message was received then process it.
         // If a message was not received then the connection was lost.  
         ByteContent* rxMsg=0;
         if (mSocket.doRecvMsg(rxMsg))
         {
            // Message was correctly received
            Prn::print(Prn::SocketRun,Prn::Run2, "Recv message %d",mSocket.mRxMsgCount);

            // process the receive message
            if (rxMsg)
            {
               processRxMsg(rxMsg);
            }
         }
         else
         {
            // Message was not correctly received, so
            // Connection was lost
            Prn::print(Prn::SocketRun,Prn::Run1, "Recv failed, Connection lost");
            mConnectionFlag = false;

            // process a session change because a
            // new session has been disestablished
            processSessionChange(false);
         }
      }
      //-----------------------------------------------------------
      // If termination request, exit the loop
      // This is set by shutdown, see below.
      if (mTerminateFlag)
      {
         going=false;
      }  
   }         
}

//******************************************************************************
// Thread exit function, base class overload.

void TcpClientThread::threadExitFunction()
{
   Prn::print(Prn::SocketInit,Prn::Init1, "TcpClientThread::threadExitFunction");
}
//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doRecvMsg then
// closing the socket will cause doRecvMsg to return with false and 
// then the terminate request flag will be polled and the the
// threadRunFunction will exit.

void TcpClientThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mSocket.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}
//******************************************************************************

void TcpClientThread::sendMsg(ByteContent* aTxMsg)
{
   if (!aTxMsg) return;

   if (mConnectionFlag)
   {
      mSocket.doSendMsg(aTxMsg);
   }
   else
   {
      delete aTxMsg;
   }

   Prn::print(Prn::SocketRun,Prn::Run2, "doSendMsg %d %d %d",mSocket.mStatus,mSocket.mError,mSocket.mTxMsgCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TcpClientThread::processSessionChange(bool aEstablished)
{
   // Invoke the session qcall to notify that a session has
   // been established or disestablished
   // Create a new qcall, copied from the original, and invoke it.
   mSessionQCall.invoke(aEstablished);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TcpClientThread::processRxMsg(Ris::ByteContent* aRxMsg)
{
   // Invoke the receive QCall
   // Create a new qcall, copied from the original, and invoke it.
   mRxMsgQCall.invoke(aRxMsg);
}

//******************************************************************************

}//namespace
}//namespace
