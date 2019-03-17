#pragma once

/*==============================================================================
Prototype udp string thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <string>
#include "risNetUdpStringThread.h"
#include "risThreadsQCallThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a udp string thread that transmits and receives byte content
// strings via a udp string child thread which manages a udp string socket.
//
// It provides the capability to send strings via the child thread socket and
// it provides handlers for strings received via the child thread socket.
// When the child thread receives a string it invokes a qcall that was
// registered by this thread to defer execution of a string handler that is 
// a member of this thread.
//   
// It inherits from BaseQCallThread to obtain a call queue based thread
// functionality.

class  NetworkThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Udp string socket child thread. It provides the thread execution
   // context for a udp string socket and uses it to provide string
   // communication. It interfaces to this thread via the receive string
   // qcall callback.
   Ris::Net::UdpStringThread* mUdpStringThread;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Control variables.
   bool mTPFlag;

   // Metrics.
   int  mStatusCount1;
   int  mStatusCount2;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   NetworkThread();
  ~NetworkThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // after the thread starts running. It starts the child thread.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated. It shuts down the child thread.
   void threadExitFunction() override;

   // Execute periodically. This is called by the base class timer. It
   // sends an echo request string.
   void executeOnTimer(int aTimerCount) override;
  
   // Show thread state info.
   void showThreadInfo() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Receive string qcall.

   // qcall registered to the mUdpStringThread child thread. It is invoked when
   // a string is received. It process the received strings.
   Ris::Net::UdpStringThread::RxStringQCall mRxStringQCall;

   // Print the string. This is bound to the qcall.
   void executeRxString (std::string* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a string via the child thread.
   void sendString (std::string* aString);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _PROCONETWORKTHREAD_CPP_
         NetworkThread* gNetworkThread;
#else
extern   NetworkThread* gNetworkThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
