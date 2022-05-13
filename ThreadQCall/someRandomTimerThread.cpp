/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "someThreadParms.h"
#include "someTestQCallThread.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someRandomTimerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

RandomTimerThread::RandomTimerThread(int aIdent)
   : mIdent(aIdent),
     mRandomGen(mRandomDevice()),
     mRandomDis(gThreadParms.mDelayA1, gThreadParms.mDelayA2)
{
   mTPFlag = false;
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
// after the thread init function. It runs a loop that waits for a 
// semi-randomly time and then sends a qcall to the test thread.

void RandomTimerThread::threadRunFunction()
{
   Prn::print(Prn::ThreadRun1, "RandomTimerThread::threadRunFunction BEGIN");

   // Loop until thread termination.
   int tCount = 0;
   while (true)
   {
      // Test for thread termination.
      if (BaseClass::mTerminateFlag) break;

      // Wait for a random delay.
      int tDelay = mRandomDis(mRandomGen);
      BaseClass::threadSleep(tDelay);

      // Send a qcall to the test thread.
      gTestQCallThread->mTest1QCall(mIdent,tCount);
      tCount++;
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