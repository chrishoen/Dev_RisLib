#pragma once

/*==============================================================================
UDP port responder thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risNetUdpStringSocket.h"
#include "risNetSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread listens on a udp port and sends a response. It is
// used to respond to simple network discovery requests.

class PortResponderThread : public Ris::Threads::BaseThread
{
public:
   typedef Ris::Threads::BaseThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Udp port settings.
   Ris::Net::Settings mSettings;

   // Udp port.
   Ris::Net::UdpStringSocket mUdpStringSocket;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   PortResponderThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Base class overloads.

   // Thread init function. This is called by the base class immediately
   // after the thread starts running. It initializes the udp port.
   void threadInitFunction() override;

   // Thread run function. This is called by the base class immediately
   // after the thread init function. It runs a loop that blocks on 
   // udp port receives and then processes them. The loop terminates
   // when the udp port receive is aborted.
   void threadRunFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated. It is close the udp port.
   void threadExitFunction() override;

   // Thread shutdown function. It is called out of the context of
   // this thread. It aborts the udp port receive and waits for the
   // thread to terminate after execution of the thread exit function.
   void shutdownThread() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send bytes via the udp port. This executes in the context of
   // the calling thread.
   void sendString(char* aString);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _SOMEPORTRESPONDERTHREAD_CPP_
PortResponderThread* gPortResponderThread = 0;
#else
extern  PortResponderThread* gPortResponderThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

