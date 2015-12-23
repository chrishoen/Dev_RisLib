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
// BaseClass::setThreadPriorityHigh();

   // Set timer period
   int tFrequency = 100;
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
// gThread1->mThreadSem.put();
// gThread1->mConditionVariable.wakeUp();
   Experiment::send();

// Prn::print(Prn::ThreadRun1, "Timer1");
}

}//namespace