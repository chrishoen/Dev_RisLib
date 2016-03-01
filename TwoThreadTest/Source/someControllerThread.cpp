/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "somePlantThread.h"

#define  _SOMECONTROLLERTHREAD_CPP_
#include "someControllerThread.h"

namespace Some
{

ControllerThread::ControllerThread()
{
   // Set base class thread priority
   BaseClass::mShortThread->setThreadPriorityHigh();

   // Set timer period
   BaseClass::mShortThread->mTimerPeriod = 1000;

   // Base class call pointers
   BaseClass::mShortThread->mThreadInitCallPointer.bind(this,&ControllerThread::threadInitFunction);
   BaseClass::mShortThread->mThreadExitCallPointer.bind(this,&ControllerThread::threadExitFunction);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer.bind(this,&ControllerThread::executeOnTimer);

   // QCall CallPointers
   mTest1QCall.bind     (this->mLongThread, this,&ControllerThread::executeTest1);
   mTest2QCall.bind     (this->mLongThread, this,&ControllerThread::executeTest2);
   mSendQCall.bind      (this->mLongThread, this,&ControllerThread::executeSend);
   mResponseQCall.bind  (this->mShortThread,this,&ControllerThread::executeResponse);
   mSequenceQCall.bind  (this->mLongThread, this,&ControllerThread::executeSequence);

   // Members
   mTPFlag = false;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void ControllerThread::threadInitFunction()
{
   Prn::print(0,"ControllerThread::threadInitFunction");
}

void ControllerThread::threadExitFunction()
{
   Prn::print(0,"ControllerThread::threadExitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void ControllerThread::executeOnTimer(int aTimerCount)
{
   if (!mTPFlag) return;
// if (aTimerCount % 100 !=0) return;
   Prn::print(0,"CTimer %6d",aTimerCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

void ControllerThread::executeTest1(int aN)
{
   Prn::print(0,"ControllerThread::executeTest1 BEGIN %04d",aN);
   try
   {
      waitForTimer(aN);
   }
   catch(int aStatus)
   {
      Prn::print(0, "Exception ControllerThread::executeTest1 ABORTED  %d",aStatus);
   }
   Prn::print(0,"ControllerThread::executeTest1 END   %04d",aN);
}

//******************************************************************************

void ControllerThread::executeTest2(int aN)
{
   Prn::print(0,"ControllerThread::executeTest2 BEGIN %04d",aN);
   try
   {
      BaseClass::resetNotify();
      BaseClass::waitForNotify(aN,1);
   }
   catch(int aStatus)
   {
      Prn::print(0, "Exception ControllerThread::executeTest2 ABORTED  %d",aStatus);
   }
   Prn::print(0,"ControllerThread::executeTest2 END   %04d",aN);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ControllerThread::executeSend(int aId,int aParm1,int aParm2)
{
   try
   {
      Prn::print(0,"ControllerThread::executeSend BEGIN     %04d",aId);

      BaseClass::resetNotify();
      gPlantThread->mCommandQCall(aId,aParm1,aParm2);
      BaseClass::waitForNotify(-1,aId);

      Prn::print(0,"ControllerThread::executeSend END       %04d",aId);
   }
   catch(int aStatus)
   {
      Prn::print(0, "Exception ControllerThread::executeSend ABORTED  %d",aStatus);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ControllerThread::executeSequence(int aId,int aIterations,int aCommandTimeout,int aResponseDelay)
{
   try
   {
      Prn::print(0,"ControllerThread::executeSequence BEGIN %04d",aId);

      switch(aId)
      {
      case 1 :
         {
            BaseClass::resetNotify();
            gPlantThread->mCommandQCall(1,aResponseDelay,0);
            BaseClass::waitForNotify(aCommandTimeout,1);
         }
         break;
      case 2 :
         {
            BaseClass::resetNotify();
            gPlantThread->mCommandQCall(2,aResponseDelay,0);
            BaseClass::waitForNotify(aCommandTimeout,2);
         }
         break;
      case 3 :
         {
            BaseClass::resetNotify();
            gPlantThread->mCommandQCall(3,aResponseDelay,0);
            BaseClass::waitForNotifyAny(aCommandTimeout,2,3,4);
         }
         break;
      case 5 :
         {
            BaseClass::resetNotify();
            gPlantThread->mCommandQCall(5,aResponseDelay,0);
            BaseClass::waitForNotifyAll(aCommandTimeout,2,5,6);
         }
         break;
      case 7 :
         {
            BaseClass::resetNotify();
            gPlantThread->mCommandQCall(7,aResponseDelay,0);
            BaseClass::waitForNotifyAny(aCommandTimeout,2,7,8);
         }
         break;
      case 9 :
         {
            BaseClass::resetNotify();
            gPlantThread->mCommandQCall(9,aResponseDelay,0);
            BaseClass::waitForNotifyAll(aCommandTimeout,2,9,10);
         }
         break;
      case 105 :
         {
            for (int i=0;i<aIterations;i++)
            {
               BaseClass::resetNotify();
               gPlantThread->mCommandQCall(5,2,0);
               BaseClass::waitForNotifyAll(10,2,5,6);
            }
         }
         break;
      case 109 :
         {
            for (int i=0;i<aIterations;i++)
            {
               Prn::print(0,"ControllerThread::executeSequence LOOP BEGIN %04d",i);
               BaseClass::resetNotify();
               gPlantThread->mCommandQCall(9,aResponseDelay,0);
               BaseClass::waitForNotifyAll(aCommandTimeout,2,9,10);
               Prn::print(0,"ControllerThread::executeSequence LOOP END   %04d",i);
            }
         }
         break;
      }

      Prn::print(0,"ControllerThread::executeSequence END   %04d",aId);
   }
   catch(int aStatus)
   {
      Prn::print(0, "Exception ControllerThread::executeSequence ABORTED  %d",aStatus);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ControllerThread::executeResponse(int aId)
{
   Prn::print(0,"ControllerThread::executeResponse       %04d",aId);

   BaseClass::notify(aId);
}


}//namespace