#ifndef _RISNETTCPCLIENTTHREAD_H_
#define _RISNETTCPCLIENTTHREAD_H_

/*==============================================================================
Tcp client thread classes.

This file provides classes that define a single thread that provides the 
execution context for a tcp client that connects to a tcp server.

There is a base class and three classes that provide different interfaces.

1) BaseTcpClientThread   provides the tcp client thread functionality

2) TcpClientThreadWithQCall : public BaseTcpClientThread provides a tcp client
   thread with a QCall (deferred procedure call) interface

3) TcpClientThreadWithQCallAndCallSource : public BaseTcpClientThread provides 
   a tcp client thread with a QCall (deferred procedure call) interface
   and a call source identifier that is passed in at configure
   and returned as the first QCall argument.

3) TcpClientThreadWithCallback : public BaseTcpClientThread provides a tcp
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
#include "risThreads.h"
#include "risThreadsQCall.h"

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

class  BaseTcpClientThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   BaseTcpClientThread();

   //--------------------------------------------------------------
   // Configure:

   // aServerIpAddr    is the server ip address
   // aServerIpPort    is the server ip port
   // aMessageParser   is the message parser to be used on receive messages

   void configure(
      char*                     aServerIpAddr,
      int                       aServerIpPort,
      Ris::BaseMessageParser*   aMessageParser,
      int                       aFlags=0); 

   //--------------------------------------------------------------
   // Thread base class overloads:

   // configure sets configuration members.
   // threadInitFunction sets up the socket.
   // threadRunFunction does a while loop that does connect and recv calls
   void configureThread(); 
   void threadInitFunction(); 
   void threadRunFunction(); 
   void threadExitFunction(); 
   void shutdownThread(); 

   //--------------------------------------------------------------
   // Process, supplied by inheritors
   
   // This is called by the threadRunFunction when a new session is
   // established or an existing session is disestablished. It notifies
   // the user of this thread that a session has changed.
   virtual void processSessionChange  (bool aEstablished)=0;

   // This is called by the threadRunFunction to process a received
   // message.
   virtual void processRxMsg          (Ris::ByteContent* aRxMsg)=0;

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

   // Message parser for a stream socket
   BaseMessageParser* mMessageParser;

   //--------------------------------------------------------------
   // State:

   // If this flag is true then a connection has been established 
   // with the server and sendMsg can be called.
   bool mConnectionFlag;

   int  mFlags;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Tcp client thread with a QCall interface. 
//
// This interfaces to an owner thread that is based on a call queue. The owner
// configures with QCalls that are invoked when a session changes or when a 
// message is received.

class  TcpClientThreadWithQCall : public BaseTcpClientThread
{
public:

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
      char*                   aServerIpAddr,
      int                     aServerIpPort,
      Ris::BaseMessageParser* aMessageParser,
      SessionQCall*           aSessionQCall,
      RxMsgQCall*             aRxMsgQCall,
      int                     aFlags=0); 

   //--------------------------------------------------------------
   // Process:
   
   // This is called by the BaseTcpClientThread threadRunFunction 
   // when a new session is established or an existing session is 
   // disestablished. It notifies the user of this thread that a
   // session has changed.
   //
   // It invokes the mSessionQCall that is passed in at configure.
   void processSessionChange  (bool aEstablished);

   // This is called by the BaseTcpClientThread threadRunFunction 
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
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Tcp client thread with a QCall interface and a call source identifier. 
//
// This interfaces to an owner thread that is based on a call queue. The owner
// configures with QCalls that are invoked when a session changes or when a 
// message is received. 
//
// It also supplies a CallSource identifier that is  passed to the thread at 
// configure and returned with  each qcall invokation. It is used by an owning
// thread that contains multiple instances of this threadand has a common
// QCall execute member function that is invoked by the multiple instances.
// The CallSource is used to identify which of the multiple instances
// invoked the QCall.
 

class  TcpClientThreadWithQCallAndCallSource : public BaseTcpClientThread
{
public:
   TcpClientThreadWithQCallAndCallSource();

   //--------------------------------------------------------------
   // Configure:

   // aServerIpAddr       is the server ip address
   // aServerIpPort       is the server ip port
   // aMessageParser      is the message parser to be used on receive messages
   // aCallSource         is an identifier that is passed to the QCalls
   // aRxMsgQCall         is a qcall for receive messages
   // aSessionQCallChange is a qcall for session changes

   typedef Ris::Threads::QCall2<int,bool>              SessionQCall;
   typedef Ris::Threads::QCall2<int,Ris::ByteContent*> RxMsgQCall;

   void configure(
      char*                   aServerIpAddr,
      int                     aServerIpPort,
      Ris::BaseMessageParser* aMessageParser,
      int                     aCallSource,
      SessionQCall*           aSessionQCall,
      RxMsgQCall*             aRxMsgQCall,
      int                     aFlags=0); 

   //--------------------------------------------------------------
   // Process:
   
   // This is called by the BaseTcpClientThread threadRunFunction 
   // when a new session is established or an existing session is 
   // disestablished. It notifies the user of this thread that a
   // session has changed.
   //
   // It invokes the mSessionQCall that is passed in at configure.
   void processSessionChange  (bool aEstablished);

   // This is called by the BaseTcpClientThread threadRunFunction 
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

   // This is set at configure and passed to each QCall. It is used
   // by owner threads to identify which instance of this class
   // invoked the QCall.
   int mCallSource;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Tcp client thread with a callback interface.
//
// This interfaces to an owner thread that is based on callbacks. The owner
// configures with CallPointers that are called when a session changes or
// when a message is received.

class  TcpClientThreadWithCallback : public BaseTcpClientThread
{
public:
   
   //--------------------------------------------------------------
   // Definitions:

   // CallPointer definition for function with signature:
   // void processSession(bool aEstablish);
   typedef CallPointer1<bool> SessionNotifyCallPointer;

   // CallPointer definition for function with signature:
   // void processRxMsg(Ris::ByteContent* aMsg).
   typedef Ris::CallPointer1<Ris::ByteContent*> MsgCallPointer;

   //--------------------------------------------------------------
   // Configure:

   // aServerIpAddr    is the server ip address
   // aServerIpPort    is the server ip port
   // aMessageParser   is the message parser to be used on receive messages
   // aRxCallback      is called when a message is received
   // aSessionCallback is called when the session changes connection state

   void configure(
      char*                     aServerIpAddr,
      int                       aServerIpPort,
      Ris::BaseMessageParser*   aMessageParser, 
      MsgCallPointer*           aRxCallback,
      SessionNotifyCallPointer* aSessionCallback,
      int                       aFlags=0); 

   //--------------------------------------------------------------
   // Process:
   
   // This is called by the BaseTcpClientThread threadRunFunction 
   // when a new session is established or an existing session is 
   // disestablished. It notifies the user of this thread that a
   // session has changed.
   //
   // It calls the mSessionCallback that is passed in at configure.

   void processSessionChange  (bool aEstablished);

   // This is called by the BaseTcpClientThread threadRunFunction 
   // to process a received message.
   //
   // It calls the mRxCallback that is passed in at configure.

   void processRxMsg          (Ris::ByteContent* aRxMsg);

   //--------------------------------------------------------------
   // Callbacks:

   // This is a callback that is called when a session is
   // established or disestablished.
   // It uses a CallPointer for a function with signature:
   // void function(bool aConnected);

   SessionNotifyCallPointer mSessionCallback;

   // This is a callback that is called when a message is received
   MsgCallPointer mRxCallback;
};

//******************************************************************************
}//namespace
}//namespace
#endif

