/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include <time.h>

#include "risProgramTime.h"
#include "risThreadsPriorities.h"

#include "somePeriodicParms.h"

#define  _SOMETIMERTESTTHREAD_CPP_
#include "someTimerTestThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TimerTestThread::TimerTestThread()
{
   // Set base class variables.
   BaseClass::setThreadName("TimerTest");
   BaseClass::setThreadPrintLevel(0);
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mTimerTest);

   // Set timer period.
   BaseClass::mTimerPeriod = gPeriodicParms.mTestThreadPeriod;
   mTimeMarker.initialize(gPeriodicParms.mSampleSize, gPeriodicParms.mTestThreadPeriod*1000);

   // Set member variables.
   mTestCode = 1;
   mTestCount = 0;

   mUpdateFlag = false;
   mProcessorNumber = 0;
   mMean = 0;
   mStdDev = 0;
   mMin = 0;
   mMax = 0;
   mMaxError = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerTestThread::executeOnTimer(int aTimeCount)
{
   if (aTimeCount == 0)
   {
      BaseClass::showThreadFullInfo();
   }

   if (aTimeCount < 20) return;

   mTimeMarker.doUpdate();

   if (mTimeMarker.mStatistics.mEndOfPeriod)
   {
      mTestCount++;
      //mProcessorNumber = BaseClass::getThreadProcessorNumber(),
      mMean = mTimeMarker.mStatistics.mMean;
      mStdDev = mTimeMarker.mStatistics.mStdDev;
      mMin = mTimeMarker.mStatistics.mMinX;
      mMax = mTimeMarker.mStatistics.mMaxX;
      mMaxError = mTimeMarker.mStatistics.mMaxError;
      mTimeMarker.mStatistics.mEndOfPeriod = false;
      mUpdateFlag = true;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace