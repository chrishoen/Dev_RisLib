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

   int tFrequency = 1000;
   int tPeriod = 1000 / tFrequency;

   // Set timer period
   BaseClass::mTimerPeriod = tPeriod;

   mTimeMarker.initialize(5*tFrequency);

   // Members
   mTPFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeOnTimer(int aTimeCount)
{
   if (!mTPFlag) return;

   mTimeMarker.doStop();
   mTimeMarker.doStart();
#if 0
   if (aTimeCount == 0)
   {
      int tPrioriyClass = GetPriorityClass(GetCurrentProcess());
      int tThreadPriority = BaseThread::getThreadPriority();
      Prn::print(Prn::ThreadRun1, "TimerThread::executeOnTimer THREAD %08X %d", tPrioriyClass, tThreadPriority);
   }
#endif
   if (mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun1, " %10d %10d  %10.3f  %10.3f  %10.3f  %10.3f",
         aTimeCount,
         mTimeMarker.mChangeCount,
         mTimeMarker.mStatistics.mMean/1000.0,
         mTimeMarker.mStatistics.mStdDev/1000.0,
         mTimeMarker.mStatistics.mMinX/1000.0,
         mTimeMarker.mStatistics.mMaxX/1000.0);
   }
}

}//namespace