/*==============================================================================
File: someTest1Thread.cpp
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"


#include "someShare.h"

#define  _SOMEQCALLTHREAD1_CPP_
#include "someQCallThread1.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

QCallThread1::QCallThread1()
{
   // Set base class
   BaseClass::setThreadPriorityHigh();
   BaseClass::mThreadAffinityMask = 0x20;
   BaseClass::mThreadIdealProcessor = 5;
   BaseClass::mTimerPeriod=0;

   // QCall CallPointers
   mC101QCall.bind  (this,&QCallThread1::executeC101);
   mC102QCall.bind  (this,&QCallThread1::executeC102);

   // QCall CallPointers
   mF100QCall.bind  (this,&QCallThread1::executeF100);
   mF101QCall.bind  (this,&QCallThread1::executeF101);
   mF102QCall.bind  (this,&QCallThread1::executeF102);
   mF103QCall.bind  (this,&QCallThread1::executeF103);
   mF104QCall.bind  (this,&QCallThread1::executeF104);
   mF105QCall.bind  (this,&QCallThread1::executeF105);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void QCallThread1::executeOnTimer (int aTimeCount)
{
}

//******************************************************************************

void QCallThread1::executeC101(int aN)
{
   gShare.mTimeMarker.doStop();
}

//******************************************************************************

void QCallThread1::executeC102(int aN)
{
   Prn::print(0,"QCallThread1::executeC102 %d",aN);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void QCallThread1::executeF100 ()
{
   Prn::print(0,"QCallThread1::executeF100");
}

void QCallThread1::executeF101 (int aX1)
{
   Prn::print(0,"QCallThread1::executeF101 %d",aX1);
}

void QCallThread1::executeF102 (int aX1,double aX2)
{
   Prn::print(0,"QCallThread1::executeF102 %d %5.2f",aX1,aX2);
}

void QCallThread1::executeF103 (int aX1,double aX2,float aX3)
{
   Prn::print(0,"QCallThread1::executeF103 %d %5.2f %5.2f",aX1,aX2,aX3);
}

void QCallThread1::executeF104 (int aX1,double aX2,float aX3,long long int aX4)
{
   Prn::print(0,"QCallThread1::executeF104 %d %5.2f %5.2f %llX",aX1,aX2,aX3,aX4);
}

void QCallThread1::executeF105 (int aX1,double aX2,float aX3,long long int aX4,double aX5)
{
   Prn::print(0,"QCallThread1::executeF105 %d %5.2f %5.2f %llX %5.2f",aX1,aX2,aX3,aX4,aX5);
}


}//namespace