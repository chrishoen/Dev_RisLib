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

#define  _SOMEMONITORTHREAD_CPP_
#include "someMonitorThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

MonitorThread::MonitorThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Timer");
   BaseClass::setThreadPrintLevel(0);

   // Set base class variables.
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mMonitor);

   // Set timer period.
   BaseClass::mTimerPeriod = 10*1000;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void MonitorThread::executeOnTimer(int aTimeCount)
{
#if 0
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
#endif
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace