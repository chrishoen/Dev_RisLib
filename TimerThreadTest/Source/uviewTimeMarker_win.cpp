/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>

#include "risPortableCalls.h"
#include "uviewTimeMarker.h"

namespace UView
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TimeMarker::TimeMarker()
{
   // All zero
   mTimeCountAtStart = 0;
   mTimeCountAtStop = 0;
   mStartFlag=false;
   mChangeCount = 0;
   mScaleFactorUS = 0.0;

   // Scale factor
   mScaleFactorUS = (float)((1E6)/Ris::portableGetHiResFrequency());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimeMarker::initialize(int aWindowSize)
{
   // All zero
   mTimeCountAtStart = 0;
   mTimeCountAtStop = 0;
   mStartFlag=false;
   mChangeCount=0;

   // Scale factor
   mScaleFactorUS = (float)((1E6)/Ris::portableGetHiResFrequency());

   // Initialize statistics
   mStatistics.initialize(aWindowSize);
}

//******************************************************************************

void TimeMarker::doStart()
{
   // Read start time from hardware
   mTimeCountAtStart = Ris::portableGetHiResCounter();

   // Set flag
   mStartFlag=true;
}

//******************************************************************************

void TimeMarker::doStop()
{
   // Read stop time from hardware
   mTimeCountAtStop = Ris::portableGetHiResCounter();

   long long tDeltaTimeCount = mTimeCountAtStop - mTimeCountAtStart;

   // Calculate delta time in microseconds
   mTimeDifferenceUS = (float)(tDeltaTimeCount*mScaleFactorUS);

   // Calculate statistics on delta time
   if (mStartFlag)
   {
      mStatistics.put(mTimeDifferenceUS);

      if (mStatistics.mEndOfPeriod)
      {
         mChangeCount++;
      }
   }
}

//******************************************************************************
}//namespace
