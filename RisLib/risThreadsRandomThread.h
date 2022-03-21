#pragma once

/*==============================================================================
Random periodic thread base class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <random>

#include "risThreadsThreads.h"
#include "risStatistics.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This provides a base class for randomly aperiodic timer threads. Inheritors
// provide a timer handler function that gets called aperiodically. It inherits
// from the thread base class to obtain basic thread functionality.
// 
// This also measures execution times and provides statistics on periodic
// jitter and on the exection time of the inheritor supplied timer handler
// function.
//   
// The execution times are repeatedly measured and associtated statistics 
// are calculated at a specific period. The results are latched and can be
// polled by a monitoring thread.

class  BaseRandomThread : public BaseThread
{
private:
   typedef Ris::Threads::BaseThread BaseClass;
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members. Inheritors set these before thread launch.

   // Timer interval range, microseconds. These provide the bounds for a
   // uniform random number generator that generates a random sleep interval.
   // The inheritor timer handler function gets called after the random sleep.
   // The bounds are calculated as
   // [IntervalMeanMs*1000 - IntervalRandomUs, IntervalMeanMs*1000 + IntervalRandomUs].
   int mIntervalMeanMs;
   int mIntervalRandomUs;

   // If true then get the thread processor number when the statistics
   // are finished.
   bool mPollProcessor;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members. 

   // Timer interval range, microseconds. These provide the bounds for a
   // uniform random number generator that generates a random sleep interval.
   // The inheritor timer handler function gets called after the random sleep.
   int mIntervalUs1;
   int mIntervalUs2;

   // Timer count incremented at each timer handler execution.
   int mTimerCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Random numbers.
   std::mt19937 mRandomGenerator;
   std::uniform_int_distribution<> mRandomDistribution;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Trial statistics for periodic jitter and execution time.
   TrialStatistics mStatJitter;
   TrialStatistics mStatExec;

   // Trial statistics period, milliseconds. This is the time for which
   // the statistics are measured and calculated.
   int mStatPeriod;

   // Latched results. These are copied from the statistics at the end 
   // of a repeated trial. They can be used by a monitor thread to poll
   // the results.
   double mStatJitterMean;
   double mStatJitterStdDev;
   double mStatJitterMin;
   double mStatJitterMax;
   double mStatJitterMaxMax;
   double mStatExecMean;
   double mStatExecStdDev;
   double mStatExecMin;
   double mStatExecMax;
   double mStatExecMaxMax;

   // If true then the above latched results have been updated. This is
   // used by a monitoring thread to poll the results. The monitoring
   // thread resets it after reading the above latched results.
   bool mStatPollFlag;

   // Timer count incremented at each timer execution for statistics.
   // This resets at the beginning of a statistics trial.
   int mStatTimerCount;

   // Count incremented at each periodic evaluation of statistics.
   int mStatCount;

   // Maximum statistics timer count. This defines how many timer counts
   // are in a statistics trial. It has a value that is the trial statistics
   // period divided by the thread timer period.
   // The statistics timer count varies as 0..timer count max - 1.
   int mStatTimerCountMax;

   // The beginning time of when the timer execution function is called;
   // The previous value.
   // The end time of when the timer execution function is called;
   long long mStatBeginTimeNs;
   long long mStatLastBeginTimeNs;
   long long mStatEndTimeNs;

   // The current jitter and execution times.
   double mStatJitterTimeUs;
   double mStatExecTimeUs;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   BaseRandomThread(); 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Base class overloads.

   // Thread run function. This is called by the base class immediately 
   // after the thread init function. It runs a loop that waits for the
   // timer or the termination event.
   void threadRunFunction() override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Inheritor overloads.

   // An overload of this is supplied by the inheritor.
   // It is called periodically by the threadRunFunction.
   virtual void executeOnTimer(int aTimerCount) = 0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


