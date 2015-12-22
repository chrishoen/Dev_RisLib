/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <windows.h>
#include <atomic>

#include "prnPrint.h"
#include "risCriticalSection.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TimerThread::TimerThread()
{
   // Set base class thread priority
   BaseClass::setThreadPriorityHigh();

   int tFrequency = 1;
   int tPeriod = 1000 / tFrequency;

   // Set timer period
   BaseClass::mTimerPeriod = tPeriod;

   mTimeMarker.initialize(tFrequency);

   // Members
   mTPFlag = false;
   mTestCode = 5;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeOnTimer(int aTimeCount)
{
   switch (mTestCode)
   {
      case 0: executeTest0 (aTimeCount); break;
      case 1: executeTest1 (aTimeCount); break;
      case 2: executeTest2 (aTimeCount); break;
      case 3: executeTest3 (aTimeCount); break;
      case 4: executeTest4 (aTimeCount); break;
      case 5: executeTest5 (aTimeCount); break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest1(int aTimeCount)
{
   int tCount = 10000;

   mTimeMarker.initialize(tCount);

   while(true)
   {
      mTimeMarker.doStart();
      mTimeMarker.doStop();

      if (mTimeMarker.mStatistics.mEndOfPeriod)
      {
         break;
      }
   }

   Prn::print(Prn::ThreadRun1, "TEST1 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
      mTimeMarker.mChangeCount,
      mTimeMarker.mStatistics.mMean,
      mTimeMarker.mStatistics.mStdDev,
      mTimeMarker.mStatistics.mMinX,
      mTimeMarker.mStatistics.mMaxX);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest2(int aTimeCount)
{
   int tCount = 10000;

   Ris::Threads::MutexSemaphore tMutex;

   mTimeMarker.initialize(tCount);

   while(true)
   {
      mTimeMarker.doStart();

      tMutex.lock();
      tMutex.unlock();

      mTimeMarker.doStop();

      if (mTimeMarker.mStatistics.mEndOfPeriod)
      {
         break;
      }
   }

   Prn::print(Prn::ThreadRun1, "TEST2 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
      aTimeCount,
      mTimeMarker.mStatistics.mMean,
      mTimeMarker.mStatistics.mStdDev,
      mTimeMarker.mStatistics.mMinX,
      mTimeMarker.mStatistics.mMaxX);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest3(int aTimeCount)
{
   int tCount = 10000;

   mTimeMarker.initialize(tCount);

   while(true)
   {
      mTimeMarker.doStart();

      Ris::enterCriticalSection();
      Ris::leaveCriticalSection();

      mTimeMarker.doStop();

      if (mTimeMarker.mStatistics.mEndOfPeriod)
      {
         break;
      }
   }

   Prn::print(Prn::ThreadRun1, "TEST3 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
      aTimeCount,
      mTimeMarker.mStatistics.mMean,
      mTimeMarker.mStatistics.mStdDev,
      mTimeMarker.mStatistics.mMinX,
      mTimeMarker.mStatistics.mMaxX);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest4(int aTimeCount)
{
   int tCount = 10000;

   mTimeMarker.initialize(tCount);

   std::atomic_int tAtom;

   while(true)
   {
      mTimeMarker.doStart();

      std::atomic_fetch_add (&tAtom,1);

      mTimeMarker.doStop();

      if (mTimeMarker.mStatistics.mEndOfPeriod)
      {
         break;
      }
   }

   Prn::print(Prn::ThreadRun1, "TEST4 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
      aTimeCount,
      mTimeMarker.mStatistics.mMean,
      mTimeMarker.mStatistics.mStdDev,
      mTimeMarker.mStatistics.mMinX,
      mTimeMarker.mStatistics.mMaxX);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest5(int aTimeCount)
{
   int tCount = 10000;

   mTimeMarker.initialize(tCount);

   while(true)
   {
      mTimeMarker.doStart();

      void* tPtr = malloc(1000);
      free(tPtr);

      mTimeMarker.doStop();

      if (mTimeMarker.mStatistics.mEndOfPeriod)
      {
         break;
      }
   }

   Prn::print(Prn::ThreadRun1, "TEST5 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
      aTimeCount,
      mTimeMarker.mStatistics.mMean,
      mTimeMarker.mStatistics.mStdDev,
      mTimeMarker.mStatistics.mMinX,
      mTimeMarker.mStatistics.mMaxX);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread::executeTest0(int aTimeCount)
{
   mTimeMarker.doStop();
   mTimeMarker.doStart();

   if (aTimeCount == 0)
   {
      int tPrioriyClass = GetPriorityClass(GetCurrentProcess());
      int tThreadPriority = BaseThread::getThreadPriority();
      Prn::print(Prn::ThreadRun1, "TimerThread::executeOnTimer THREAD %08X %d", tPrioriyClass, tThreadPriority);
   }

   if (!mTPFlag) return;

   if (mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun1, " TEST0 %10d %10d  %10.3f  %10.3f  %10.3f  %10.3f",
         aTimeCount,
         mTimeMarker.mChangeCount,
         mTimeMarker.mStatistics.mMean/1000.0,
         mTimeMarker.mStatistics.mStdDev/1000.0,
         mTimeMarker.mStatistics.mMinX/1000.0,
         mTimeMarker.mStatistics.mMaxX/1000.0);
   }
}

}//namespace