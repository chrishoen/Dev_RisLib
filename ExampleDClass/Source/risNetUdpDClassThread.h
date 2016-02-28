#ifndef _RISNETUDPDCLASSTHREAD_H_
#define _RISNETUDPDCLASSTHREAD_H_

/*==============================================================================

Udp receiver thread classes.

This file provides classes that define a single thread that provides the 
execution context for a udp socket that receives udp datagrams.

There is a base class and three classes that provide different interfaces.

1) UdpDClassThread   provides the udp receiver thread functionality

2) UdpDClassThreadWithQCall : public UdpDClassThread provides a udp receiver
   thread with a QCall (queued function call) interface

3) UdpDClassThreadWithCallback : public UdpDClassThread provides a udp
   receiver thread with a callback interface

Threads that want to perform Udp receiver activity maintain instances of 
UdpDClassThreadWithQCall or UdpDClassThreadWithCallback and pass in QCalls
or callbacks in their configure calls.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCallPointer.h"
#include "risContainers.h"
#include "risSockets.h"
#include "risThreadsThreads.h"
#include "risThreadsQCallThread.h"
#include "risNetUdpDClassSocket.h"

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

class UdpDClassThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   UdpDClassThread();

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
   // aMessageParser  is the message parser to be used on receive messages
   // aRxDClassQCall         is a qcall for receive messages

   typedef Ris::Threads::QCall1<DHandle> RxDClassQCall;

   void configure(
      char*                   aLocalIpAddress,
      int                     aLocalIpPort,
      char*                   aRemoteIpAddress,
      int                     aRemoteIpPort,
      Ris::BaseDClassCopier*  aClassCopier,
      RxDClassQCall*          aRxDClassQCall);

   //--------------------------------------------------------------
   // Process:
   
   // This is called by the UdpDClassThread threadRunFunction 
   // to process a received message.
   //
   // It invokes the mRxDClassQCall that is passed in at configure.
   void processRxDClass       (DHandle aRxDClass);

   //--------------------------------------------------------------
   // QCall:

   // This is a dpc that is called when a message is received
   RxDClassQCall   mRxDClassQCall;

   //--------------------------------------------------------------
   // Transmit message:

   void sendDClass (DHandle aClass);

   //--------------------------------------------------------------
   // Sockets:

   // Socket address that socket instance connects to
   char  mLocalIpAddress[40];
   int   mLocalIpPort;
   char  mRemoteIpAddress[40];
   int   mRemoteIpPort;

   // Socket instance
   UdpRxDClassSocket mRxSocket;
   UdpTxDClassSocket mTxSocket;

   // Message parser creator, this is used by the receive socket to
   // create an instance of a message parser
   BaseDClassCopier* mDClassCopier;
};

//******************************************************************************
}//namespace
}//namespace
#endif

