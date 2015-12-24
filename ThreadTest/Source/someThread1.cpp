/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "someShare.h"
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

   // Set base class thread priority
   BaseClass::setThreadPriorityHigh();

   // Set timer period
   int tFrequency = 100;
   int tPeriod = 1000 / tFrequency;

   BaseClass::mTimerPeriod = tPeriod;

   gShare.mTimeMarker.initialize(5*tFrequency);

   // Members
   mTPFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::executeOnTimer(int aTimerCount)
{
   if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
   {
      Prn::print(Prn::ThreadRun1, "TEST1 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
         gShare.mTimeMarker.mChangeCount,
         gShare.mTimeMarker.mStatistics.mMean,
         gShare.mTimeMarker.mStatistics.mStdDev,
         gShare.mTimeMarker.mStatistics.mMinX,
         gShare.mTimeMarker.mStatistics.mMaxX);
   }

   gShare.mTimeMarker.doStart();
   gThread2->mC101QCall.invoke(aTimerCount);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::executeF101(int aN)
{

// Prn::print(Prn::ThreadRun1,"Thread1::executeF101    %04d",aN);
// Prn::print(Prn::ThreadRun1,"");
}

//******************************************************************************

void Thread1::executeF102(int aN)
{
   gThread2->mC102QCall.invoke(102);
}


}//namespace