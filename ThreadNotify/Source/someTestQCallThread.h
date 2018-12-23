#pragma once

/*==============================================================================
Test qcall thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsQCallThread.h"
#include "risThreadsNotify.h"

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

   Ris::Threads::Notify mNotify;

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

   // Test qcall. 
   Ris::Threads::QCall1<int> mTest1QCall;
   Ris::Threads::QCall1<int> mTest2QCall;
   Ris::Threads::QCall1<int> mTest3QCall;
   Ris::Threads::QCall1<int> mTest4QCall;

   // Test function. This is bound to the qcall.
   void executeTest1(int aTimeout);
   void executeTest2(int aTimeout);
   void executeTest3(int aTimeout);
   void executeTest4(int aTimeout);
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
