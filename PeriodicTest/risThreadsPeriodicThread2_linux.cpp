//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <time.h>

#include "prnPrint.h"
#include "risNanoConvert.h"
#include "risProgramTime.h"

#include "risThreadsPeriodicThread2.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regionals.

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current high resolution timer value in nanoseconds. 

inline long long int my_get_hires_count()
{
   struct timespec tTimespec;
   clock_gettime(CLOCK_MONOTONIC, &tTimespec);

   long long int tSeconds = (long long int)tTimespec.tv_sec;
   long long int tNanoseconds = (long long int)tTimespec.tv_nsec;
   long long int tTimeNs = tSeconds * 1000 * 1000 * 1000 + tNanoseconds;

   return tTimeNs;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BasePeriodicThread2::BasePeriodicThread2()
{
   // Set base class variables.
   mTimerPeriod = 1000;
   mTimerCount = 0;
   mTerminateFlag = false;
   mPollProcessor = false;

   // Set class variables.
   mStatPeriod = 0;
   mStatJitterMean = 0;
   mStatJitterStdDev = 0;
   mStatJitterMin = 0;
   mStatJitterMax = 0;
   mStatJitterMaxMax = 0;
   mStatExecMean = 0;
   mStatExecStdDev = 0;
   mStatExecMin = 0;
   mStatExecMax = 0;
   mStatExecMaxMax = 0;
   mStatPollFlag = false;
   mStatTimerCount = 0;
   mStatCount = 0;
   mStatTimerCountMax = 0;
   mStatBeginTimeUs = 0;
   mStatLastBeginTimeUs = 0;
   mStatEndTimeUs = 0;
   mStatJitterTimeUs = 0;
   mStatExecTimeUs = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that waits for the
// timer or the termination event.

void BasePeriodicThread2::threadRunFunction()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Time variables
   timespec   tSleepTimespec;
   long long  tSleepTimeNs;
   long long  tTimerPeriodNs = Ris::NanoConvert::getNsFromMs(mTimerPeriod);
   double     tTimerPeriodUs = tTimerPeriodNs / 1000.0;

   // Get current timespec at start.
   clock_gettime(CLOCK_MONOTONIC, &tSleepTimespec);

   // Convert to ns.
   tSleepTimeNs = Ris::NanoConvert::getNsFromTimespec(&tSleepTimespec);

   // Initialize statistics variables.
   mStatTimerCountMax = mStatPeriod / mTimerPeriod;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Loop until thread terminate.

   while (!BaseClass::mTerminateFlag)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Advance periodically according to the system clock. 
      
      // Advance the absolute sleep time by the period.
      tSleepTimeNs += tTimerPeriodNs;

      // Convert to timespec.
      tSleepTimespec = Ris::NanoConvert::getTimespecFromNs(tSleepTimeNs);

      // Sleep until the absolute sleep time.
      clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &tSleepTimespec, 0);

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Call the timer handler. 

      // Get the time before timer handler execution.
      mStatBeginTimeUs = getCurrentProgramTimeUS();;

      // Call the inheritor's timer handler function.
      executeOnTimer(mTimerCount);

      // Test the poll processor flag and end of trial.
      if (mPollProcessor && mStatTimerCount == mStatTimerCountMax - 1)
      {
         // Get the current processor number.
         BaseClass::getThreadProcessorNumber();
      }

      // Get the time after timer handler execution.
      mStatEndTimeUs = getCurrentProgramTimeUS();;

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Calculate the time durations. 

      // Test if this is the first time.
      if (mTimerCount == 0)
      {
         // Store the first begin time as the current last.
         mStatLastBeginTimeUs = mStatBeginTimeUs;

         // Ignore everything else in this loop.
         mTimerCount++;
         continue;
      }

      // Calculate the time durations.
      mStatJitterTimeUs = mStatBeginTimeUs - mStatLastBeginTimeUs - tTimerPeriodUs;
      mStatLastBeginTimeUs = mStatBeginTimeUs;
      mStatExecTimeUs = mStatEndTimeUs - mStatBeginTimeUs;

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Calculate statistics on the times. 

      // Test for the start of trial.
      if (mStatTimerCount == 0)
      {
         // Start the statistics.
         mStatJitter.startTrial();
         mStatExec.startTrial();
      }

      // Update the statistics.
      mStatJitter.put(mStatJitterTimeUs);
      mStatExec.put(mStatExecTimeUs);

      // Test for end of trial.
      if (mStatTimerCount == mStatTimerCountMax - 1)
      {
         // Finish the statistics.
         mStatJitter.finishTrial();
         mStatExec.finishTrial();

         // Latch the results.
         mStatJitterMean = mStatJitter.mMean;
         mStatJitterStdDev = mStatJitter.mStdDev;
         mStatJitterMin = mStatJitter.mMinX;
         mStatJitterMax = mStatJitter.mMaxX;
         mStatJitterMaxMax = mStatJitter.mMaxMaxX;
         mStatExecMean = mStatExec.mMean;
         mStatExecStdDev = mStatExec.mStdDev;
         mStatExecMin = mStatExec.mMinX;
         mStatExecMax = mStatExec.mMaxX;
         mStatExecMaxMax = mStatExec.mMaxMaxX;

         // Increment for each trial evaluation.
         mStatCount++;

         // Set the poll flag.
         mStatPollFlag = true;
      }

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Loop done.

      // Update the counter.
      mTimerCount++;

      // Update the counter.
      if (++mStatTimerCount == mStatTimerCountMax) mStatTimerCount = 0;

   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

