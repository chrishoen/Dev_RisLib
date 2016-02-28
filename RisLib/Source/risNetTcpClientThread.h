#ifndef _RISNETTCPCLIENTTHREAD_H_
#define _RISNETTCPCLIENTTHREAD_H_

/*==============================================================================
Tcp client thread classes.

This file provides classes that define a single thread that provides the 
execution context for a tcp client that connects to a tcp server.

There is a base class and three classes that provide different interfaces.

1) TcpClientThread   provides the tcp client thread functionality

2) TcpClientThreadWithQCall : public TcpClientThread provides a tcp client
   thread with a QCall (deferred procedure call) interface

3) TcpClientThreadWithQCallAndCallSource : public TcpClientThread provides 
   a tcp client thread with a QCall (deferred procedure call) interface
   and a call source identifier that is passed in at configure
   and returned as the first QCall argument.

3) TcpClientThreadWithCallback : public TcpClientThread provides a tcp
   client thread with a callback interface

Threads that want to perform Tcp client activity maintain instances of 
TcpClientThreadWithQCall or TcpClientThreadWithCallback and pass in QCalls
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

#include "risNetTcpMsgSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
// Base Tcp Client thread.
//
// This is a single thread that provides the execution context for a tcp
// client that connects to a tcp server.
//
// It contains a stream socket that does socket connect calls to
// establish a connection with a server and then does send/recv calls
// to exchange data with it.
//
// The thread is structured around a while loop that does either a connect
// call or a recv call to do one of three things: to establish a connection to
// a server, to detect if the connection has been lost, and to receive data from
// the server.
//
// The thread also provides a transmit method that can be used to send
// a message out the socket. The method uses a mutex semaphore for
// mutual exclusion and a blocking send call executes in the context of the
// calling thread.
//
// The thread provides serialized access to the socket and associated 
// state variables and it provides the context for the blocking of the 
// recv call.

class TcpClientThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   TcpClientThread();

   //--------------------------------------------------------------
   // Configure:

   // aServerIpAddr     is the server ip address
   // aServerIpPort     is the server ip port
   // aMessageParser    is the message parser to be used on receive messages
   // aRxMsgQCall         is a qcall for receive messages
   // aSessionQCallChange is a qcall for session changes

   typedef Ris::Threads::QCall1<bool>              SessionQCall;
   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;

   void configure(
      char*                          aServerIpAddr,
      int                            aServerIpPort,
      Ris::BaseMessageParserCreator* aMessageParser,
      SessionQCall*                  aSessionQCall,
      RxMsgQCall*                    aRxMsgQCall,
      int                            aFlags=0); 

   //--------------------------------------------------------------
   // Thread base class overloads:

   // threadInitFunction sets up the socket.
   // threadRunFunction does a while loop that does connect and recv calls
   // threadExitFunction shuts down the socket.
   void threadInitFunction(); 
   void threadRunFunction(); 
   void threadExitFunction(); 
   void shutdownThread(); 

   //--------------------------------------------------------------
   // Process:
   
   // This is called by the TcpClientThread threadRunFunction 
   // when a new session is established or an existing session is 
   // disestablished. It notifies the user of this thread that a
   // session has changed.
   //
   // It invokes the mSessionQCall that is passed in at configure.
   void processSessionChange  (bool aEstablished);

   // This is called by the TcpClientThread threadRunFunction 
   // to process a received message.
   //
   // It invokes the mRxMsgQCall that is passed in at configure.
   void processRxMsg          (Ris::ByteContent* aRxMsg);

   //--------------------------------------------------------------
   // QCall:

   // This is a qcall that is called when a session is
   // established or disestablished.
   SessionQCall mSessionQCall;

   // This is a qcall that is called when a message is received
   RxMsgQCall   mRxMsgQCall;

   //--------------------------------------------------------------
   // Transmit message:

   // This sends a transmit message through the socket to the server
   // It executes a blocking send() call in the context of the caller.
   // It is protected by a mutex semaphore.
   void sendMsg(ByteContent* aTxMsg);

   //--------------------------------------------------------------
   // Sockets:

   // Socket instance
   TcpMsgSocket mSocket;

   // Socket address that socket instance connects to
   Sockets::SocketAddress mSocketAddress;

   // Message parser creator, this is used by the socket to
   // create an instance of a message parser
   BaseMessageParserCreator* mMessageParserCreator;

   //--------------------------------------------------------------
   // State:

   // If this flag is true then a connection has been established 
   // with the server and sendMsg can be called.
   bool mConnectionFlag;

   int  mFlags;
};

//******************************************************************************
}//namespace
}//namespace
#endif

