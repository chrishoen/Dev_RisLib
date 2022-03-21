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

RandomSleep::RandomSleep()
{
   // Set base class variables.
   mSleepMeanMs = 0;
   mSleepRandomUs = 0;
   mSleepUs1 = 1000000;
   mSleepUs2 = 1000000;
}

RandomSleep::RandomSleep(int aSleepMeanMs, int aSleepRandomUs)
{
   initialize(aSleepMeanMs, aSleepRandomUs);
}

void RandomSleep::initialize(int aSleepMeanMs, int aSleepRandomUs)
{
   // Sleep time variables.
   mSleepMeanMs = aSleepMeanMs;
   mSleepRandomUs = aSleepRandomUs;
   mSleepUs1 = mSleepMeanMs * 1000 - mSleepRandomUs;
   mSleepUs2 = mSleepMeanMs * 1000 + mSleepRandomUs;

   // Seed the random number generator.
   std::random_device tRandomDevice;
   mRandomGenerator.seed(tRandomDevice());
   mRandomDistribution = std::uniform_int_distribution<>(mSleepUs1, mSleepUs2);
}

void RandomSleep::doSleep()
{
   // Get a random sleep time.
   int tSleepUs = mRandomDistribution(mRandomGenerator);
   // Sleep.
   sleepUs(tSleepUs);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

