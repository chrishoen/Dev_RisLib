/*==============================================================================
File: someTest1Thread.cpp
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "someThread1.h"

#define  _SOMETHREAD2_CPP_
#include "someThread2.h"

namespace Some
{

Thread2::Thread2()
{
   // QCall CallPointers
   mC101QCall.bind  (this,&Thread2::executeC101);
   mC102QCall.bind  (this,&Thread2::executeC102);
   mTest1QCall.bind (this,&Thread2::executeTest1);
}

void Thread2::threadInitFunction()
{
   mTimerPeriod=1000;
   BaseClass::threadInitFunction();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread2::executeOnTimer(int aCurrentTimeCount)
{
   return;
   if ((aCurrentTimeCount + 1)%100==0)
   {
      gThread1->mF101QCall.invoke(aCurrentTimeCount);
   }

   if ((aCurrentTimeCount + 2)%100==0)
   {
      gThread1->mF102QCall.invoke(aCurrentTimeCount);
   }
}

//******************************************************************************

void Thread2::executeC101(int aN)
{
   Prn::print(0,"Thread2::executeC101 %d",aN);
}

//******************************************************************************

void Thread2::executeC102(int aN)
{
   Prn::print(0,"Thread2::executeC102 %d",aN);
}

//******************************************************************************

void Thread2::executeTest1(int aN)
{
   Prn::print(0,"Thread1::executeTest1 BEGIN %d",aN);
   BaseClass::threadSleep(1000*aN);
   Prn::print(0,"Thread1::executeTest1 END %d",aN);
}

}//namespace