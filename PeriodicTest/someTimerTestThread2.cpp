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

#define  _SOMETIMERTESTTHREAD2_CPP_
#include "someTimerTestThread2.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TimerTestThread2::TimerTestThread2()
{
   // Set base class variables.
   BaseClass::setThreadName("TimerTest");
   BaseClass::setThreadPrintLevel(0);
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mTimerTest);
   BaseClass::setThreadPriority(
      Ris::Threads::Priority(
         gPeriodicParms.mTestThreadProcessor,
         gPeriodicParms.mTestThreadPriority));

   // Set deadline parameters.
   BaseClass::mThreadEDFRunTimeUs = gPeriodicParms.mEDFRunTimeUs;
   BaseClass::mThreadEDFDeadlineUs = gPeriodicParms.mEDFDeadlineUs;
   BaseClass::mThreadEDFPeriodUs = gPeriodicParms.mEDFPeriodUs;

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

void TimerTestThread2::executeOnTimer(int aTimeCount)
{
   mTestCount = aTimeCount;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace