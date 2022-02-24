#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsTwoThread.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an example master thread that sends commands to a slave thread
// and waits for responses.

class TestThread : public Ris::Threads::TwoThreadLongThread
{
public:
   typedef Ris::Threads::TwoThreadLongThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   TestThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // before the thread starts running.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immediately 
   // after the thread starts running.
   void threadExitFunction() override;

   // Execute periodically. This is called by the base class timer.
   void executeOnTimer(int aTimerCount) override;


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.qcalls.

   // Test qcall. It is invoked by the timer thread.
   Ris::Threads::QCall0 mTest0QCall;

   // Test function. This is bound to the qcall.
   void executeTest0();

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _SOMETESTTHREAD_CPP_
          TestThread* gTestThread;
#else
   extern TestThread* gTestThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
