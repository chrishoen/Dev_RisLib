/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <windows.h> 

#include "prnPrint.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TimerThread::TimerThread()
{
   // Set base class thread priority
   BaseClass::setThreadPriorityHigh();

   int tFrequency = 1;
   int tPeriod = 1000 / tFrequency;

   // Set timer period
   BaseClass::mTimerPeriod = tPeriod;

   mTimeMarker.initialize(5*tFrequency);
   mTimerMode=1;
   return;


   // Members
   mTPFlag = false;


   mTimerMode=2;
   BaseClass::mTimerPeriod = 1000;
   mTimerTester.startTest(1000,1000,10);

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeOnTimer(int aTimeCount)
{
   if (mTimerMode == 1)
   {
      executeOnTimer1(aTimeCount);
   }
   if (mTimerMode == 2)
   {
      executeOnTimer2(aTimeCount);
   }
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeOnTimer1(int aTimeCount)
{
   mTimeMarker.doStop();
   mTimeMarker.doStart();

   if (aTimeCount == 0)
   {
      int tPrioriyClass = GetPriorityClass(GetCurrentProcess());
      int tThreadPriority = BaseThread::getThreadPriority();
      Prn::print(Prn::ThreadRun, Prn::Run1, "TimerThread::executeOnTimer THREAD %08X %d", tPrioriyClass, tThreadPriority);
   }

   if (mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun, Prn::Run1, " %10d %10d  %10.3f  %10.3f  %10.3f  %10.3f",
         aTimeCount,
         mTimeMarker.mChangeCount,
         mTimeMarker.mStatistics.mMean/1000.0,
         mTimeMarker.mStatistics.mStdDev/1000.0,
         mTimeMarker.mStatistics.mMinX/1000.0,
         mTimeMarker.mStatistics.mMaxX/1000.0);

   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeOnTimer2(int aTimeCount)
{
   mTimerTester.executeOnTimer();
}
}//namespace