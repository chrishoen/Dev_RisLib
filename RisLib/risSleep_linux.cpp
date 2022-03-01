/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <time.h>

#include "risNanoTime.h"
#include "risSleep.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sleep milliseconds.

void sleepMs(int aSleepForMs)
{
   if (aSleepForMs == 0) return;
   long long tCurrentNs = get_Nanotime();
   long long  tSleepForNs = get_NsFromMs(aSleepForMs);
   long long tSleepUntilNs = tCurrentNs + tSleepForNs;
   timespec tSleepUntilTimespec = get_TimespecFromNs(tSleepUntilNs);
   clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &tSleepUntilTimespec, 0);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sleep microseconds.

void sleepUs(int aSleepForUs)
{
   if (aSleepForUs == 0) return;
   long long tCurrentNs = get_Nanotime();
   long long  tSleepForNs = get_NsFromUs(aSleepForUs);
   long long tSleepUntilNs = tCurrentNs + tSleepForNs;
   timespec tSleepUntilTimespec = get_TimespecFromNs(tSleepUntilNs);
   clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &tSleepUntilTimespec, 0);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

