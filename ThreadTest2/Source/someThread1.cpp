/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "someShare.h"

#define  _SOMETHREAD1_CPP_
#include "someThread1.h"

namespace Some
{

Thread1::Thread1()
{
   // Set base class thread priority
   BaseClass::setThreadPriorityHigh();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Thread1::threadRunFunction()
{
   while (true)
   {
      mThreadSem.get();
      gShare.mTimeMarker.doStop();

      if (gShare.mTimeMarker.mStatistics.mEndOfPeriod)
      {
         Prn::print(Prn::ThreadRun1, "TEST1 %5d $$ %10.3f  %10.3f  %10.3f  %10.3f",
            gShare.mTimeMarker.mStatistics.mSize,
            gShare.mTimeMarker.mStatistics.mMean,
            gShare.mTimeMarker.mStatistics.mStdDev,
            gShare.mTimeMarker.mStatistics.mMinX,
            gShare.mTimeMarker.mStatistics.mMaxX);
      }
   }

// Prn::print(Prn::ThreadRun1, "Thread1");
}

}//namespace