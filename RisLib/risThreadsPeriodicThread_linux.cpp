//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <time.h>

#include "risNanoTime.h"
#include "prnPrint.h"

#include "risThreadsPeriodicThread.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

BasePeriodicThread::BasePeriodicThread()
{
   // Set base class variables.
   mPeriodUs = 1000000;
   mPeriodMs = 0;
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
   mStatBeginTimeNs = 0;
   mStatLastBeginTimeNs = 0;
   mStatEndTimeNs = 0;
   mStatJitterTimeUs = 0;
   mStatExecTimeUs = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function. This is called by the base class immediately 
// after the thread init function. It runs a loop that waits for the
// timer or the termination event.

void BasePeriodicThread::threadRunFunction()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Override the period.
   if (mPeriodMs)
   {
      mPeriodUs = mPeriodMs * 1000;
   }

   // Time variables
   timespec   tSleepTimespec;
   long long  tSleepTimeNs;
   long long  tTimerPeriodNs = get_NsFromUs(mPeriodUs);
   double     tTimerPeriodUs = (double)mPeriodUs;
   // Get current nanotime at start.
   tSleepTimeNs = get_Nanotime();
   mStatBeginTimeNs = tSleepTimeNs;

   // Initialize statistics variables.
   mStatTimerCountMax = (mStatPeriod * 1000) / mPeriodUs;

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
      //tSleepTimeNs = mStatBeginTimeNs + tTimerPeriodNs;

      // Convert to timespec.
      tSleepTimespec = get_TimespecFromNs(tSleepTimeNs);

      // Sleep until the absolute sleep time.
      clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &tSleepTimespec, 0);

      // Guard.
      if (BaseClass::mTerminateFlag) break;

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Call the timer handler. 

      // Get the time before timer handler execution.
      mStatBeginTimeNs = get_Nanotime();

      // Call the inheritor's timer handler function.
      executeOnTimer(mTimerCount);

      // Test the poll processor flag and end of trial.
      if (mPollProcessor && mStatTimerCount == mStatTimerCountMax - 1)
      {
         // Get the current processor number.
         BaseClass::getThreadProcessorNumber();
      }

      // Get the time after timer handler execution.
      mStatEndTimeNs = get_Nanotime();

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Calculate the time durations. 

      // Test if this is the first time.
      if (mTimerCount == 0)
      {
         // Store the first begin time as the current last.
         mStatLastBeginTimeNs = mStatBeginTimeNs;

         // Ignore everything else in this loop.
         mTimerCount++;
         continue;
      }

      // Calculate the time durations.
    //mStatJitterTimeUs = get_UsFromNs(mStatBeginTimeNs - mStatLastBeginTimeNs - tTimerPeriodNs);
      mStatJitterTimeUs = get_UsFromNs(mStatBeginTimeNs - mStatLastBeginTimeNs);
      mStatExecTimeUs = get_UsFromNs(mStatEndTimeNs - mStatBeginTimeNs);
      mStatLastBeginTimeNs = mStatBeginTimeNs;

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

