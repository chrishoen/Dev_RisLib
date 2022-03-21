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

#define  _SOMERANDOMTESTTHREAD_CPP_
#include "someRandomTestThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

RandomTestThread::RandomTestThread()
{
   // Set base class variables.
   BaseClass::setThreadName("TimerTest");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mTimerTest);
   BaseClass::setThreadPriority(
      Ris::Threads::Priority(
         gPeriodicParms.mTestThreadProcessor,
         gPeriodicParms.mTestThreadPriority));

   BaseClass::mPollProcessor = gPeriodicParms.mPollProcessor;
   BaseClass::mIntervalMeanMs = gPeriodicParms.mIntervalMeanMs;
   BaseClass::mIntervalRandomUs = gPeriodicParms.mIntervalRandomUs;
   BaseClass::mStatPeriod = gPeriodicParms.mStatPeriod;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void RandomTestThread::executeOnTimer(int aTimeCount)
{
   if (aTimeCount == 0)
   {
      BaseClass::showThreadFullInfo();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace