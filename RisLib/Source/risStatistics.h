#ifndef _RISSTATISTICS_H_
#define _RISSTATISTICS_H_

/*==============================================================================

Class Ris::Statistics

This can be used to perform calculations on sequences of values.

==============================================================================*/

#include "risPortableTypes.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************

class  Statistics
{
public:
   Statistics();

   void initialize (int aPass, double aExpectedMean=0.0);
   void putValue   (double aValue);
   void finalize   ();

   void initializePass1 (double aExpectedMean=0.0);
   void putValuePass1   (double aValue);
   void finalizePass1   ();

   void initializePass2 ();
   void putValuePass2   (double aValue);
   void finalizePass2   ();

   // Results
   double mMin;   
   double mMax;  
   double mExtremum;
   double mMean;   
   double mMeanDiff;   
   double mStdDev;   
   double mRms;   

   // State
   int    mPass;
   int    mPutCount;
   bool   mFirstFlag;

   double mSum;   

   double mExpectedMean;
};

}//namespace
#endif

