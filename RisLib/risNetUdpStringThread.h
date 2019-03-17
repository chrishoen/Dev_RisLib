#pragma once

/*==============================================================================
Udp string thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <string>

#include "risThreadsQCallThread.h"
#include "risNetUdpStringSocket.h"
#include "risNetSettings.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp string thread.
//
// This is a thread that provides the execution context for a udp peer that
// that communicates strings with another udp peer.
//
// It contains a receive  udp socket that is bound to a local address.
// It contains a transmit udp socket that is bound to a remote address.
//
// The data that is communicated via the sockets is encapsulated by strings.
//
// The thread is structured around a while loop that does a recvfrom
// call to receive a string on the socket.
//
// The thread provides serialized access to the socket and associated 
// state variables and it provides the context for the blocking of the 
// recv call.
//
// An instance of this thread is created as a child thread of a parent thread
// that performs string processing. The parent creates the child and
// registers a receive string qcall callback to it. When the child thread
// receives a string it invokes the string qcall to pass it to the parent
// for processing.

class UdpStringThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings mSettings;

   // Socket instance.
   UdpRxStringSocket mRxSocket;
   UdpTxStringSocket mTxSocket;

   // This is a qcall callback that is invoked when a string is received.
   // It is registered by the parent thread at initialzation.
   typedef Ris::Threads::QCall1<std::string*> RxStringQCall;
   RxStringQCall mRxStringQCall;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   UdpStringThread(Settings& aSettings);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, thread base class overloads:

   // Thread init function. This is called by the base class immediately 
   // after the thread starts running. It configures the sockets.
   void threadInitFunction() override;

   // Thread run function. This is called by the base class for the main 
   // thread processing. Execute a while loop that does read calls.
   // The loop exits when the thread is canceled.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated.
   void threadExitFunction() override;

   // Thread shutdown function. This is called in the context of the parent
   // thread. Set the termination flag, close the socket and wait for the
   // thread to terminate.
   void shutdownThread() override; 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Pass a received string to the parent thread. This is called by the
   // threadRunFunction when a string is received. It invokes the
   // mRxStringQCall that is registered at initialization.
   virtual void processRxString(std::string* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a transmit string through the socket to the peer. It executes a
   // blocking send call in the context of the calling thread. Delete the
   // string when done.
   void sendString(std::string* aString);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

