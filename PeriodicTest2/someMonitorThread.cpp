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
   if (gTimerTestThread1->mStatPollFlag)
   {
      gTimerTestThread1->mStatPollFlag = false;
      Prn::print(0, "Timer2 %5d %2d $$ %10.1f %10.1f %10.1f %10.1f $$ %10.1f",
         gTimerTestThread1->mStatCount,
         gTimerTestThread1->mThreadCurrentProcessor,
         gTimerTestThread1->mStatJitterMean,
         gTimerTestThread1->mStatJitterStdDev,
         gTimerTestThread1->mStatJitterMin,
         gTimerTestThread1->mStatJitterMax,
         gTimerTestThread1->mStatJitterMaxMax);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace