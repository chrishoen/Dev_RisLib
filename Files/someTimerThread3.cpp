/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "prnPrint.h"
#include "someShare.h"
#include "GSettings.h"

#define  _SOMETIMERTHREAD3_CPP_
#include "someTimerThread3.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TimerThread3::TimerThread3() 
{
   // Set thread priority
   BaseClass::setThreadPriorityHigh();
   BaseClass::mThreadAffinityMask = 0x20;
   BaseClass::mThreadIdealProcessor = 5;

   // Set timer period
   int mTimerFrequency = 20;
   mTimerPeriod = 1000 / mTimerFrequency;

   gShare.mTimeMarker.initialize(5 * mTimerFrequency);

   // Members
   mTerminateFlag = false;
}

//******************************************************************************

void TimerThread3::threadInitFunction()
{
   // Guard
   if (mTimerPeriod <= 0) return;

   // Bind timer callback
   mThreadTimerCall.bind (this,&TimerThread3::executeOnTimer);

   // Start timer
   mThreadTimer.startTimer(mThreadTimerCall,mTimerPeriod);
}

//******************************************************************************

void TimerThread3::threadRunFunction()
{
   while(1)
   {
      // Wait for semaphore to timeout
      mSemaphore.get(0xFFFFFFFF);
      // Test for termination
      if (mTerminateFlag) break;
      // Execute inheritor timer function
      executeOnTimer(mTimerCount++);
   }
}

//******************************************************************************

void TimerThread3::threadExitFunction()
{
   mThreadTimer.cancel();
}

//******************************************************************************

void TimerThread3::shutdownThread()
{
   // Set terminate
   mTerminateFlag = true;
   mSemaphore.put();
   // Wait for terminate
   waitForThreadTerminate();
}   

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread3::executeOnTimer(int aTimeCount)
{
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

