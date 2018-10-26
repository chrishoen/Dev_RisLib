/*==============================================================================
File: someTest1Thread.cpp
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "someMasterThread.h"

#define  _SOMESLAVETHREAD_CPP_
#include "someSlaveThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SlaveThread::SlaveThread()
{
   // Set base class thread variables.
   BaseClass::setThreadPriorityHigh();
   BaseClass::mTimerPeriod = 1000;

   // Set qcalls.
   mCommandQCall.bind (this,&SlaveThread::executeCommand);
   mWorkRequestQCall.bind (this,&SlaveThread::executeWorkRequest);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SlaveThread::executeOnTimer(int aCurrentTimeCount)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SlaveThread::executeCommand(int aId,int aParm1,int aParm2)
{
   Prn::print(0,"SlaveThread::executeCommand             %04d",aId);

   switch(aId)
   {
   case 1 :
      {
         wait(aParm1);
         gMasterThread->mResponseQCall(1);
      }
      break;
   case 2 :
      {
         wait(aParm1);
         gMasterThread->mResponseQCall(2);
      }
      break;
   case 3 :
      {
         wait(aParm1);
         gMasterThread->mResponseQCall(3);
         wait(aParm1);
         gMasterThread->mResponseQCall(4);
      }
      break;
   case 5 :
      {
         wait(aParm1);
         gMasterThread->mResponseQCall(5);
         wait(aParm1);
         gMasterThread->mResponseQCall(6);
      }
      break;
   case 7 :
      {
         wait(aParm1);
         gMasterThread->mResponseQCall(7);
         wait(aParm1);
         gMasterThread->mResponseQCall(8);
      }
      break;
   case 9 :
      {
         wait(aParm1);
         gMasterThread->mResponseQCall(9);
         wait(aParm1);
         gMasterThread->mResponseQCall(10);
      }
      break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SlaveThread::wait(int aSeconds)
{
   if (aSeconds==0)return;
   BaseClass::threadSleep(aSeconds*1000);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SlaveThread::executeWorkRequest (int aParm1,Ris::Threads::TwoThreadNotify aCompletionNotify)
{
   Prn::print(0,"SlaveThread::executeWorkRequest                %04d",aParm1);

   wait(aParm1/1000);

   aCompletionNotify.mStatus = 0;
   aCompletionNotify.notify();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace