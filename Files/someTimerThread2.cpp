/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h>
#include "prnPrint.h"

#include "someShare.h"
#include "someClass1.h"
#include "Experiment.h"
#include "GSettings.h"

#define  _SOMETIMERTHREAD2_CPP_
#include "someTimerThread2.h"

namespace Some
{

TimerThread2::TimerThread2()
{
   // Set thread priority
   BaseClass::setThreadPriorityHigh();
   BaseClass::mThreadAffinityMask = 0x20;
   BaseClass::mThreadIdealProcessor = 5;

   // Set timer period
   int mTimerFrequency = 20;
   BaseClass::mTimerPeriod = 1000 / mTimerFrequency;

   gShare.mTimeMarker.initialize(5 * mTimerFrequency);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread2::executeOnTimer(int aTimeCount)
{
   //---------------------------------------------------------------------------
   // Time marker

   gShare.mTimeMarker.doStop();
   gShare.mTimeMarker.doStart();

   if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
   {
      double tPeriodUS = mTimerPeriod*1000.0;
      Prn::print(Prn::ThreadRun1, "TEST %1d %1d %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         gGSettings.mTestThread,
         gGSettings.mTestNumber,
         gShare.mTimeMarker.mStatistics.mSize,
         gShare.mTimeMarker.mStatistics.mMean   - tPeriodUS,
         gShare.mTimeMarker.mStatistics.mStdDev,
         gShare.mTimeMarker.mStatistics.mMinX   - tPeriodUS,
         gShare.mTimeMarker.mStatistics.mMaxX   - tPeriodUS);
         gShare.mTimeMarker.mStatistics.mEndOfPeriod = false;
         return;
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread2::showThreadInfo(int aTimeCount)
{
   Prn::print(Prn::ThreadRun1, "TimerThread INFO******************************** %d",aTimeCount);

   //---------------------------------------------------------------------------
   int tPriorityClass = GetPriorityClass(GetCurrentProcess());
   Prn::print(Prn::ThreadRun1, "GetPriorityClass %08X", tPriorityClass);

   //---------------------------------------------------------------------------
   unsigned long long tProcessAffinityMask=0;
   unsigned long long tSystemAffinityMask=0;
   GetProcessAffinityMask(
     GetCurrentProcess(),
     &tProcessAffinityMask,
     &tSystemAffinityMask);

   Prn::print(Prn::ThreadRun1, "GetProcessAffinityMask %llX  %llX",
     tProcessAffinityMask,
     tSystemAffinityMask);

   //---------------------------------------------------------------------------
   int tThreadPriority = GetThreadPriority(
      GetCurrentThread());
   Prn::print(Prn::ThreadRun1, "ThreadPriority   %8d", tThreadPriority);

   //---------------------------------------------------------------------------
   GROUP_AFFINITY tGroupAffinity;
   GetThreadGroupAffinity(
      GetCurrentThread(),
      &tGroupAffinity);
   Prn::print(Prn::ThreadRun1, "ThreadGroupAffinity   %d %llX ",
      tGroupAffinity.Group,
      tGroupAffinity.Mask);

   //---------------------------------------------------------------------------
   PROCESSOR_NUMBER tProcessorNumber;
   GetThreadIdealProcessorEx(
      GetCurrentThread(),
      &tProcessorNumber);

   Prn::print(Prn::ThreadRun1, "ThreadIdealProcessor  %d",
      tProcessorNumber.Number);



}

}//namespace