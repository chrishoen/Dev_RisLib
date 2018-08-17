/*==============================================================================
File: someTest1Thread.cpp
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "someShare.h"

#define  _SOMEQCALLTHREAD2_CPP_
#include "someQCallThread2.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

QCallThread2::QCallThread2()
{
   // Set base class
   BaseClass::setThreadPriorityHigh();
   BaseClass::mThreadAffinityMask = 0x20;
   BaseClass::mThreadIdealProcessor = 5;
   BaseClass::mTimerPeriod=0;

   // QCall CallPointers
   mC101QCall.bind  (this,&QCallThread2::executeC101);
   mC102QCall.bind  (this,&QCallThread2::executeC102);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void QCallThread2::executeOnTimer (int aTimeCount)
{
}

//******************************************************************************

void QCallThread2::executeC101(int aN)
{
   gShare.mTimeMarker.doStop();
}

//******************************************************************************

void QCallThread2::executeC102(int aN)
{
   Prn::print(0,"QCallThread2::executeC102 %d",aN);
}

}//namespace