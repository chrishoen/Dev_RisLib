#ifndef _RISNETUDPRXTHREAD_H_
#define _RISNETUDPRXTHREAD_H_

/*==============================================================================

Udp receiver thread classes.

This file provides classes that define a single thread that provides the 
execution context for a udp socket that receives udp datagrams.

There is a base class and three classes that provide different interfaces.

1) BaseUdpRxThread   provides the udp receiver thread functionality

2) UdpRxThreadWithQCall : public BaseUdpRxThread provides a udp receiver
   thread with a QCall (queued function call) interface

3) UdpRxThreadWithCallback : public BaseUdpRxThread provides a udp
   receiver thread with a callback interface

Threads that want to perform Udp receiver activity maintain instances of 
UdpRxThreadWithQCall or UdpRxThreadWithCallback and pass in QCalls
or callbacks in their configure calls.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"
#include "risCallPointer.h"
#include "risContainers.h"
#include "risSockets.h"
#include "risThreads.h"
#include "risThreadsQCall.h"

#include "risNetUdpMsgSocket.h"

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

class  BaseUdpRxThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   BaseUdpRxThread();

   //--------------------------------------------------------------
   // Configure:

   // aLocalIpAddr    is the ip address of the local interface bound to
   // aLocalIpPort    is the ip port    of the local interface bound to
   // aMessageParser  is the message parser to be used on receive messages

   void configure(
      char*                     aLocalIpAddr,
      int                       aLocalIpPort,
      Ris::BaseMessageParser*   aMessageParser); 

   //--------------------------------------------------------------
   // ConfigureForMulticast:

   // aMulticastGroup is the ip address of multicast group
   // aLocalIpAddr    is the ip address of the local interface bound to
   // aLocalIpPort    is the ip port    of the local interface bound to
   // aMessageParser  is the message parser to be used on receive messages

   void configureForMulticast(
      char*                     aMulticastGroup,
      char*                     aLocalIpAddr,
      int                       aLocalIpPort,
      Ris::BaseMessageParser*   aMessageParser); 

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
   // Process, supplied by inheritors
   
   // This is called by the threadRunFunction to process a received
   // message.
   virtual void processRxMsg (Ris::ByteContent* aRxMsg)=0;

   //--------------------------------------------------------------
   // Sockets:

   // Socket instance
   UdpRxMsgSocket mRxSocket;

   // Socket address that socket instance connects to
   Sockets::SocketAddress mRxSocketAddress;

   // Ip address of optional multicast group
   Sockets::IpAddress mMulticastGroup;
   bool               mMulticastFlag;

   // Message parser for a stream socket
   BaseMessageParser* mMessageParser;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp receiver thread with a QCall interface. 
//
// This interfaces to an owner thread that is based on a call queue. The owner
// configures with QCalls that are invoked when a session changes or when a 
// message is received.

class  UdpRxThreadWithQCall : public BaseUdpRxThread
{
public:

   //--------------------------------------------------------------
   // Configure:

   // aLocalIpAddr    is the ip address of the local interface bound to
   // aLocalIpPort    is the ip port    of the local interface bound to
   // aMessageParser  is the message parser to be used on receive messages
   // aRxMsgQCall         is a qcall for receive messages

   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;

   void configure(
      char*                   aLocalIpAddr,
      int                     aLocalIpPort,
      Ris::BaseMessageParser* aMessageParser,
      RxMsgQCall*             aRxMsgQCall);

   //--------------------------------------------------------------
   // ConfigureForMulticast:

   // aMulticastGroup is the ip address of multicast group
   // aLocalIpAddr    is the ip address of the local interface bound to
   // aLocalIpPort    is the ip port    of the local interface bound to
   // aMessageParser  is the message parser to be used on receive messages

   void configureForMulticast(
      char*                     aMulticastGroup,
      char*                     aLocalIpAddr,
      int                       aLocalIpPort,
      Ris::BaseMessageParser*   aMessageParser, 
      RxMsgQCall*               aRxMsgQCall);

   //--------------------------------------------------------------
   // Process:
   
   // This is called by the BaseUdpRxThread threadRunFunction 
   // to process a received message.
   //
   // It invokes the mRxMsgQCall that is passed in at configure.
   void processRxMsg          (Ris::ByteContent* aRxMsg);

   //--------------------------------------------------------------
   // QCall:

   // This is a dpc that is called when a message is received
   RxMsgQCall   mRxMsgQCall;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp receiver thread with a callback interface.
//
// This interfaces to an owner thread that is based on callbacks. The owner
// configures with CallPointers that are called when a session changes or
// when a message is received.

class  UdpRxThreadWithCallback : public BaseUdpRxThread
{
public:
   
   //--------------------------------------------------------------
   // Definitions:

   // CallPointer definition for function with signature:
   // void processRxMsg(Ris::ByteContent* aMsg).
   typedef Ris::CallPointer1<Ris::ByteContent*> MsgCallPointer;

   //--------------------------------------------------------------
   // Configure:

   // aLocalIpAddr    is the ip address of the local interface bound to
   // aLocalIpPort    is the ip port    of the local interface bound to
   // aMessageParser  is the message parser to be used on receive messages
   // aRxCallback      is called when a message is received

   void configure(
      char*                     aLocalIpAddr,
      int                       aLocalIpPort,
      Ris::BaseMessageParser*   aMessageParser, 
      MsgCallPointer*           aRxCallback);

   //--------------------------------------------------------------
   // Process:
   
   // This is called by the BaseUdpRxThread threadRunFunction 
   // to process a received message.
   //
   // It calls the mRxCallback that is passed in at configure.

   void processRxMsg (Ris::ByteContent* aRxMsg);

   //--------------------------------------------------------------
   // Callbacks:

   // This is a callback that is called when a message is received
   MsgCallPointer mRxCallback;
};

//******************************************************************************
}//namespace
}//namespace
#endif

