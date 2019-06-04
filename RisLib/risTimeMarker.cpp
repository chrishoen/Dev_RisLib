/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risHiresTime.h"
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
   // All zero
   mTimeCountAtStart = 0;
   mTimeCountAtStop = 0;
   mStartFlag=false;
   mChangeCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void PeriodicTimeMarker::initialize(int aWindowSize)
{
   // All zero.
   mTimeCountAtStart = 0;
   mTimeCountAtStop = 0;
   mStartFlag=false;
   mChangeCount=0;

   // Initialize statistics.
   mStatistics.initialize(aWindowSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void PeriodicTimeMarker::doStart()
{
   // Read start time from hardware.
   mTimeCountAtStart = Ris::getCurrentHiresTime();

   // Set flag.
   mStartFlag=true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void PeriodicTimeMarker::doStop()
{
   // Read stop time from hardware.
   mTimeCountAtStop = Ris::getCurrentHiresTime();

   unsigned long long int tDeltaTimeCount = mTimeCountAtStop - mTimeCountAtStart;

   // Calculate delta time in microseconds.
   mTimeDifferenceUS = tDeltaTimeCount*(1E-3);

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
//******************************************************************************
//******************************************************************************
//******************************************************************************

TrialTimeMarker::TrialTimeMarker()
{
   // All zero.
   mTimeCountAtStart = 0;
   mTimeCountAtStop = 0;
   mStartFlag=false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TrialTimeMarker::startTrial(double aXLimit)
{
   // All zero.
   mTimeCountAtStart = 0;
   mTimeCountAtStop = 0;
   mStartFlag=false;

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
   mTimeCountAtStart = Ris::getCurrentHiresTime();

   // Set flag.
   mStartFlag=true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TrialTimeMarker::doStop()
{
   // Read stop time from hardware.
   mTimeCountAtStop = Ris::getCurrentHiresTime();

   unsigned long long int tDeltaTimeCount = mTimeCountAtStop - mTimeCountAtStart;

   // Calculate delta time in microseconds.
   mTimeDifferenceUS = tDeltaTimeCount*(1E-3);

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
