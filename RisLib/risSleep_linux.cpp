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
   mSleepMeanMs = 1000;
   mSleepRandom = 0;
   mSleepUs1 = 1000;
   mSleepUs2 = 1000;
}

RandomSleepMs::RandomSleepMs(int aSleepMeanMs, double aSleepRandom)
{
   initialize(aSleepMeanMs, aSleepRandom);
}

void RandomSleepMs::initialize(int aSleepMeanMs, double aSleepRandom)
{
   // Calculate the time limits for the random number generator.
   if (aSleepRandom < 0) aSleepRandom = 0;
   if (aSleepRandom > 1) aSleepRandom = 1;
   mSleepMeanMs = aSleepMeanMs;
   mSleepRandom = aSleepRandom;
   mSleepUs1 = (int)(mSleepMeanMs * 1000 * (1.0 - mSleepRandom));
   mSleepUs2 = (int)(mSleepMeanMs * 1000 * (1.0 + mSleepRandom));
   
   // Seed the random number generator.
   std::random_device tRandomDevice;
   mRandomGenerator.seed(tRandomDevice());
   mRandomDistribution = std::uniform_int_distribution<>(mSleepUs1, mSleepUs2);
}

void RandomSleepMs::initialize(int aSleepRandomIntervalMs1, int aSleepRandomIntervalMs2)
{
   // Calculate the time limits for the random number generator.
   mSleepUs1 = aSleepRandomIntervalMs1 * 1000;
   mSleepUs2 = aSleepRandomIntervalMs2 * 1000;

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

