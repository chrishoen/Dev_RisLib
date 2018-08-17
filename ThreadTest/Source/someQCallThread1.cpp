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

}//namespace