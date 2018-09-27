#pragma once

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

#include "risThreadsQCallThread.h"
#include "risNetSettings.h"
#include "risNetTcpMsgSocket.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
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
//

class TcpMsgClientThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.
   
   // Settings.
   Settings mSettings;

   // Socket instance.
   TcpMsgSocket mSocket;

   // This is a qcall that is called when a session is established or
   // disestablished.
   typedef Ris::Threads::QCall1<bool> SessionQCall;
   SessionQCall mSessionQCall;

   // This is a qcall that is called when a message is received.
   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;
   RxMsgQCall   mRxMsgQCall;

   // If this flag is true then a connection has been established with the 
   // server and sendMsg can be called.
   bool mConnectionFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   
   // Constructor.
   TcpMsgClientThread(Settings& aSettings);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, thread base class overloads:

   // Setup the socket.
   void threadInitFunction()override;

   // Execute a while loop that does connect and recv calls. The loop exits
   // when the socket is closed and the termination flag is true.
   void threadRunFunction()override;

   // Print.
   void threadExitFunction()override;

   // Set the termination flag, close the socket and wait for the thread to
   // terminate.
   void shutdownThread()override; 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   
   // Notify the thread owner of this thread that a session has changed. This
   // is  called by the threadRunFunction  when a new session is established 
   // or an existing session is disestablished. It invokes the mSessionQCall
   // that is passed in at configure.
   virtual void processSessionChange  (bool aEstablished);

   // Pass a received message to the thread owner. It invokes the mRxMsgQCall
   // that is passed in at configure. This is called by the threadRunFunction
   // to process a received message.
   virtual void processRxMsg (Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   
   // Send a transmit message through the socket to the server. It executes a
   // blocking send() call in the context of the caller. It is protected by a
   // mutex semaphore.
   void sendMsg(ByteContent* aTxMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

