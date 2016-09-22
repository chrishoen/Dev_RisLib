#ifndef _RISNETUDPMSGBTHREAD_H_
#define _RISNETUDPMSGBTHREAD_H_

/*==============================================================================

Udp receiver thread classes.

This file provides classes that define a single thread that provides the 
execution context for a udp socket that receives udp datagrams.

There is a base class and three classes that provide different interfaces.

1) UdpMsgBThread   provides the udp receiver thread functionality

2) UdpMsgBThreadWithQCall : public UdpMsgBThread provides a udp receiver
   thread with a QCall (queued function call) interface

3) UdpMsgBThreadWithCallback : public UdpMsgBThread provides a udp
   receiver thread with a callback interface

Threads that want to perform Udp receiver activity maintain instances of 
UdpMsgBThreadWithQCall or UdpMsgBThreadWithCallback and pass in QCalls
or callbacks in their configure calls.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risThreadsQCall.h"
#include "risNetUdpMsgBSocket.h"
#include "exampleMsgTraits.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
// Base Udp Client thread.
//
// This is a single thread that provides the execution context for a udp
// receiver that connects to a udp server.
//
// It contains a udp socket that is bound to a local address.
//
// The thread is structured around a while loop that does a recvfrom
// call to receive a message on the socket.
//
// The thread provides serialized access to the socket and associated 
// state variables and it provides the context for the blocking of the 
// recv call.

//template <class MsgTraits>
class UdpMsgBThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Socket address that socket instance connects to
   char  mLocalIpAddress[40];
   int   mLocalIpPort;
   char  mRemoteIpAddress[40];
   int   mRemoteIpPort;

   // Socket instance
// UdpRxMsgBSocket<MsgTraits> mRxSocket;
// UdpTxMsgBSocket<MsgTraits> mTxSocket;
   UdpRxMsgBSocket<ExampleMsg::MsgTraits> mRxSocket;
   UdpTxMsgBSocket<ExampleMsg::MsgTraits> mTxSocket;

   // This is a qcall that is called when a message is received
   typedef Ris::Threads::QCall1<Ris::ByteMsgB*> RxMsgQCall;

   RxMsgQCall   mRxMsgQCall;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constructor.

   UdpMsgBThread()
   {
      mThreadPriority = get_default_udp_rx_thread_priority();
      mLocalIpAddress[0]=0;
      mLocalIpPort=0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Configure:
   // aLocalIpAddr    is the ip address of the local interface bound to
   // aLocalIpPort    is the ip port    of the local interface bound to
   // aMsgParser  is the message parser to be used on receive messages
   // aRxMsgQCall         is a qcall for receive messages

   void configure(
      char*                 aLocalIpAddress,
      int                   aLocalIpPort,
      char*                 aRemoteIpAddress,
      int                   aRemoteIpPort,
      RxMsgQCall*           aRxMsgQCall)
   {
      strcpy(mLocalIpAddress,aLocalIpAddress);
      mLocalIpPort = aLocalIpPort;

      strcpy(mRemoteIpAddress, aRemoteIpAddress);
      mRemoteIpPort = aRemoteIpPort;

      mRxMsgQCall = *aRxMsgQCall;
   }
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread base class overloads:

   //***************************************************************************
   // Thread init function, base class overload.
   // It configures the socket.

   void UdpMsgBThread::threadInitFunction() 
   {
      Prn::print(Prn::SocketInit1, "UdpMsgBThread::threadInitFunction BEGIN");

      mRxSocket.configure(
         mLocalIpAddress,
         mLocalIpPort);

      mTxSocket.configure(
         mRemoteIpAddress,
         mRemoteIpPort);
   
      Prn::print(Prn::SocketInit1, "UdpMsgBThread::threadInitFunction END");
   }

   //***************************************************************************
   // Thread run function, base class overload.
   // It contains a while loop that manages the connection to the server
   // and receives messages.

   void  UdpMsgBThread::threadRunFunction22()
   {
      Prn::print(Prn::SocketRun1, "UdpRxMsgBThread::threadRunFunction");

      //-----------------------------------------------------------
      // Loop

      bool going=mRxSocket.mValidFlag;

      while(going)
      {
         // Try to receive a message with a blocking receive call
         // If a message was received then process it.
         // If a message was not received then the connection was lost.  
         ByteMsgB* tMsg=0;
         if (mRxSocket.doReceiveMsg(tMsg))
         {
            // Message was correctly received
            Prn::print(Prn::SocketRun1, "Recv message %d %d",mRxSocket.mRxCount,tMsg->mMessageType);

            // Call the receive method
            processRxMsg(tMsg);
         }
         else
         {
            // Message was not correctly received, so
            // Connection was lost
            Prn::print(Prn::SocketRun1, "Recv failed, Connection lost");
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

   //***************************************************************************
   // Thread exit function, base class overload.

   void UdpMsgBThread::threadExitFunction22()
   {
      Prn::print(Prn::SocketInit1, "UdpMsgBThread::threadExitFunction");
   }

   //***************************************************************************
   // Shutdown, base class overload.
   // This sets the terminate request flag and closes the socket.
   //
   // If the while loop in the threadRunFunction is blocked on doReceiveMsg then
   // closing the socket will cause doReceiveMsg to return with false and 
   // then the terminate request flag will be polled and the threadRunFunction 
   // will exit.

   void UdpMsgBThread::shutdownThread22()
   {
      BaseThreadWithTermFlag::mTerminateFlag = true;

      mRxSocket.doClose();

      BaseThreadWithTermFlag::waitForThreadTerminate();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This is called by the UdpMsgBThread threadRunFunction 
   // to process a received message.
   //
   // It invokes the mRxMsgQCall that is passed in at configure.

   void processRxMsg       (Ris::ByteMsgB* aMsg)
   {
      // Invoke the receive QCall
      // Create a new qcall, copied from the original, and invoke it.
      mRxMsgQCall(aMsg);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Transmit message:

   void sendMsg (Ris::ByteMsgB* aMsg)
   {
      mTxSocket.doSendMsg(aMsg);
   }

};

//******************************************************************************
}//namespace
}//namespace
#endif

