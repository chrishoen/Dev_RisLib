#pragma once

/*==============================================================================
Udp data thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risNetUdpDataSocket.h"
#include "risNetSettings.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp data thread.
//
// This is a thread that provides the execution context for a udp peer that
// that communicates datas with another udp peer.
// 
// The data is raw binary data from a given pointer and length.
//
// It contains a udp socket that is bound to a local address and that
// sends to a remote address.
//
// The data that is communicated via the sockets is encapsulated by datas.
//
// The thread is structured around a while loop that does a recvfrom
// call to receive data on the socket.
//
// The thread provides serialized access to the socket and associated 
// state variables and it provides the context for the blocking of the 
// recv call.
//
// An instance of this thread is created as a child thread of a parent thread
// that performs data processing. The parent creates the child and
// registers a receive data qcall callback to it. When the child thread
// receives data it invokes the data qcall to pass it to the parent
// for processing. The parent thread can send datas to the udp peer
// via the class send calls.

class UdpDataThread : public Ris::Threads::BaseThread
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
   UdpDataSocket mDataSocket;

   // This is a qcall callback that is invoked when a data is received.
   // It is registered by the parent thread at initialzation via the settings.
   typedef Ris::Threads::QCall2<char*,int> RxDataQCall;
   RxDataQCall mRxDataQCall;

   // Receive data maximum receive data size.
   int mMaxDataSize;

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
   UdpDataThread(Settings& aSettings);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, thread base class overloads:

   // Thread init function. This is called by the base class immediately 
   // after the thread starts running. It initializes the socket.
   void threadInitFunction() override;

   // Thread run function. This is called by the base class for the main 
   // thread processing. It executes a while loop that does receive calls.
   // When data is received, it is passed to the parent via the receive
   // qcall. The loop exits when the thread is shutdown.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated.
   void threadExitFunction() override;

   // Thread shutdown function. This is called in the context of the parent
   // thread. It sets the termination flag, closes the socket, and waits
   // for the thread to terminate.
   void shutdownThread() override; 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Pass a received data to the parent thread. This is called by the
   // thread run function when data is received. It invokes the
   // receive qcall that is registered at initialization.
   virtual void processRxData(char* aData, int aSize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a transmit data through the socket to the peer. It executes a
   // send call in the context of the calling thread.
   void sendData(const char* aData, int aSize);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

