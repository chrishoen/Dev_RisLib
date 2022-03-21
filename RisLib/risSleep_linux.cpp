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

RandomSleepMs::RandomSleepMs()
{
   // Set base class variables.
   mSleepMeanMs = 1000;
   mSleepRandomMs = 0;
   mSleepUs1 = 0;
   mSleepUs2 = 0;
}

RandomSleepMs::RandomSleepMs(int aSleepMeanMs, int aSleepRandomMs)
{
   initialize(aSleepMeanMs, aSleepRandomMs);
}

void RandomSleepMs::initialize(int aSleepMeanMs, int aSleepRandomMs)
{
   // Sleep time variables.
   mSleepMeanMs = aSleepMeanMs;
   mSleepRandomMs = aSleepRandomMs;
   mSleepUs1 = mSleepMeanMs * 1000 - mSleepRandomMs * 1000;
   mSleepUs2 = mSleepMeanMs * 1000 + mSleepRandomMs * 1000;

   // Seed the random number generator.
   std::random_device tRandomDevice;
   mRandomGenerator.seed(tRandomDevice());
   mRandomDistribution = std::uniform_int_distribution<>(mSleepUs1, mSleepUs2);
}

void RandomSleepMs::doSleep()
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

