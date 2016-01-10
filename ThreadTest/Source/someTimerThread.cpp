/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h>
#include "prnPrint.h"

#include "someShare.h"
#include "Experiment.h"
#include "GSettings.h"
#include "someApcThread.h"
#include "someQCallThread1.h"
#include "someThread1.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

TimerThread::TimerThread()
{
   if (gGSettings.mTestThread == GSettings::cTestThread_None)
   {
      BaseClass::setThreadPriorityHigh();
      BaseClass::mThreadAffinityMask = 0x20;
      BaseClass::mThreadIdealProcessor = 5;
      mTimerPeriod = 1000;
   }
   else
   {
      // Set timer period
      int mTimerFrequency = 20;
      BaseClass::mTimerPeriod = 1000 / mTimerFrequency;

      gShare.mTimeMarker.initialize(5 * mTimerFrequency);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeOnTimer(int aTimeCount)
{
   //---------------------------------------------------------------------------
   // Time marker

   if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun1, "TEST %1d %1d %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         gGSettings.mTestThread,
         gGSettings.mTestNumber,
         gShare.mTimeMarker.mStatistics.mSize,
         gShare.mTimeMarker.mStatistics.mMean,
         gShare.mTimeMarker.mStatistics.mStdDev,
         gShare.mTimeMarker.mStatistics.mMinX,
         gShare.mTimeMarker.mStatistics.mMaxX);
         gShare.mTimeMarker.mStatistics.mEndOfPeriod = false;
         return;
   }

   gShare.mTimeMarker.doStart();


   //---------------------------------------------------------------------------
   // Send to test thread

   switch (gGSettings.mTestThread)
   {

   case GSettings::cTestThread_None:
   {
      showThreadInfo(aTimeCount);
   }
   break;

   case GSettings::cTestThread_Thread1:
   {
      switch (gGSettings.mTestNumber)
      {
      case 1:
         gThread1->mBinarySem.put();
         break;
      case 2:
         gThread1->mCountingSem.put();
         break;
      case 3:
         gThread1->mConditionVariable.wakeUp();
         break;
      case 4:
         Experiment::send();
         break;
      }
   }
   break;

   case GSettings::cTestThread_ApcThread:
   {
      switch (gGSettings.mTestNumber)
      {
      case 1:
         gApcThread->enqueueApc();
         break;
      }
   }
   break;

   case GSettings::cTestThread_QCallThread1:
   {
      switch (gGSettings.mTestNumber)
      {
      case 1:
         gQCallThread1->mC101QCall.invoke(aTimeCount);
         break;
      }
   }
   break;

   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::showThreadInfo(int aTimeCount)
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