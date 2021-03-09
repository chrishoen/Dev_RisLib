#pragma once

/*==============================================================================
Periodic thread base class
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

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
// This is an extension of the virtual base class for general purpose threads.
// It provides a basis for a periodic thread that uses a polling termination
// policy.

class  BasePeriodicThread : public BaseThread
{
public:

   typedef Ris::Threads::BaseThread BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Timer period, milliseconds. Inheritors should set this in their
   // constructors. 
   int mTimerPeriod;

   // Timer count incremented at each timer execution.
   int mTimerCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Trial statistics for execution periodic jitter and execution time.
   TrialStatistics mStatJitter;
   TrialStatistics mStatExec;

   // Trial statistics period, milliseconds.
   int mStatPeriod;

   // Latched results. These are copied from the statistics at the end 
   // of a repeated trial. They can be used by a monitor thread to poll
   // the results.
   double mStatJitterMean;
   double mStatJitterStdDev;
   double mStatJitterMin;
   double mStatJitterMax;
   double mStatExecMean;
   double mStatExecStdDev;
   double mStatExecMin;
   double mStatExecMax;

   // If true then the above latched results have been updated. This is
   // used by a monitoring thread to poll the results. The monitoring
   // thread resets it after reading the above latched results.
   bool mStatPollFlag;

   // Timer count incremented at each timer execution for statistics.
   // This resets at the beginning of a statistics trial.
   int mStatTimerCount;

   // Maximum statistics timer count. This defines how many timer counts
   // are in a statistics trial. It has a value that is the trial statistics
   // period divided by the thread timer period.
   // The statistics timer count varies as 0..timer count max - 1.
   int mStatTimerCountMax;

   // The beginning time of when the timer execution function is called;
   // The previous value.
   // The end time of when the timer execution function is called;
   double mStatBeginTimeUs;
   double mStatLastBeginTimeUs;
   double mStatEndTimeUs;

   // The current jitter and execution times.
   double mStatJitterTimeUs;
   double mStatExecTimeUs;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   BasePeriodicThread(); 

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
   virtual void executeOnTimer(int aTimerCount)=0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


