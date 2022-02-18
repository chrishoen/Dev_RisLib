//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <time.h>

#include "prnPrint.h"

#include "risThreadsRandomThread.h"

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

// Nanoseconds.
inline long long int get_Nanotime()
{
   struct timespec tTimespec;
   clock_gettime(CLOCK_MONOTONIC, &tTimespec);

   long long int tSeconds = (long long int)tTimespec.tv_sec;
   long long int tNanoseconds = (long long int)tTimespec.tv_nsec;
   long long int tTimeNs = tSeconds * 1000 * 1000 * 1000 + tNanoseconds;

   return tTimeNs;
}

// Nanoseconds from milliseconds.
inline long long  get_NsFromMs(int aTimeMs)
{
   return ((long long)aTimeMs) * (1000 * 1000);
}

// Nanoseconds from microseconds.
inline long long  get_NsFromUs(int aTimeMs)
{
   return ((long long)aTimeMs) * (1000);
}

// Microseconds from nanoseconds
double get_UsFromNs(long long  aTimeNs)
{
   return (double)(aTimeNs / 1000.0);
}

// Timespec from nanoseconds.
struct timespec  get_TimespecFromNs(long long aTimeNs)
{
   struct timespec tTimespec;
   tTimespec.tv_sec = (int)(aTimeNs / (1000 * 1000 * 1000));
   tTimespec.tv_nsec = (int)(aTimeNs % (1000 * 1000 * 1000));
   return tTimespec;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseRandomThread::BaseRandomThread()
{
   // Set base class variables.
   mTimerPeriodUs1 = 1000000;
   mTimerPeriodUs2 = 1000000;
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

void BaseRandomThread::threadRunFunction()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Initial value
   mTimerPeriodUs = (mTimerPeriodUs1 + mTimerPeriodUs2) / 2;

   // Initialize statistics variables.
   mStatTimerCountMax = (mStatPeriod * 1000) / mTimerPeriodUs;

   // Seed random generator.
   std::random_device tRandomDevice;
   mRandomGenerator.seed(tRandomDevice());
   mRandomDistribution = std::uniform_int_distribution<>(mTimerPeriodUs1, mTimerPeriodUs2);

   // Time variables
   timespec   tSleepTimespec;
   long long  tSleepTimeNs = 0;
   long long  tTimerPeriodNs = 0;

   // Get current nanotime at start.
   tSleepTimeNs = get_Nanotime();
   mStatBeginTimeNs = tSleepTimeNs;

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
      
      // Get a random period.
      mTimerPeriodUs = mRandomDistribution(mRandomGenerator);
      tTimerPeriodNs = get_NsFromUs(mTimerPeriodUs);

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

