/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

TimerThread::TimerThread()
{
   // Members
   mTPFlag = false;
}

//******************************************************************************
// This sets base thread configuration members

void TimerThread::configureThread()
{
   // Set base class configuration members to defaults
   BaseClass::configureThread();

   // Set base class thread priority
   BaseClass::setThreadPriorityHigh();

   // Set timer period
   BaseClass::mTimerPeriod = 1000;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void TimerThread::executeOnTimer(int aTimeCount)
{
   if (mTPFlag) Prn::print(0,0,"TTTimer %04d",aTimeCount);
}

}//namespace