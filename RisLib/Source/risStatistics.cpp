/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <math.h>
#include "risStatistics.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

Statistics::Statistics()
{
   initializePass1(0.0);
}

void Statistics::initializePass1(double aExpectedMean)
{
   mMin          = 0.0;   
   mMax          = 0.0;   
   mExtremum     = 0.0;
   mMean         = 0.0;   
   mMeanDiff     = 0.0;   
   mStdDev       = 0.0;   
   mPutCount     = 0;
   mFirstFlag    = true;
   mSum          = 0.0; 
   mExpectedMean = aExpectedMean;  
}

//******************************************************************************

void Statistics::putValuePass1(double aValue)
{
   mPutCount++;

   if(mFirstFlag)
   {
      mFirstFlag = false;
      mMin       = aValue;   
      mMax       = aValue;
      mExtremum  = aValue;   
   }
   else
   {
      if (aValue<mMin) mMin=aValue;
      if (aValue>mMax) mMax=aValue;

      double tAbsValue = aValue > 0.0 ? aValue : -aValue;
      if (tAbsValue > mExtremum) mExtremum = tAbsValue;
   }

   mSum += aValue;
}

//******************************************************************************

void Statistics::finalizePass1()
{
   if (mPutCount==0) return;

   mMean = mSum/double(mPutCount);
}


//******************************************************************************

void Statistics::initializePass2()
{
   mStdDev   = 0.0;
   mRms      = 0.0;   
   mPutCount = 0;
   mSum      = 0.0;   
}

//******************************************************************************

void Statistics::putValuePass2(double aValue)
{
   mPutCount++;

   double  tTemp = aValue - mMean;
   mSum += tTemp*tTemp;
}

//******************************************************************************

void Statistics::finalizePass2()
{
   if (mPutCount==0) return;
   if (mSum<0.0)     return;

   mStdDev = sqrt(mSum/double(mPutCount));

   double tTemp = mMean - mExpectedMean;
   mRms = sqrt(mStdDev*mStdDev + tTemp*tTemp);

   mMeanDiff = fabs(mMean - mExpectedMean);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Statistics::initialize(int aPass, double aExpectedMean )
{
   mPass = aPass;

   if (mPass==1)initializePass1(aExpectedMean);
   if (mPass==2)initializePass2();
}

//******************************************************************************

void Statistics::putValue(double aValue)
{
   if (mPass==1)putValuePass1(aValue);
   if (mPass==2)putValuePass2(aValue);
}

//******************************************************************************

void Statistics::finalize()
{
   if (mPass==1)finalizePass1();
   if (mPass==2)finalizePass2();
}

}//namespace

