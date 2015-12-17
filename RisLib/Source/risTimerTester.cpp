/*==============================================================================
==============================================================================*/
#include <stdio.h>

#include "prnPrint.h"
#include "risTimerTester.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
TimerTester::TimerTester()
{
	reset();
}
//******************************************************************************
void TimerTester::reset (void)
{
   mTimerPeriod     = 1000;
   mEnableFlag      = false;
   mIterationCount  = 0;
   mExecutionCount  = 0;
   mNumOfIterations = 0;
   mNumOfExecutions = 0;
}
//******************************************************************************
void TimerTester::startTest(int aTimerPeriod,int aNumOfIterations,int aNumOfExecutions)
{
   mTimerPeriod = aTimerPeriod;

   mEnableFlag      = true;
   mIterationCount  = 0;
   mExecutionCount      = 0;
   mNumOfIterations = aNumOfIterations;
   mNumOfExecutions     = aNumOfExecutions;
}
//******************************************************************************
void TimerTester::executeOnTimer (void)
{
   if (!mEnableFlag) return;

   mTimeMarker1.doEnd();
   mTimeMarker2=mTimeMarker1;
   mTimeMarker1.doBegin();

   if (mExecutionCount==0)
   {
      mStatistics.initialize(1,0.0);
   }

   double tTestPeriodUsec = double(mTimerPeriod)*1000.0;
   double tTimerError = tTestPeriodUsec - mTimeMarker2.elapsedTimeInMicroseconds();

   if (mExecutionCount>0)
   {
      mStatistics.putValue(tTimerError);
   }

   if (++mExecutionCount==mNumOfExecutions)
   {
      mStatistics.finalize();

      mExecutionCount=0; 
      if (++mIterationCount==mNumOfIterations) mEnableFlag=false;

      Prn::print(0,"TIMER TESTER   %5d   %8.2f   %8.2f   %8.2f    %8.2f",
         mIterationCount,
         mStatistics.mMin,
         mStatistics.mMax,
         mStatistics.mMean,
         mStatistics.mExtremum);
   }
}

}//namespace

