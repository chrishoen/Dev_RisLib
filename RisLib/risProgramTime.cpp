/*==============================================================================
==============================================================================*/

#include "stdafx.h"

#include "risPortableCalls.h"
#include "risProgramTime.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regionals

// High performance time count at program start
static unsigned long long int mProgramStartTimeCount = Ris::portableGetHiResCounter();

// Scale factor used to calculate the current program time
static double mScaleFactorSec = (double)((1.0)/Ris::portableGetHiResFrequency());

   
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read current time and subtract program start time.
// This returns the current program time in seconds.

double getCurrentProgramTime()
{
   unsigned long long int mCurrentTimeCount = Ris::portableGetHiResCounter();
   return (mCurrentTimeCount - mProgramStartTimeCount)*mScaleFactorSec;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constuctor.

ProgramTimeMarker::ProgramTimeMarker()
{
   mTimeDifferenceUS = 0.0;
   mTimeAtStartUS = 0.0;
   mTimeAtStopUS = 0.0;
   mStartFlag = false;
   mChangeCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ProgramTimeMarker::initialize(int aWindowSize)
{
   mTimeDifferenceUS = 0.0;
   mTimeAtStartUS = 0.0;
   mTimeAtStopUS = 0.0;
   mStartFlag = false;
   mChangeCount = 0;

   // Initialize statistics.
   mStatistics.initialize(aWindowSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Start/stop the time measurement.

void ProgramTimeMarker::doStart()
{
   // Read start time from hardware.
   mTimeAtStartUS = getCurrentProgramTime() * 1E6;

   // Set flag.
   mStartFlag = true;
}

void ProgramTimeMarker::doStop()
{
   // Read stop time from hardware.
   mTimeAtStopUS = getCurrentProgramTime() * 1E6;

   // Calculate delta time in microseconds.
   mTimeDifferenceUS = mTimeAtStopUS - mTimeAtStartUS;

   // Calculate statistics on delta time.
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
//******************************************************************************
//******************************************************************************
}//namespace

