#ifndef _RISTIMEMARKER_H_
#define _RISTIMEMARKER_H_
/*==============================================================================

This file contains a class that encapsulate a time marker that can be used
to measure computer execution time durations

==============================================================================*/

#include "risStatistics.h"


//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class PeriodicTimeMarker
{
public:
   //------------------------------------------------
   PeriodicTimeMarker();

   void initialize(int aWindowSize);

   void doStart();
   void doStop();

   //------------------------------------------------
   // Timer counter values at start and stop of 
   // measured time interval.

   long long mTimeCountAtStart;
   long long mTimeCountAtStop;

   // Difference between stop and start time counts, in microseconds
   float mTimeDifferenceUS;

   // Scale factor used to calculate the time difference
   float mScaleFactorUS;

   // True if a start has occurred
   bool mStartFlag;
   
   //------------------------------------------------
   // Statistics that are calculated for measured
   // time interval

   Ris::PeriodicStatistics mStatistics;
   int mChangeCount;
};
//******************************************************************************
//******************************************************************************
//******************************************************************************

class TrialTimeMarker
{
public:
   //------------------------------------------------
   TrialTimeMarker();

   void startTrial(double aXLimit=0.0);
   void finishTrial();

   void doStart();
   void doStop();

   //------------------------------------------------
   // Timer counter values at start and stop of 
   // measured time interval.

   long long mTimeCountAtStart;
   long long mTimeCountAtStop;

   // Difference between stop and start time counts, in microseconds
   float mTimeDifferenceUS;

   // Scale factor used to calculate the time difference
   float mScaleFactorUS;

   // True if a start has occurred
   bool mStartFlag;
   
   //------------------------------------------------
   // Statistics that are calculated for measured
   // time interval

   Ris::TrialStatistics mStatistics;
};
}//namespace

#endif
