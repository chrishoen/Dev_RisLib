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
#include "tsThreadServices.h"

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
   BaseClass::setThreadPrintLevel(3);

   // Set base class variables.
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mTimerTest);

   // Set timer period.
   BaseClass::mTimerPeriod = 250;
   mTimeMarker.initialize(100);

   // Members
   mTPFlag = false;
   mTestCode = 1;
   mTestCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::threadInitFunction()
{
   if (mTestCode !=3) return;

   BaseClass::showThreadFullInfo();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeOnTimer(int aTimeCount)
{
   if (aTimeCount < 20) return;

   switch (mTestCode)
   {
      case 1: executeTest1(aTimeCount); break;
      case 2: executeTest2(aTimeCount); break;
      case 3: executeTest3(aTimeCount); break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* get_timespec_asString(timespec aTimeSpec, char* aBuffer)
{
   char tTemp[40];
   strftime(tTemp, 40, "%F %T", localtime(&aTimeSpec.tv_sec));
// sprintf(aBuffer, "%s.%03ld", tTemp, aTimeSpec.tv_nsec / 1000000);
   strftime(aBuffer, 40, "%F %T", localtime(&aTimeSpec.tv_sec));

   return aBuffer;
}

void TimerThread::executeTest1(int aTimeCount)
{
   timespec tTOA;
   timespec_get(&tTOA, TIME_UTC);
   char tBuffer[100];
   Prn::print(Prn::View11, "TIMER %5d $$ %s",
      aTimeCount,
      get_timespec_asString(tTOA, tBuffer));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest2(int aTimeCount)
{
   mTimeMarker.doUpdate();

   if (mTimeMarker.mStatistics.mEndOfPeriod)
   {
//    TS::print(0, "CURRENT %10.4f", Ris::getCurrentProgramTime());
      TS::print(0, "TEST %5d %1d %5d $$ %10.3f %10.3f %10.3f %10.3f $$ %10.3f",
         mTestCount++,
         BaseClass::getThreadProcessorNumber(),
         mTimeMarker.mStatistics.mSize,
         mTimeMarker.mStatistics.mMean,
         mTimeMarker.mStatistics.mStdDev,
         mTimeMarker.mStatistics.mMinX,
         mTimeMarker.mStatistics.mMaxX,
         mTimeMarker.mStatistics.mMaxX - mTimeMarker.mStatistics.mMinX);
      mTimeMarker.mStatistics.mEndOfPeriod = false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest3(int aTimeCount)
{
   Prn::print(Prn::ThreadRun1, "TEST2 %5d $$ %3d %10.4f",
      aTimeCount,
      BaseClass::getThreadProcessorNumber(),
      Ris::getCurrentProgramTime());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace