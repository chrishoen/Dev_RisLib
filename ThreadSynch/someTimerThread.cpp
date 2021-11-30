/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"


#include "someShare.h"
#include "Experiment.h"
#include "someThread1.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

TimerThread::TimerThread()
{
   // Set base class thread priority

   // Set timer period
   int tTimerFrequency = 20;
   int tWindowSize = 100;
   BaseClass::mTimerPeriod = 1000 / tTimerFrequency;

   gShare.mTimeMarker.initialize(tWindowSize, 5*tTimerFrequency);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void TimerThread::executeOnTimer(int aTimeCount)
{
   if (aTimeCount < mTimerFrequency) return;

   if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun1, "TEST%d %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         gShare.mTest,
         gShare.mTimeMarker.mStatistics.mSize,
         gShare.mTimeMarker.mStatistics.mMean,
         gShare.mTimeMarker.mStatistics.mStdDev,
         gShare.mTimeMarker.mStatistics.mMinX,
         gShare.mTimeMarker.mStatistics.mMaxX);
         gShare.mTimeMarker.mStatistics.mEndOfPeriod = false;
         return;
   }

   switch (gShare.mTest)
   {
   case 1:
      gThread1->mThreadSem.put();
      break;
   case 3:
      Experiment::send();
      break;
   }
}

}//namespace