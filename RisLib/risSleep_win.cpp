/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risSleep.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sleep milliseconds.

void sleepMs(int aSleepForMs)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sleep microseconds.

void sleepUs(int aSleepForUs)
{
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

