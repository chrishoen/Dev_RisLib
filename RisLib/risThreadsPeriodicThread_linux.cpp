//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <time.h>

#include "prnPrint.h"
#include "risNanoConvert.h"

#include "risThreadsPeriodicThread.h"

namespace Ris
{
namespace Threads
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

BasePeriodicThread::BasePeriodicThread()
{
   mTimerPeriod = 1000;
   mTimerCount = 0;
   mTerminateFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that waits for the
// timer or the termination event.

void BasePeriodicThread::threadRunFunction()
{
   // Time variables
   timespec   tSleepTimespec;
   long long  tSleepTimeNs;
   long long  tTimerPeriodNs = Ris::NanoConvert::getNsFromMs(mTimerPeriod);

   // Get current timespec at start.
   clock_gettime(CLOCK_MONOTONIC, &tSleepTimespec);

   // Convert to ns.
   Ris::NanoConvert::getNsFromTimespec(tSleepTimeNs, &tSleepTimespec);

   // Loop until thread terminate.
   while (!BaseClass::mTerminateFlag)
   {
      // Advance the sleep time by the period.
      tSleepTimeNs += tTimerPeriodNs;

      // Convert to timespec.
      Ris::NanoConvert::getTimespecFromNs(&tSleepTimespec, tSleepTimeNs);

      // Sleep until the absolute sleep time.
      clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &tSleepTimespec, 0);

      // Call the inheritors handler for the timer.
      executeOnTimer(mTimerCount++);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

