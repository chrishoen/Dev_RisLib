/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>

#include "risPortableCalls.h"
#include "risTimeMarker.h"

//******************************************************************************

Ris::TimeMarker::TimeMarker()
{
   mDelta=0.0;
   mString[0]=0;

   mPerformanceFrequency = (double) portableGetHiResFrequency();

   doBegin();
}

//******************************************************************************

void Ris::TimeMarker::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   aBuffer->copy ( & mBegin );
   aBuffer->copy ( & mEnd   );
}

//******************************************************************************

void Ris::TimeMarker::doBegin()
{
   mBegin = portableGetHiResCounter();
}
//******************************************************************************

double Ris::TimeMarker::elapsedTimeInSeconds()
{
   double delta;
   long long tEnd = portableGetHiResCounter();

   delta = ((double)(tEnd-mBegin))/mPerformanceFrequency ;
   return delta;
}

//******************************************************************************

double Ris::TimeMarker::elapsedTimeInMilliseconds()
{
   double delta;
   long long tEnd = portableGetHiResCounter();

   delta = ((double)(tEnd-mBegin))/mPerformanceFrequency ;
   return delta*1000.0;
}

//******************************************************************************

double Ris::TimeMarker::elapsedTimeInMicroseconds()
{
   double delta;
   long long tEnd = portableGetHiResCounter();

   delta = ((double)(tEnd-mBegin))/mPerformanceFrequency ;
   return delta*1000000.0;
}

//******************************************************************************

void Ris::TimeMarker::doEnd()
{
   mEnd = portableGetHiResCounter();
   
   mDelta = ((double)(mEnd-mBegin))/mPerformanceFrequency ;

   if (mDelta>1E-3)
   {
      sprintf(mString,"%8.6f",mDelta);
   }
   else
   {
      sprintf(mString,"%8.3f uS",mDelta/1E-6);
   }

}
  
//******************************************************************************

char* Ris::TimeMarker::asString1()
{
   sprintf(mStr,"%8.1f",mDelta);
   return mStr;
}

char* Ris::TimeMarker::asString2()
{
   return mString;
}

