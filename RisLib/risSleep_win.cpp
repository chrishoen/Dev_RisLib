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
   mSleepRandomUs = 0;
   mSleepUs1 = 0;
   mSleepUs2 = 0;
}

void RandomSleepMs::initializeMsMs(int aSleepMeanMs, int aSleepRandomMs)
{
   // Sleep time variables.
   mSleepMeanMs = aSleepMeanMs;
   mSleepRandomUs = aSleepRandomMs * 1000;
   mSleepUs1 = mSleepMeanMs * 1000 - mSleepRandomUs;
   mSleepUs2 = mSleepMeanMs * 1000 + mSleepRandomUs;

   // Seed the random number generator.
   std::random_device tRandomDevice;
   mRandomGenerator.seed(tRandomDevice());
   mRandomDistribution = std::uniform_int_distribution<>(mSleepUs1, mSleepUs2);
}

void RandomSleepMs::initializeMsUs(int aSleepMeanMs, int aSleepRandomUs)
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

