/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

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

   // Set timer period
   BaseClass::mTimerPeriod = 1000/100;

   mTimeMarker.initialize(5*100);

   // Members
   mTPFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeOnTimer(int aTimeCount)
{
   mTimeMarker.doStop();
   mTimeMarker.doStart();

   if (mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun, Prn::Run1, "1 %10d  %10.4f  %10.4f  %10.4f  %10.4f",
         mTimeMarker.mChangeCount,
         mTimeMarker.mStatistics.mMean,
         mTimeMarker.mStatistics.mStdDev,
         mTimeMarker.mStatistics.mMinX,
         mTimeMarker.mStatistics.mMaxX);
   }

   if (mTPFlag) Prn::print(0,0,"TTTimer %04d",aTimeCount);


}

}//namespace