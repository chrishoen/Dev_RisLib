/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "GSettings.h"
#include "someShare.h"
#include "Experiment.h"

#define  _SOMETHREAD1_CPP_
#include "someThread1.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

Thread1::Thread1()
{
   // Set base class thread priority
   BaseClass::setThreadPriorityHigh();
   BaseClass::mThreadAffinityMask = 0x20;
   BaseClass::mThreadIdealProcessor = 5;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::threadRunFunction()
{
   while (true)
   {
      switch (gGSettings.mTestNumber)
      {
      case 1:
         mThreadSem.get();
         gShare.mTimeMarker.doStop();
         break;
      case 2:
         mConditionVariable.waitFor();
         gShare.mTimeMarker.doStop();
         break;
      case 3:
         Experiment::receive();
         gShare.mTimeMarker.doStop();
         break;
      default:
         threadSleep(200);
         break;
      }
   }
}


}//namespace