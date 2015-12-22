/*==============================================================================
File: someTest1Thread.cpp
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "someThread1.h"
#include "someShare.h"

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

   mTimerPeriod=1000;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread2::executeOnTimer(int aCurrentTimeCount)
{
}

//******************************************************************************

void Thread2::executeC101(int aN)
{
   gShare.mTimeMarker.doStop();

   if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun1, "TEST1 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         gShare.mTimeMarker.mChangeCount,
         gShare.mTimeMarker.mStatistics.mMean,
         gShare.mTimeMarker.mStatistics.mStdDev,
         gShare.mTimeMarker.mStatistics.mMinX,
         gShare.mTimeMarker.mStatistics.mMaxX);
   }
   // Prn::print(Prn::ThreadRun1,"Thread2::executeC101    %04d",aN);
   // gThread1->mF101QCall.invoke(aN);
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
}

}//namespace