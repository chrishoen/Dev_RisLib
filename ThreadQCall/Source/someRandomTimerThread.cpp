/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someRandomTimerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

RandomTimerThread::RandomTimerThread()
{
   mTPFlag = false;
   mDelayA1 = 100;
   mDelayA2 = 200;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function. This is called by the base class immediately 
// after the thread starts running. It initializes SDL and creates the
// thread SDL window and associated resources.

void RandomTimerThread::threadInitFunction()
{
   Prn::print(Prn::ThreadInit1, "RandomTimerThread::threadInitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that waits on SDL
// events and processes posted events. The loop exits when it receives
// a quit event.

void RandomTimerThread::threadRunFunction()
{
   Prn::print(Prn::ThreadRun1, "RandomTimerThread::threadRunFunction BEGIN");

   // Loop to wait for posted events and process them.
   while (true)
   {
      // Test for thread termination.
      if (BaseClass::mTerminateFlag) break;
      // Wait.
      BaseClass::threadSleep(1000);
   }

   Prn::print(Prn::ThreadRun1, "RandomTimerThread::threadRunFunction END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function. This is called by the base class immediately
// before the thread is terminated. It releases SDL resources and closes
// the thread SDL window.

void RandomTimerThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "RandomTimerThread::threadExitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace