#pragma once

/*==============================================================================
Udp message thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risNetUdpMsgSocket.h"
#include "risNetSettings.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp message thread.
//
// This is a thread that provides the execution context for a udp peer that
// that communicates messages with another udp peer.
//
// The messages follow the byte content message scheme.
// 
// It contains a udp socket that is bound to a local address and that
// sends to a remote address.
//
// The data that is communicated via the sockets is encapsulated by messages.
//
// The thread is structured around a while loop that does a recvfrom
// call to receive a message on the socket.
//
// The thread provides serialized access to the socket and associated 
// state variables and it provides the context for the blocking of the 
// recv call.
//
// An instance of this thread is created as a child thread of a parent thread
// that performs message processing. The parent creates the child and
// registers a receive message qcall callback to it. When the child thread
// receives a message it invokes the message qcall to pass it to the parent
// for processing. The parent thread can send messages to the udp peer
// via the class send calls.

class UdpMsgThread : public Ris::Threads::BaseThread
{
private:
   typedef Ris::Threads::BaseThread BaseClass;
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings mSettings;

   // Socket instance.
   UdpMsgSocket mMsgSocket;

   // This is a qcall callback that is invoked when a message is received.
   // It is registered by the parent thread at initialzation via the settings.
   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;
   RxMsgQCall mRxMsgQCall;

   // Program trace index.
   int mTI;

   // Metrics.
   int mErrorCount;
   int mRestartCount;
   int mRxCount;
   int mTxCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   UdpMsgThread(Settings& aSettings);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, thread base class overloads:

   // Setup the socket.
   void threadInitFunction()override;

   // Thread run function. This is called by the base class for the main 
   // thread processing. It executes a while loop that does receive calls.
   // When a message is received, it is passed to the parent via the receive
   // qcall. The loop exits when the thread is shutdown.
   void threadRunFunction()override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated.
   void threadExitFunction()override;

   // Thread shutdown function. This is called in the context of the parent
   // thread. It sets the termination flag, closes the socket, and waits
   // for the thread to terminate.
   void shutdownThread()override; 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Pass a received message to the parent thread. This is called by the
   // thread run function when a message is received. It invokes the
   // receive qcall that is registered at initialization.
   virtual void processRxMsg(Ris::ByteContent* aMsg);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a transmit message through the socket to the peer. It executes a
   // send call in the context of the calling thread. It deletes the message
   // when done.
   void sendMsg(Ris::ByteContent* aMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

