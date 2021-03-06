#pragma once

/*==============================================================================
Timer test thread.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsPeriodicThread2.h"
#include "risTimeMarker.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class TimerTestThread1 : public Ris::Threads::BasePeriodicThread2
{
public:
   typedef Ris::Threads::BasePeriodicThread2 BaseClass;

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Members.

   Ris::PeriodicTimeMarker mTimeMarker;

   int  mTestCode;
   int  mTestCount;

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Members.

   // If true then the test variables have been updated.
   bool mUpdateFlag;

   // Test variables.
   int mProcessorNumber;
   double mMean;
   double mStdDev;
   double mMin;
   double mMax;
   double mMaxError;

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Methods.

   // Constructor.
   TimerTestThread1();

   // Base class overloads.
   void executeOnTimer(int aTimeCount) override;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _SOMETIMERTESTTHREAD1_CPP_
          TimerTestThread1* gTimerTestThread1 = 0;
#else
   extern TimerTestThread1* gTimerTestThread1;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


