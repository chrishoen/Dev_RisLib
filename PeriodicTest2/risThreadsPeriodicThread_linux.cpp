//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <time.h>

#include "prnPrint.h"
#include "risNanoConvert.h"
#include "risProgramTime.h"

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
   mTimerPeriod = 1000;
   mTimerCount = 0;
   mTerminateFlag = false;

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

   // Time variables
   timespec   tSleepTimespec;
   long long  tSleepTimeNs;
   long long  tTimerPeriodNs = Ris::NanoConvert::getNsFromMs(mTimerPeriod);
   double     tTimerPeriodUs = tTimerPeriodNs / 1000.0;

   // Get current timespec at start.
   clock_gettime(CLOCK_MONOTONIC, &tSleepTimespec);

   // Convert to ns.
   tSleepTimeNs = Ris::NanoConvert::getNsFromTimespec(&tSleepTimespec);

   // initialize statistics variables.
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
      
      // Advance the sleep time by the period.
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
      executeOnTimer(mTimerCount++);

      // Get the time after timer handler execution.
      mStatEndTimeUs = getCurrentProgramTimeUS();;

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Calculate statistics on the times. 

      // Timer and count flags.
      bool tFirstFlag = mTimerCount == 0;
      bool tStartTrial = mStatTimerCount == 0;
      bool tEndTrial = mStatTimerCount == mStatTimerCount - 1;
      if(++mStatTimerCount== mStatTimerCountMax) mStatTimerCount = 0;

      // Calculate the time durations.
      if (tFirstFlag)
      {
         mStatLastBeginTimeUs = mStatBeginTimeUs;
      }
      else
      {
         mStatJitterTimeUs = mStatBeginTimeUs - mStatLastBeginTimeUs;
         mStatLastBeginTimeUs = mStatBeginTimeUs;
         mStatExecTimeUs = mStatEndTimeUs - mStatBeginTimeUs;
      }

      // Test for start of trial.
      if (tStartTrial)
      {
         // Start the statistics.
         mStatJitter.startTrial();
         mStatExec.startTrial();

         // Update the statistics.
         mStatExec.put(mStatExecTimeUs);
      }
      else
      {
         // Update the statistics.
         mStatJitter.put(mStatJitterTimeUs);
         mStatExec.put(mStatExecTimeUs);
      }

      // Test for end of trial.
      if (tEndTrial)
      {
         // Finish the statistics.
         mStatJitter.finishTrial();
         mStatExec.finishTrial();

         // Latch the results.
         mStatJitterMean = mStatJitter.mMean;
         mStatJitterStdDev = mStatJitter.mStdDev;
         mStatJitterMin = mStatJitter.mMinX;
         mStatJitterMax = mStatJitter.mMaxX;
         mStatExecMean = mStatExec.mMean;
         mStatExecStdDev = mStatExec.mStdDev;
         mStatExecMin = mStatExec.mMinX;
         mStatExecMax = mStatExec.mMaxX;

         // Set the poll flag.
         mStatPollFlag = true;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

