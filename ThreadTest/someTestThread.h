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
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread that executes short term qcalls. That is, execution of its
// thread qcalls is expected to take a short amount of time.

class TestParentThread : public Ris::Threads::BaseQCallThread
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Thread call pointers, these are called by the thread base overloads, if 
   // they are bound. They are bound to functions by the instantiator before
   // the thread is launched. Any that are not bound result in a no op for the
   // thread run function.
   std::function<void(void)>   mThreadInitCallPointer;
   std::function<void(void)>   mThreadExitCallPointer;
   std::function<void(int)>    mThreadExecuteOnTimerCallPointer;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Constructor.
   TestParentThread();
   ~TestParentThread();

   // Thread init function. This is called by the base class immedidately 
   // after the thread starts running.
   // It calls the associated call pointer, if it exists.
   void threadInitFunction() override;

   // Thread exit function. This is called by the base class immedidately
   //  before the thread is terminated.
   // It calls the associated call pointer, if it exists.
   void threadExitFunction() override;

   // Execute periodically. This is called by the base class timer.
   // It calls the associated call pointer, if it exists.
   void executeOnTimer(int aTimerCount) override;
};
 
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an example master thread that sends commands to a slave thread
// and waits for responses.

class TestChildThread : public TestParentThread
{
public:
   typedef TestParentThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   TestChildThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Thread base class overloads.

   // Thread init function. This is called by the base class immediately 
   // before the thread starts running.
   //void threadInitFunction() override;

   // Thread exit function. This is called by the base class immediately 
   // after the thread starts running.
   //void threadExitFunction() override;

   // Execute periodically. This is called by the base class timer.
   //void executeOnTimer(int aTimerCount) override;


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
          TestChildThread* gTestChildThread;
#else
   extern TestChildThread* gTestChildThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
