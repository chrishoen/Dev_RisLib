/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "someShare.h"
#include "Experiment.h"
#include "someThread1.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

TimerThread::TimerThread()
{
   // Set base class thread priority

   // Set timer period
   int mTimerFrequency = 10;
   BaseClass::mTimerPeriod = 1000 / mTimerFrequency;

   gShare.mTimeMarker.initialize(5*mTimerFrequency);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void TimerThread::executeOnTimer(int aTimeCount)
{
   if (aTimeCount < mTimerFrequency) return;

   gShare.mTimeMarker.doStart();

   switch (gShare.mTest)
   {
   case 1:
      gThread1->mThreadSem.put();
      break;
   case 2:
      gThread1->mConditionVariable.wakeUp();
      break;
   case 3:
      Experiment::send();
      break;
   }
}

}//namespace