/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <Windows.h>
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
   mSleepMs1 = (int)(mSleepMeanMs * (1.0 - mSleepRandom));
   mSleepMs2 = (int)(mSleepMeanMs * (1.0 + mSleepRandom));

   // Seed the random number generator.
   std::random_device tRandomDevice;
   mRandomGenerator.seed(tRandomDevice());
   mRandomDistribution = std::uniform_int_distribution<>(mSleepMs1, mSleepMs2);
}

void RandomSleepMs::initialize(int aSleepRandomIntervalMs1, int aSleepRandomIntervalMs2)
{
   // Calculate the time limits for the random number generator.
   mSleepMs1 = aSleepRandomIntervalMs1;
   mSleepMs2 = aSleepRandomIntervalMs2;

   // Seed the random number generator.
   std::random_device tRandomDevice;
   mRandomGenerator.seed(tRandomDevice());
   mRandomDistribution = std::uniform_int_distribution<>(mSleepMs1, mSleepMs2);
}

void RandomSleepMs::doSleep()
{
   // Get a random sleep time.
   int tSleepMs = mRandomDistribution(mRandomGenerator);
   // Sleep.
   Sleep(tSleepMs);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

