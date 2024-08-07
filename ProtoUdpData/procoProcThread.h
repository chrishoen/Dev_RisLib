#pragma once

/*==============================================================================
Udp data prototype thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risNetUdpDataThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp data prototype thread class. It processes datas that are
// communicated via a socket channel. The datas follow the byte content
// binary data scheme.
//
// This is a prototype socket thread class. It inherits from BaseQCallThread to
// obtain a call queue based thread functionality.
//
// The prototype thread creates a child socket data thread that establishes
// and manages a socket connection, receives datas and passes them to the
// parent via a qcall callback, and allows for the transmission of datas.
// the child thread also notifies the parent thread of socket connection
// establishment/disestablishment via a qcall callback.
// 
// The prototype thread is based on a call queue and it uses qcalls to
// interface to the child thread. When the child thread detects a session
// change it invokes the prototypes thread's mSessionQCall, which defers
// execution of its executeSession member function. Likewise, when the child
// thread receives a data it invokes the socket thread's mRxDataQCall, which
// defers  execution of its executeRxData member function. 
//
// The child thread provides the execution context for actually managing
// session changes and receiving datas. The parent thread provides the
// execution context for processing the session changes and the received 
// datas.
//

class  ProcThread : public Ris::Threads::BaseQCallThread
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Udp data thread, this manages socket data connections and
   // data transmission and reception.
   Ris::Net::UdpDataThread*  mUdpDataThread;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Maximum send and receive data size.
   int mMaxDataSize;

   // True if the socket connection is valid.
   bool mConnectionFlag;

   // State variables.
   bool mTPFlag;

   // Metrics.
   int mTxCount;
   int mRxCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   typedef Ris::Threads::BaseQCallThread BaseClass;

   // Constructor.
   ProcThread();
  ~ProcThread();
  void show();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread base class overloads.

   // Thread init function. This is called by the base class immedidately 
   // after the thread starts running. It creates and launches the 
   // child UdpDataThread.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   // before the thread is terminated. It shuts down the child UdpDataThread.
   void threadExitFunction() override;

   // Thread shutdown function. This calls the base class shutdownThread
   // function to terminate the thread. This executes in the context of
   // the calling thread.
   void shutdownThread() override;

   // Execute periodically. This is called by the base class timer.
   void executeOnTimer(int aTimerCount) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Receive data qcall.

   // qcall registered to the mUdpDataThread child thread. It is invoked by
   // the child thread when a data is received.
   Ris::Net::UdpDataThread::RxDataQCall mRxDataQCall;

   // Print the data. This is bound to the qcall.
   void executeRxData(char* aData, int aSize);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a data via the child thread.
   void sendData(const char* aData, int aSize);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _PROCOPROCTHREAD_CPP_
         ProcThread* gProcThread;
#else
extern   ProcThread* gProcThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
