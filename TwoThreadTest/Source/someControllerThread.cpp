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
   mTPFlag=true;
}

//******************************************************************************
// This sets base thread configuration members

void ControllerThread::configureThreads()
{
   // Set base class configuration members to defaults
   BaseClass::configureThreads();

   // Set base class thread priority
   BaseClass::mShortThread->setThreadPriorityHigh();

   // Set timer period
   BaseClass::mShortThread->mTimerPeriod = 1000;
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
   if (mTPFlag) Prn::print(0,"Timer %04d",aTimerCount);
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
      Prn::print(0,0, "Exception ControllerThread::executeTest1 ABORTED  %d",aStatus);
   }
   Prn::print(0,"ControllerThread::executeTest1 END   %04d",aN);
}

//******************************************************************************

void ControllerThread::executeTest2(int aN)
{
   Prn::print(0,"ControllerThread::executeTest2 BEGIN %04d",aN);
   try
   {
      waitForNotify(aN);
   }
   catch(int aStatus)
   {
      Prn::print(0,0, "Exception ControllerThread::executeTest2 ABORTED  %d",aStatus);
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

      gPlantThread->mCommandQCall(aId,aParm1,aParm2);
      BaseClass::waitForNotify();

      Prn::print(0,"ControllerThread::executeSend END       %04d",aId);
   }
   catch(int aStatus)
   {
      Prn::print(0,0, "Exception ControllerThread::executeSend ABORTED  %d",aStatus);
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
            gPlantThread->mCommandQCall(1,aResponseDelay,0);
            BaseClass::waitForNotify(aCommandTimeout);
         }
         break;
      case 2 :
         {
            gPlantThread->mCommandQCall(2,aResponseDelay,0);
            BaseClass::mNotifyLatch.reset();
            BaseClass::mNotifyLatch.setMask(2,true);
            BaseClass::waitForNotifyAny(aCommandTimeout);
         }
         break;
      case 3 :
         {
            gPlantThread->mCommandQCall(3,aResponseDelay,0);
            BaseClass::mNotifyLatch.reset();
            BaseClass::mNotifyLatch.setMask(3,true);
            BaseClass::mNotifyLatch.setMask(4,true);
            BaseClass::waitForNotifyAny(aCommandTimeout);
         }
         break;
      case 5 :
         {
            gPlantThread->mCommandQCall(5,aResponseDelay,0);
            BaseClass::mNotifyLatch.reset();
            BaseClass::mNotifyLatch.setMask(5,true);
            BaseClass::mNotifyLatch.setMask(6,true);
            BaseClass::waitForNotifyAll(aCommandTimeout);
         }
         break;
      case 7 :
         {
            gPlantThread->mCommandQCall(7,aResponseDelay,0);
            BaseClass::waitForNotifyAny(aCommandTimeout,2,7,8);
         }
         break;
      case 9 :
         {
            gPlantThread->mCommandQCall(9,aResponseDelay,0);
            BaseClass::waitForNotifyAll(aCommandTimeout,2,9,10);
         }
         break;
      case 105 :
         {
            for (int i=0;i<aIterations;i++)
            {
               gPlantThread->mCommandQCall(5,2,0);
               BaseClass::mNotifyLatch.reset();
               BaseClass::mNotifyLatch.setMask(5,true);
               BaseClass::mNotifyLatch.setMask(6,true);
               BaseClass::waitForNotifyAll(10);
            }
         }
         break;
      case 109 :
         {
            for (int i=0;i<aIterations;i++)
            {
               Prn::print(0,"ControllerThread::executeSequence LOOP BEGIN %04d",i);
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
      Prn::print(0,0, "Exception ControllerThread::executeSequence ABORTED  %d",aStatus);
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