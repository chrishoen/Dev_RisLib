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
   int tFrequency = 10;
   int tPeriod = 1000 / tFrequency;

   BaseClass::mTimerPeriod = tPeriod;

   gShare.mTimeMarker.initialize(5*tFrequency);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void TimerThread::executeOnTimer(int aTimeCount)
{
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