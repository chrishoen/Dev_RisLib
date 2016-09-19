#ifndef _RISNETUDPMSGTHREAD_H_
#define _RISNETUDPMSGTHREAD_H_

/*==============================================================================

Udp receiver thread classes.

This file provides classes that define a single thread that provides the 
execution context for a udp socket that receives udp datagrams.

There is a base class and three classes that provide different interfaces.

1) UdpMsgAThread   provides the udp receiver thread functionality

2) UdpMsgAThreadWithQCall : public UdpMsgAThread provides a udp receiver
   thread with a QCall (queued function call) interface

3) UdpMsgAThreadWithCallback : public UdpMsgAThread provides a udp
   receiver thread with a callback interface

Threads that want to perform Udp receiver activity maintain instances of 
UdpMsgAThreadWithQCall or UdpMsgAThreadWithCallback and pass in QCalls
or callbacks in their configure calls.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"
#include "risCallPointer.h"
#include "risContainers.h"
#include "risSockets.h"
#include "risThreadsThreads.h"
#include "risThreadsQCallThread.h"
#include "risNetUdpMsgASocket.h"

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

class UdpMsgAThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   UdpMsgAThread();

   //--------------------------------------------------------------
   // Thread base class overloads:

   // configure sets configuration members.
   // threadInitFunction sets up the socket.
   // threadRunFunction does a while loop that does connect and recv calls
   void threadInitFunction(); 
   void threadRunFunction(); 
   void threadExitFunction(); 
   void shutdownThread(); 

   //--------------------------------------------------------------
   // Configure:

   // aLocalIpAddr    is the ip address of the local interface bound to
   // aLocalIpPort    is the ip port    of the local interface bound to
   // aMsgParser  is the message parser to be used on receive messages
   // aRxMsgQCall         is a qcall for receive messages

   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;

   void configure(
      char*                        aLocalIpAddress,
      int                          aLocalIpPort,
      char*                        aRemoteIpAddress,
      int                          aRemoteIpPort,
      Ris::BaseMsgAParserCreator*  aMsgParserCreator,
      RxMsgQCall*                  aRxMsgQCall);

   //--------------------------------------------------------------
   // Process:
   
   // This is called by the UdpMsgAThread threadRunFunction 
   // to process a received message.
   //
   // It invokes the mRxMsgQCall that is passed in at configure.
   void processRxMsg (Ris::ByteContent* aMsg);

   //--------------------------------------------------------------
   // QCall:

   // This is a dpc that is called when a message is received
   RxMsgQCall   mRxMsgQCall;

   //--------------------------------------------------------------
   // Transmit message:

   void sendMsg (Ris::ByteContent* aMsg);

   //--------------------------------------------------------------
   // Sockets:

   // Socket address that socket instance connects to
   char  mLocalIpAddress[40];
   int   mLocalIpPort;
   char  mRemoteIpAddress[40];
   int   mRemoteIpPort;

   // Socket instance
   UdpRxMsgASocket mRxSocket;
   UdpTxMsgASocket mTxSocket;

   // Message parser creator, this is used by the receive socket to
   // create an instance of a message parser
   BaseMsgAParserCreator* mMsgParserCreator;
};

//******************************************************************************
}//namespace
}//namespace
#endif

