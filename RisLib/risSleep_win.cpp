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

