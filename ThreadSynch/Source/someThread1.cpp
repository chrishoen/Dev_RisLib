/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "someShare.h"
#include "Experiment.h"

#define  _SOMETHREAD1_CPP_
#include "someThread1.h"

namespace Some
{

Thread1::Thread1()
{
   // Set base class thread priority
   BaseClass::setThreadPriorityHigh();
}

void Thread1::threadRunFunction()
{
   while (true)
   {
      switch (gShare.mTest)
      {
      case 1:
         runTest1();
         break;
      case 2:
         runTest2();
         break;
      case 3:
         runTest3();
         break;
      default:
         threadSleep(200);
         break;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::runTest1()
{
   mThreadSem.get();
   gShare.mTimeMarker.doStop();

   if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun1, "TEST1 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         gShare.mTimeMarker.mStatistics.mSize,
         gShare.mTimeMarker.mStatistics.mMean,
         gShare.mTimeMarker.mStatistics.mStdDev,
         gShare.mTimeMarker.mStatistics.mMinX,
         gShare.mTimeMarker.mStatistics.mMaxX);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::runTest2()
{
   mConditionVariable.waitFor();
   gShare.mTimeMarker.doStop();

   if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun1, "TEST2 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         gShare.mTimeMarker.mStatistics.mSize,
         gShare.mTimeMarker.mStatistics.mMean,
         gShare.mTimeMarker.mStatistics.mStdDev,
         gShare.mTimeMarker.mStatistics.mMinX,
         gShare.mTimeMarker.mStatistics.mMaxX);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::runTest3()
{
   Experiment::receive();
   gShare.mTimeMarker.doStop();

   if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun1, "TEST3 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         gShare.mTimeMarker.mStatistics.mSize,
         gShare.mTimeMarker.mStatistics.mMean,
         gShare.mTimeMarker.mStatistics.mStdDev,
         gShare.mTimeMarker.mStatistics.mMinX,
         gShare.mTimeMarker.mStatistics.mMaxX);
   }
}

}//namespace