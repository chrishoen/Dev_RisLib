/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risProgramTime.h"
#include "risTimeMarker.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

PeriodicTimeMarker::PeriodicTimeMarker()
{
   mTimeAtUpdateUS = 0.0;
   mTimeAtUpdateUS = 0.0;
   mTimeAtLastUpdateUS = 0.0;
   mTimeDifferenceUS = 0.0;
   mFirstFlag = true;
   mChangeCount = 0;
   mStatistics.initialize(1,1000);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void PeriodicTimeMarker::initialize(int aWindowSize, double aPeriod)
{
   mTimeAtUpdateUS = 0.0;
   mTimeAtLastUpdateUS = 0.0;
   mFirstFlag = true;
   mChangeCount = 0;

   // Initialize statistics.
   mStatistics.initialize(aWindowSize, aPeriod);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read the current time, take the difference between the current time
// and the pervious time and update the statistics.

void PeriodicTimeMarker::doUpdate()
{
   // Read current time from hardware.
   mTimeAtUpdateUS = Ris::getProgramTimeUS();

   if (!mFirstFlag)
   {
      // Calculate time difference.
      mTimeDifferenceUS = mTimeAtUpdateUS - mTimeAtLastUpdateUS;

      // Calculate statistics on the difference.
      mStatistics.put(mTimeDifferenceUS);

      if (mStatistics.mEndOfPeriod)
      {
         mChangeCount++;
      }
   }

   // Store the previous time.
   mTimeAtLastUpdateUS = mTimeAtUpdateUS;

   // Not first.
   mFirstFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

TrialTimeMarker::TrialTimeMarker()
{
   mTimeAtStartUS = 0;
   mTimeAtStopUS = 0;
   mTimeDifferenceUS = 0;
   mStartFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TrialTimeMarker::startTrial(double aXLimit)
{
   mTimeAtStartUS = 0;
   mTimeAtStopUS = 0;
   mTimeDifferenceUS = 0;
   mStartFlag = false;

   // Initialize statistics.
   mStatistics.startTrial(aXLimit);
}

void TrialTimeMarker::finishTrial()
{
   mStatistics.finishTrial();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TrialTimeMarker::doStart()
{
   // Read start time from hardware.
   mTimeAtStartUS = Ris::getProgramTimeUS();

   // Set flag.
   mStartFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TrialTimeMarker::doStop()
{
   // Read stop time from hardware.
   mTimeAtStopUS = Ris::getProgramTimeUS();

   // Calculate time difference.
   mTimeDifferenceUS = mTimeAtStopUS - mTimeAtStartUS;

   // Calculate statistics on delta time.
   if (mStartFlag)
   {
      mStatistics.put(mTimeDifferenceUS);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
