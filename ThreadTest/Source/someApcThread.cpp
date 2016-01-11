/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h>
#include "prnPrint.h"
#include "someShare.h"
#include "GSettings.h"

#define  _SOMEAPCTHREAD_CPP_
#include "someApcThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

ApcThread::ApcThread()
{
   // Set base class priority
   BaseClass::setThreadPriorityHigh();
   BaseClass::mThreadAffinityMask = 0x20;
   BaseClass::mThreadIdealProcessor = 5;

   // Set timer period
   mTimerPeriod = 0;
   if (gGSettings.mTestNumber==101 && gGSettings.mTimerThread == GSettings::cTimerThread_None)
   {
      int mTimerFrequency = 20;
      mTimerPeriod = 1000 / mTimerFrequency;
      gShare.mTimeMarker.initialize(5 * mTimerFrequency);
   }

   // Members
   mTerminateFlag = false;
}

//******************************************************************************

void ApcThread::threadInitFunction()
{
   // Guard
   if (mTimerPeriod==0) return;

   // Call pointer
   Ris::Threads::TimerCall tTimerCall;
   tTimerCall.bind(this,&ApcThread::executeOnTimer);

   // Create timer
   mApcTimer.create(mTimerPeriod,tTimerCall);
}

//******************************************************************************
// Thread run function, base class overload.
// This provides the execution context for processing queued QCalls
// It waits for the call queue semaphore, extracts a call from
// the call queue, and executes the call.
//
// When a QCall is written to the call queue, this wakes up, reads it from the 
// queue, and executes it. It returns true if successful, it returns false if
// mTerminateFlag was true.

void ApcThread::threadRunFunction()
{
   // Wait forever on the alertable termination semaphore
   while (true)
   {
      mTerminateSem.get(INFINITE);
      if (mTerminateFlag) break;
   }
}

//******************************************************************************
// Thread exit function, base class overload.

void ApcThread::threadExitFunction()
{
   if (mTimerPeriod==0) return;
   mApcTimer.cancel();
}

//******************************************************************************
// shutdown

void ApcThread::shutdownThread()
{
   // Set termination flag
   mTerminateFlag = true;
   mTerminateSem.put();
   // Wait for thread terminate
   waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This executes the qcall that was passed via APC by the following function.

static VOID CALLBACK Some_APCProc(_In_ ULONG_PTR dwParam)
{
   ApcThread* tThread = (ApcThread*)dwParam;
   tThread->executeOnApc();
}

//******************************************************************************
// This sends an APC to this thread, it contains a pointer to the above 
// function and a pointer to a qcall.

// This enqueues an APC to the threads APC queue

void ApcThread::enqueueApc()
{
   QueueUserAPC(
      Some_APCProc,
      *(HANDLE*)BaseClass::getHandlePtr(),
      (ULONG_PTR)this);
}

//******************************************************************************

void ApcThread::executeOnApc()
{
   gShare.mTimeMarker.doStop();
}

//******************************************************************************

void ApcThread::executeOnTimer(int aTimeCount)
{
   // Guard
   if(mTerminateFlag) return;

   //---------------------------------------------------------------------------
   // Time marker

   gShare.mTimeMarker.doStop();
   gShare.mTimeMarker.doStart();

   if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
   {
      double tPeriodUS = mTimerPeriod*1000.0;
      Prn::print(Prn::ThreadRun1, "TEST %1d %1d %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         gGSettings.mTestThread,
         gGSettings.mTestNumber,
         gShare.mTimeMarker.mStatistics.mSize,
         gShare.mTimeMarker.mStatistics.mMean   - tPeriodUS,
         gShare.mTimeMarker.mStatistics.mStdDev,
         gShare.mTimeMarker.mStatistics.mMinX   - tPeriodUS,
         gShare.mTimeMarker.mStatistics.mMaxX   - tPeriodUS);
         gShare.mTimeMarker.mStatistics.mEndOfPeriod = false;
         return;
   }
}
}//namespace
