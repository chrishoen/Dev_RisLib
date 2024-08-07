#pragma once

/*==============================================================================
Test qcall thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a test qcall thread.
//   
// It inherits from BaseQCallThread to obtain a call queue based thread
// functionality.

class  TestQCallThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Control variables.
   bool mTPFlag;

   // Metrics.
   int  mStatusCount1;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   TestQCallThread();
  ~TestQCallThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // after the thread starts running.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immediately
   // before the thread is terminated.
   void threadExitFunction() override;

   // Execute periodically. This is called by the base class timer.
   void executeOnTimer(int aTimerCount) override;
  
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.qcalls.

   // Test qcall. It is invoked by the timer thread.
   Ris::Threads::QCall2<bool,int> mTest1QCall;

   // This is bound to the qcall. It executes a test function.
   void executeTest1 (bool aFlag,int aCode);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global singular instance.

#ifdef _TESTQCALLTHREAD_CPP_
         TestQCallThread* gTestQCallThread;
#else
extern   TestQCallThread* gTestQCallThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
