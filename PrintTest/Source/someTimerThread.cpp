/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#define  _SOMETIMERTHREAD_CPP_
#include "someTimerThread.h"

namespace Some
{

TimerThread::TimerThread()
{
   // Set base class thread priority
   BaseClass::setThreadPriorityHigh();

   // Set timer period
   BaseClass::mTimerPeriod = 1000;

   // Members
   mTPFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void TimerThread::executeOnTimer(int aTimeCount)
{
   if (mTPFlag)
   {
      Prn::print(Prn::ThreadRun1, "TTTimer %04d", aTimeCount);
      Prn::print(Prn::ViewRun1,   "TTTimer %04d", aTimeCount);
   }
}

}//namespace