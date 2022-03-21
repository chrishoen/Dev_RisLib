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

#define  _SOMEPERIODICTESTTHREAD1_CPP_
#include "somePeriodicTestThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

PeriodicTestThread::PeriodicTestThread()
{
   // Set base class variables.
   BaseClass::setThreadName("TimerTest");
   BaseClass::setThreadPriority(Ris::Threads::gPriorities.mTimerTest);
   BaseClass::setThreadPriority(
      Ris::Threads::Priority(
         gPeriodicParms.mTestThreadProcessor,
         gPeriodicParms.mTestThreadPriority));

   BaseClass::mPollProcessor = gPeriodicParms.mPollProcessor;
   BaseClass::mStatPeriod = gPeriodicParms.mStatPeriod;
   BaseClass::mPeriodUs = gPeriodicParms.mPeriodUs;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void PeriodicTestThread::executeOnTimer(int aTimeCount)
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