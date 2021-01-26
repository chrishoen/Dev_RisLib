/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someThreadParms.h"

#define  _SOMETESTTHREAD_CPP_
#include "someTestThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TestThread::TestThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Test");
   BaseClass::setThreadPrintLevel(0);
   BaseClass::setThreadPriorityHigh();

   mTimerPeriod = 1000;
   mTimerCount = 0;
   mSemCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running.

void TestThread::threadInitFunction()
{
   mWaitable.initialize(mTimerPeriod);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function.

void TestThread::threadRunFunction()
{
   // Loop to wait for posted events and process them.
   while (true)
   {
      // Wait for the timer or the semaphore.
      mWaitable.waitForTimerOrSemaphore();
      // Test for thread termination.
      if (BaseClass::mTerminateFlag) break;

      // Call a handler for the timer or the semaphore.
      if (mWaitable.wasTimer())
      {
         Prn::print(Prn::View21, "mWaitable.wasTimer %4d  %4d", mTimerCount++, mSemCount);
      }

      if (mWaitable.wasSemaphore())
      {
         mSemCount++;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated.

void TestThread::threadExitFunction()
{
   mWaitable.finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread shutdown function. Set the termination flag, post to the 
// waitable semaphore and wait for the thread to terminate.

void TestThread::shutdownThread()
{
   BaseClass::shutdownThreadPrologue();

   BaseClass::mTerminateFlag = true;
   mWaitable.postSemaphore();
   BaseClass::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace