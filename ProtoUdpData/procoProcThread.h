#pragma once

/*==============================================================================
Udp string prototype thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risNetUdpStringThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp string prototype thread class. It processes strings that are
// communicated via a socket channel. The strings follow the byte content
// binary string scheme.
//
// This is a prototype socket thread class. It inherits from BaseQCallThread to
// obtain a call queue based thread functionality.
//
// The prototype thread creates a child socket string thread that establishes
// and manages a socket connection, receives strings and passes them to the
// parent via a qcall callback, and allows for the transmission of strings.
// the child thread also notifies the parent thread of socket connection
// establishment/disestablishment via a qcall callback.
// 
// The prototype thread is based on a call queue and it uses qcalls to
// interface to the child thread. When the child thread detects a session
// change it invokes the prototypes thread's mSessionQCall, which defers
// execution of its executeSession member function. Likewise, when the child
// thread receives a string it invokes the socket thread's mRxStringQCall, which
// defers  execution of its executeRxString member function. 
//
// The child thread provides the execution context for actually managing
// session changes and receiving strings. The parent thread provides the
// execution context for processing the session changes and the received 
// strings.
//

class  ProcThread : public Ris::Threads::BaseQCallThread
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Udp string thread, this manages socket string connections and
   // string transmission and reception.
   Ris::Net::UdpStringThread*  mUdpStringThread;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

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
   // child UdpStringThread.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   // before the thread is terminated. It shuts down the child UdpStringThread.
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
   // Methods. Receive string qcall.

   // qcall registered to the mUdpStringThread child thread. It is invoked by
   // the child thread when a string is received.
   Ris::Net::UdpStringThread::RxStringQCall mRxStringQCall;

   // Print the string. This is bound to the qcall.
   void executeRxString(std::string* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a string via the child thread.
   void sendString(std::string* aString);
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
