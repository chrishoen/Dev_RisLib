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
// Udp string thread class. It processes strings that are communicated
// via udp datagrams.
//
// It inherits from BaseQCallThread to obtain a call queue based thread
// functionality.
//
// The thread creates a child udp string thread that creates and manages
// a udp socket, receives strings and passes them to this thread via a qcall
// callback, and provides for the transmission of strings.
// 
// This thread is based on a call queue and it uses qcalls to interface to
// the child thread. When the child thread receives a string it invokes this
// threads receive string qcall, which defers execution of its receive
// string member function, which then processes the string. 
//
// The child thread provides the execution context for managing the socket
// and receiving strings on it. This thread provides the execution
// context for processing the received strings and for sending messags.
//

class  ProcThread : public Ris::Threads::BaseQCallThread
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Udp string thread, this manages socket string transmission and reception.
   Ris::Net::UdpStringThread*  mUdpStringThread;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

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
   // child string thread.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   // before the thread is terminated. It shuts down the child string
   // thread.
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

   // qcall registered to the child string thread. It is invoked by
   // the child thread when a string is received.
   Ris::Net::UdpStringThread::RxStringQCall mRxStringQCall;

   // Print the string. This is bound to the qcall.
   void executeRxString(std::string* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a string via the child string thread.
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
