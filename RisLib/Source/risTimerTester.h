#ifndef _RISTIMERTESTER_H_
#define _RISTIMERTESTER_H_
/*==============================================================================

Used to test periodic timer functions

==============================================================================*/
#include "risPortableTypes.h"
#include "risTimeMarker.h"
#include "risStatistics.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
class  TimerTester
{
public:
   TimerTester();
   void reset();
   void startTest(int aTimerPeriod,int aNumOfIterations,int aNumOfExecutions);
   void executeOnTimer();

   //--------------------------------------------------------------
   // Execution variables:

   Ris::TimeMarker mTimeMarker1;
   Ris::TimeMarker mTimeMarker2;

   Ris::Statistics mStatistics;

   int   mTimerPeriod;
   bool  mEnableFlag;
   int   mIterationCount;
   int   mExecutionCount;
   int   mNumOfIterations;
   int   mNumOfExecutions;
};
}//namespace

#endif

