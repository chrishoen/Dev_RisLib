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

class TimerTestThread : public Ris::Threads::BasePeriodicThread2
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
   TimerTestThread();

   // Base class overloads.
   void executeOnTimer(int aTimeCount) override;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _SOMETIMERTESTTHREAD_CPP_
          TimerTestThread* gTimerTestThread = 0;
#else
   extern TimerTestThread* gTimerTestThread;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


