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
#include "someRandomTestThread.h"

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
   if (gRandomTestThread->mStatPollFlag)
   {
      gRandomTestThread->mStatPollFlag = false;
      Prn::print(0, "Timer1 %5d %2d $$ %10.1f %10.1f %10.1f %10.1f $$ %10.1f",
         gRandomTestThread->mStatCount,
         gRandomTestThread->mThreadCurrentProcessor,
         gRandomTestThread->mStatJitterMean,
         gRandomTestThread->mStatJitterStdDev,
         gRandomTestThread->mStatJitterMin,
         gRandomTestThread->mStatJitterMax,
         gRandomTestThread->mStatJitterMaxMax);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace