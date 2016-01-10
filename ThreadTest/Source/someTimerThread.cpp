/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "someShare.h"
#include "Experiment.h"
#include "GSettings.h"
#include "someApcThread.h"
#include "someThread1.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

TimerThread::TimerThread()
{
   // Set base class thread priority

   // Set timer period
   int mTimerFrequency = 20;
   BaseClass::mTimerPeriod = 1000 / mTimerFrequency;

   gShare.mTimeMarker.initialize(5*mTimerFrequency);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void TimerThread::executeOnTimer(int aTimeCount)
{
   //---------------------------------------------------------------------------
   // Time marker

   if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun1, "TEST %1d %1d %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         gGSettings.mTestThread,
         gGSettings.mTestNumber,
         gShare.mTimeMarker.mStatistics.mSize,
         gShare.mTimeMarker.mStatistics.mMean,
         gShare.mTimeMarker.mStatistics.mStdDev,
         gShare.mTimeMarker.mStatistics.mMinX,
         gShare.mTimeMarker.mStatistics.mMaxX);
         gShare.mTimeMarker.mStatistics.mEndOfPeriod = false;
         return;
   }

   gShare.mTimeMarker.doStart();


   //---------------------------------------------------------------------------
   // Send to test thread

   switch (gGSettings.mTestNumber)
   {

   case GSettings::cThread1:
   {
      switch (gGSettings.mTestNumber)
      {
      case 1:
         gThread1->mThreadSem.put();
         break;
      case 2:
         gThread1->mConditionVariable.wakeUp();
         break;
      case 3:
         Experiment::send();
         break;
      }
   }
   break;

   case GSettings::cApcThread:
   {
      switch (gGSettings.mTestNumber)
      {
      case 1:
         gApcThread->enqueueApc();
         break;
      }
   }
   break;
   }
}

}//namespace