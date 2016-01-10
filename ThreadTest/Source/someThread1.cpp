/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "GSettings.h"
#include "someShare.h"
#include "someClass1.h"
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
      {
         mBinarySem.get();
         gShare.mTimeMarker.doStop();
      }
      break;
      case 2:
      {
         mCountingSem.get();
         gShare.mTimeMarker.doStop();
      }
      break;
      case 3:
      {
         mCountingSem.get();
         Class1A* tObject = (Class1A*)gShare.mPointerQueue.readPtr();
         gShare.mTimeMarker.doStop();
      }
      break;
      case 4:
      {
         mCountingSem.get();
         Class1A* tObject = (Class1A*)gShare.mPointerQueue.readPtr();
         if (tObject) delete tObject;
         gShare.mTimeMarker.doStop();
      }
      break;
      default:
         threadSleep(200);
         break;
      }
   }
}


}//namespace