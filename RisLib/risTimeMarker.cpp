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
   mStatistics.initialize(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void PeriodicTimeMarker::initialize(int aWindowSize)
{
   mTimeAtUpdateUS = 0.0;
   mTimeAtLastUpdateUS = 0.0;
   mFirstFlag = true;
   mChangeCount = 0;

   // Initialize statistics.
   mStatistics.initialize(aWindowSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read the current time, take the difference between the current time
// and the pervious time and update the statistics.

void PeriodicTimeMarker::doUpdate()
{
   // Store the pervious time.
   mTimeAtLastUpdateUS = mTimeAtUpdateUS;

   // Read current time from hardware.
   mTimeAtUpdateUS = Ris::getCurrentProgramTimeUS();

   // Calculate time difference.
   mTimeDifferenceUS = mTimeAtUpdateUS - mTimeAtLastUpdateUS;

   // Calculate statistics on delta time.
   if (!mFirstFlag)
   {
      mStatistics.put(mTimeDifferenceUS);

      if (mStatistics.mEndOfPeriod)
      {
         mChangeCount++;
      }
   }

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
   mTimeAtStartUS = Ris::getCurrentProgramTimeUS();

   // Set flag.
   mStartFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TrialTimeMarker::doStop()
{
   // Read stop time from hardware.
   mTimeAtStopUS = Ris::getCurrentProgramTimeUS();

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
