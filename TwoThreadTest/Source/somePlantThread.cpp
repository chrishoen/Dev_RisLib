/*==============================================================================
File: someTest1Thread.cpp
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "someControllerThread.h"

#define  _SOMEPLANTTHREAD_CPP_
#include "somePlantThread.h"

namespace Some
{

PlantThread::PlantThread()
{
   // QCall CallPointers
   mCommandQCall.bind (this,&PlantThread::executeCommand);
}

//******************************************************************************
// This sets base thread configuration members

void PlantThread::configureThreads()
{
   // Set base class configuration members to defaults
   BaseClass::configureThread();

   // Set base class thread priority
   BaseClass::setThreadPriorityHigh();

   // Set timer period
   BaseClass::mTimerPeriod = 1000;
}

void PlantThread::threadInitFunction()
{
   BaseClass::threadInitFunction();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

void PlantThread::executeOnTimer(int aCurrentTimeCount)
{
}

//******************************************************************************

void PlantThread::executeCommand(int aId,int aParm1,int aParm2)
{
   Prn::print(0,0,"PlantThread::executeCommand             %04d",aId);

   switch(aId)
   {
   case 1 :
      {
         wait(aParm1);
         gControllerThread->mResponseQCall(1);
      }
      break;
   case 2 :
      {
         wait(aParm1);
         gControllerThread->mResponseQCall(2);
      }
      break;
   case 3 :
      {
         wait(aParm1);
         gControllerThread->mResponseQCall(3);
         wait(aParm1);
         gControllerThread->mResponseQCall(4);
      }
      break;
   case 5 :
      {
         wait(aParm1);
         gControllerThread->mResponseQCall(5);
         wait(aParm1);
         gControllerThread->mResponseQCall(6);
      }
      break;
   case 7 :
      {
         wait(aParm1);
         gControllerThread->mResponseQCall(7);
         wait(aParm1);
         gControllerThread->mResponseQCall(8);
      }
      break;
   case 9 :
      {
         wait(aParm1);
         gControllerThread->mResponseQCall(9);
         wait(aParm1);
         gControllerThread->mResponseQCall(10);
      }
      break;
   }
}

//******************************************************************************

void PlantThread::wait(int aSeconds)
{
   if (aSeconds==0)return;
   BaseClass::threadSleep(aSeconds*1000);
}

//******************************************************************************

}//namespace