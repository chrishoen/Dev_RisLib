/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "someThread2.h"

#define  _SOMETHREAD1_CPP_
#include "someThread1.h"

namespace Some
{

Thread1::Thread1()
{
   // QCall CallPointers
   mF101QCall.bind  (this,&Thread1::executeF101);
   mF102QCall.bind  (this,&Thread1::executeF102);
   mTest1QCall.bind (this,&Thread1::executeTest1);
   mTest2QCall.bind (this,&Thread1::executeTest2);
   mTest3QCall.bind (this,&Thread1::executeTest3);
   mTest4QCall.bind (this,&Thread1::executeTest4);

   // Members
   mTPFlag=true;
}

//******************************************************************************
// This sets base thread configuration members

void Thread1::configureThread()
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
void Thread1::executeOnTimer(int aTimerCount)
{
   mTimerTester.executeOnTimer();
   if (mTPFlag) Prn::print(0,0,"Timer %04d",aTimerCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::executeF101(int aN)
{
   gThread2->mC101QCall.invoke(101);
}

//******************************************************************************

void Thread1::executeF102(int aN)
{
   gThread2->mC102QCall.invoke(102);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::executeTest1(int aN)
{
   Prn::print(0,0,"Thread1::executeTest1 BEGIN");
   threadWaitForTimerCompletion(4);
   Prn::print(0,0,"Thread1::executeTest1 END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::executeTest2(int aN)
{
   try
   {
      Prn::print(0,0,"Thread1::executeTest2 BEGIN");
      threadWaitForTimerCompletion(20);
      Prn::print(0,0,"Thread1::executeTest2 END");
   }
   catch(int aStatus)
   {
      Prn::print(0,0, "Exception Thread1::executeTest2 ABORTED  %d",aStatus);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::executeTest3(int aN)
{
   try
   {
      Prn::print(0,0,"Thread1::executeTest3 BEGIN");
      for (int i=0;i<20;i++)
      {
         threadWaitForTimerCompletion(1);
      }
      Prn::print(0,0,"Thread1::executeTest3 END");
   }
   catch(int aStatus)
   {
      Prn::print(0,0, "Exception Thread1::executeTest3 ABORTED  %d",aStatus);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::executeTest4(int aNumOfIterations,int aNumOfExecutions)
{
   mTimerTester.startTest(mTimerPeriod,aNumOfIterations,aNumOfExecutions);
}

}//namespace