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
#include "someTimerTestThread.h"

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
   BaseClass::setThreadPrintLevel(0);

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

   if (gTimerTestThread->mUpdateFlag)
   {
      gTimerTestThread->mUpdateFlag = false;
      Prn::print(0, "%5d %1d $$ %10.1f %10.1f %10.1f %10.1f $$ %10.1f",
         gTimerTestThread->mTestCount,
         gTimerTestThread->mProcessorNumber,
         gTimerTestThread->mMean,
         gTimerTestThread->mStdDev,
         gTimerTestThread->mMin,
         gTimerTestThread->mMax,
         gTimerTestThread->mMaxError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace