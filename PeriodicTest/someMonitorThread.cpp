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
#include "someTimerTestThread2.h"

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
   if (Some::gPeriodicParms.mTestMode == 1)
   {
      if (gTimerTestThread1->mUpdateFlag)
      {
         gTimerTestThread1->mUpdateFlag = false;
         Prn::print(0, "Timer1 %5d %1d $$ %10.1f %10.1f %10.1f %10.1f $$ %10.1f",
            gTimerTestThread1->mTestCount,
            gTimerTestThread1->mProcessorNumber,
            gTimerTestThread1->mMean,
            gTimerTestThread1->mStdDev,
            gTimerTestThread1->mMin,
            gTimerTestThread1->mMax,
            gTimerTestThread1->mMaxError);
      }
   }
   else if (Some::gPeriodicParms.mTestMode == 2)
   {
      Prn::print(0, "Timer2 %5d", gTimerTestThread2->mTestCount);
      if (gTimerTestThread2->mUpdateFlag)
      {
         gTimerTestThread2->mUpdateFlag = false;
         Prn::print(0, "Timer2 %5d %1d $$ %10.1f %10.1f %10.1f %10.1f $$ %10.1f",
            gTimerTestThread2->mTestCount,
            gTimerTestThread2->mProcessorNumber,
            gTimerTestThread2->mMean,
            gTimerTestThread2->mStdDev,
            gTimerTestThread2->mMin,
            gTimerTestThread2->mMax,
            gTimerTestThread2->mMaxError);
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace