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

RandomSleepUs::RandomSleepUs()
{
   // Set base class variables.
   mSleepUs1 = 1000000;
   mSleepUs2 = 1000000;
}

RandomSleepUs::RandomSleepUs(int aSleepUs1, int aSleepUs2)
{
   initialize(aSleepUs1, aSleepUs2);
}

void RandomSleepUs::initialize(int aSleepUs1, int aSleepUs2)
{
   // Seed random generator.
   std::random_device tRandomDevice;
   mRandomGenerator.seed(tRandomDevice());
   mRandomDistribution = std::uniform_int_distribution<>(mSleepUs1, mSleepUs2);
}

void RandomSleepUs::doSleep()
{
   // Get a random sleep time.
   int tSleepUs = mRandomDistribution(mRandomGenerator);
   // Sleep.
   sleepUs(tSleepUs);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

RandomSleepMs::RandomSleepMs()
{
   // Set base class variables.
   mSleepMs1 = 1000;
   mSleepMs2 = 1000;
}

RandomSleepMs::RandomSleepMs(int aSleepMs1, int aSleepMs2)
{
   initialize(aSleepMs1, aSleepMs2);
}

void RandomSleepMs::initialize(int aSleepMs1, int aSleepMs2)
{
   // Seed random generator.
   std::random_device tRandomDevice;
   mRandomGenerator.seed(tRandomDevice());
   mRandomDistribution = std::uniform_int_distribution<>(mSleepMs1, mSleepMs2);
}

void RandomSleepMs::doSleep()
{
   // Get a random sleep time.
   int tSleepMs = mRandomDistribution(mRandomGenerator);
   // Sleep.
   sleepMs(tSleepMs);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

