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

class TestThread : public Ris::Threads::BaseQCallThread
{
private:
   typedef Ris::Threads::BaseQCallThread BaseClass;
public:

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
   ~TestThread();

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
// Global instance.

#ifdef _SOMETESTTHREAD_CPP_
       TestThread* gTestThread = 0;
#else
       extern TestThread* gTestThread;
#endif

       
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
