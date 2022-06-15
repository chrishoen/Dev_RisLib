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

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TimerThread::TimerThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Timer");
   
   // Set base class variables.
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mTimerTest);

   // Set timer period.
   BaseClass::mTimerPeriod = 250;
   BaseClass::mTimerPeriod = 1000;
   mTimeMarker.initialize(100, 250*1000);

   // Members
   mTPFlag = false;
   mTestCode = 2;
   mTestCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::threadInitFunction()
{
   Prn::print(0, "TimerThread::threadInitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeOnTimer(int aTimeCount)
{
   Prn::print(0, "TEST2 %5d $$ %3d %10.4f",
      aTimeCount,
      BaseClass::getThreadProcessorNumber(),
      Ris::getProgramTime());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest1(int aTimeCount)
{
   mTimeMarker.doUpdate();

   if (mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(0, "TEST %5d %1d %5d $$ %10.3f %10.3f %10.3f %10.3f $$ %10.3f",
         mTestCount++,
         BaseClass::getThreadProcessorNumber(),
         mTimeMarker.mStatistics.mSize,
         mTimeMarker.mStatistics.mMean,
         mTimeMarker.mStatistics.mStdDev,
         mTimeMarker.mStatistics.mMinX,
         mTimeMarker.mStatistics.mMaxX,
         mTimeMarker.mStatistics.mMaxError);
      mTimeMarker.mStatistics.mEndOfPeriod = false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest2(int aTimeCount)
{
   Prn::print(Prn::ThreadRun1, "TEST2 %5d $$ %3d %10.4f",
      aTimeCount,
      BaseClass::getThreadProcessorNumber(),
      Ris::getProgramTime());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace