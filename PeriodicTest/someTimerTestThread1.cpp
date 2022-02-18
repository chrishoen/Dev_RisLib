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

#define  _SOMETIMERTESTTHREAD1_CPP_
#include "someTimerTestThread1.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TimerTestThread1::TimerTestThread1()
{
   // Set base class variables.
   BaseClass::setThreadName("TimerTest");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mTimerTest);
   BaseClass::setThreadPriority(
      Ris::Threads::Priority(
         gPeriodicParms.mTestThreadProcessor,
         gPeriodicParms.mTestThreadPriority));

   BaseClass::mPollProcessor = gPeriodicParms.mPollProcessor;
   BaseClass::mTimerPeriodUs = gPeriodicParms.mTestThreadPeriodUs;
   BaseClass::mStatPeriod = gPeriodicParms.mStatPeriod;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerTestThread1::executeOnTimer(int aTimeCount)
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