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
#include "someTimerTestThread1.h"

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
   BaseClass::setThreadName("Monitor");

   // Set base class variables.
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mMonitor);

   // Set timer period.
   BaseClass::mTimerPeriod = gPeriodicParms.mMonitorThreadPeriod;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void MonitorThread::executeOnTimer(int aTimeCount)
{
   if (gTimerTestThread1->mUpdateFlag)
   {
      gTimerTestThread1->mUpdateFlag = false;
      Prn::print(0, "Timer1 %5d %2d $$ %10.1f %10.1f %10.1f %10.1f $$ %10.1f",
         gTimerTestThread1->mTestCount,
         gTimerTestThread1->mProcessorNumber,
         gTimerTestThread1->mMean,
         gTimerTestThread1->mStdDev,
         gTimerTestThread1->mMin,
         gTimerTestThread1->mMax,
         gTimerTestThread1->mMaxError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace