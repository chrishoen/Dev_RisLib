/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "someSlaveThread.h"

#define  _SOMEMASTERTHREAD_CPP_
#include "someMasterThread.h"

namespace Some
{

MasterThread::MasterThread()
{
   // Set base class thread priority
   BaseClass::mShortThread->setThreadPriorityHigh();

   // Set timer period
   BaseClass::mShortThread->mTimerPeriod = 1000;

   // Base class call pointers
   BaseClass::mShortThread->mThreadInitCallPointer.bind(this,&MasterThread::threadInitFunction);
   BaseClass::mShortThread->mThreadExitCallPointer.bind(this,&MasterThread::threadExitFunction);
   BaseClass::mShortThread->mThreadExecuteOnTimerCallPointer.bind(this,&MasterThread::executeOnTimer);

   // QCall CallPointers
   mTest1QCall.bind            (this->mLongThread, this,&MasterThread::executeTest1);
   mTest2QCall.bind            (this->mLongThread, this,&MasterThread::executeTest2);
   mSendCommandQCall.bind      (this->mLongThread, this,&MasterThread::executeSendCommand);
   mSendWorkRequestQCall.bind  (this->mLongThread, this,&MasterThread::executeSendWorkRequest);
   mResponseQCall.bind         (this->mShortThread,this,&MasterThread::executeResponse);
   mSequenceQCall.bind         (this->mLongThread, this,&MasterThread::executeSequence);

   // Members
   mTPFlag = false;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void MasterThread::threadInitFunction()
{
   Prn::print(0,"MasterThread::threadInitFunction");
}

void MasterThread::threadExitFunction()
{
   Prn::print(0,"MasterThread::threadExitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void MasterThread::executeOnTimer(int aTimerCount)
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

void MasterThread::executeTest1(int aN)
{
   Prn::print(0,"MasterThread::executeTest1 BEGIN %04d",aN);
   try
   {
      waitForTimer(aN);
   }
   catch(int aStatus)
   {
      Prn::print(0, "Exception MasterThread::executeTest1 ABORTED  %d",aStatus);
   }
   Prn::print(0,"MasterThread::executeTest1 END   %04d",aN);
}

//******************************************************************************

void MasterThread::executeTest2(int aN)
{
   Prn::print(0,"MasterThread::executeTest2 BEGIN %04d",aN);
   try
   {
      BaseClass::resetNotify();
      BaseClass::waitForNotify(aN,1);
   }
   catch(int aStatus)
   {
      Prn::print(0, "Exception MasterThread::executeTest2 ABORTED  %d",aStatus);
   }
   Prn::print(0,"MasterThread::executeTest2 END   %04d",aN);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void MasterThread::executeSendCommand(int aId,int aParm1,int aParm2)
{
   try
   {
      Prn::print(0,"MasterThread::executeSendCommand BEGIN     %04d",aId);

      BaseClass::resetNotify();
      gSlaveThread->mCommandQCall(aId,aParm1,aParm2);
      BaseClass::waitForNotify(-1,aId);

      Prn::print(0,"MasterThread::executeSendCommand END       %04d",aId);
   }
   catch(int aStatus)
   {
      Prn::print(0, "Exception MasterThread::executeSendCommand ABORTED  %d",aStatus);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void MasterThread::executeSendWorkRequest(int aParm1,int aParm2)
{
   try
   {
      Prn::print(0,"MasterThread::executeSendWorkRequest BEGIN     %04d",aParm1);

      BaseClass::resetNotify();
      Ris::Threads::TwoThreadNotify tNotify(this,1);
      gSlaveThread->mWorkRequestQCall(aParm1,tNotify);
      BaseClass::waitForTimer(aParm2);
      BaseClass::waitForNotify(-1,1);

      Prn::print(0,"MasterThread::executeSendWorkRequest END       %04d",aParm1);
   }
   catch(int aStatus)
   {
      Prn::print(0, "Exception MasterThread::executeSendWorkRequest ABORTED  %d",aStatus);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void MasterThread::executeSequence(int aId,int aIterations,int aCommandTimeout,int aResponseDelay)
{
   try
   {
      Prn::print(0,"MasterThread::executeSequence BEGIN %04d",aId);

      switch(aId)
      {
      case 1 :
         {
            BaseClass::resetNotify();
            gSlaveThread->mCommandQCall(1,aResponseDelay,0);
            BaseClass::waitForNotify(aCommandTimeout,1);
         }
         break;
      case 2 :
         {
            BaseClass::resetNotify();
            gSlaveThread->mCommandQCall(2,aResponseDelay,0);
            BaseClass::waitForNotify(aCommandTimeout,2);
         }
         break;
      case 3 :
         {
            BaseClass::resetNotify();
            gSlaveThread->mCommandQCall(3,aResponseDelay,0);
            BaseClass::waitForNotifyAny(aCommandTimeout,2,3,4);
         }
         break;
      case 5 :
         {
            BaseClass::resetNotify();
            gSlaveThread->mCommandQCall(5,aResponseDelay,0);
            BaseClass::waitForNotifyAll(aCommandTimeout,2,5,6);
         }
         break;
      case 7 :
         {
            BaseClass::resetNotify();
            gSlaveThread->mCommandQCall(7,aResponseDelay,0);
            BaseClass::waitForNotifyAny(aCommandTimeout,2,7,8);
         }
         break;
      case 9 :
         {
            BaseClass::resetNotify();
            gSlaveThread->mCommandQCall(9,aResponseDelay,0);
            BaseClass::waitForNotifyAll(aCommandTimeout,2,9,10);
         }
         break;
      case 105 :
         {
            for (int i=0;i<aIterations;i++)
            {
               BaseClass::resetNotify();
               gSlaveThread->mCommandQCall(5,2,0);
               BaseClass::waitForNotifyAll(10,2,5,6);
            }
         }
         break;
      case 109 :
         {
            for (int i=0;i<aIterations;i++)
            {
               Prn::print(0,"MasterThread::executeSequence LOOP BEGIN %04d",i);
               BaseClass::resetNotify();
               gSlaveThread->mCommandQCall(9,aResponseDelay,0);
               BaseClass::waitForNotifyAll(aCommandTimeout,2,9,10);
               Prn::print(0,"MasterThread::executeSequence LOOP END   %04d",i);
            }
         }
         break;
      }

      Prn::print(0,"MasterThread::executeSequence END   %04d",aId);
   }
   catch(int aStatus)
   {
      Prn::print(0, "Exception MasterThread::executeSequence ABORTED  %d",aStatus);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void MasterThread::executeResponse(int aId)
{
   Prn::print(0,"MasterThread::executeResponse       %04d",aId);

   BaseClass::notify(aId);
}


}//namespace