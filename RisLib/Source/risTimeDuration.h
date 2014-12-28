#ifndef _RISTIMEDURATION_H_
#define _RISTIMEDURATION_H_
/*==============================================================================
==============================================================================*/
#include <math.h>
#include <stdio.h>
#include "risPortableTypes.h"

namespace Ris
{


class  TimeDuration
{
public:
   enum Units {Sec, Ms, Us , Ns, Seconds,  Milliseconds, Microseconds, Nanoseconds};

   explicit TimeDuration(double pValue);
   TimeDuration();
   TimeDuration(double pValue, Units pUnit);


   static TimeDuration fromSeconds(double pTimeDuration);
   static TimeDuration fromMilliseconds(double pTimeDuration);
   static TimeDuration fromMicroseconds(double pTimeDuration);
   static TimeDuration fromNanoseconds(double pTimeDuration);


   static TimeDuration fromMs(double pTimeDuration);
   static TimeDuration fromUs(double pTimeDuration);
   static TimeDuration fromNs(double pTimeDuration);

   double inSeconds(void) const;
   double inMilliseconds(void) const;
   double inMicroseconds(void) const;
   double inNanoseconds(void) const;
   double inNs(void) const;
   void setSeconds(double pTimeInSeconds);
   void setMilliseconds(double pTimeInMilliSeconds);
   void setMicroseconds(double pTimeInMicroSeconds);
   void setNanoseconds(double pTimeInNanoseconds);
   void setNanoseconds(long long pTimeInNanoseconds);
   void setNs(double pTimeInNanoseconds);
   void setNs(long long pTimeInNanoseconds);

   char* asString   (char* aString);
   void  fromString (char* aString);

   void convertFromSecondsAndHalfNanoseconds (int  aSeconds, int  aHalfNanoseconds);
   void convertToSecondsAndHalfNanoseconds   (int& aSeconds, int& aHalfNanoseconds) const;

   void setZero();
   bool isZero();
   static const TimeDuration cZero;
   static const TimeDuration cDurationMax;
   

   void operator()(double pValue, Units pUnit);
   double operator()(Units pUnit);
   TimeDuration operator+(TimeDuration pRight) const;
   TimeDuration operator+=(TimeDuration pRight);
   TimeDuration operator-(TimeDuration pRight) const;
   TimeDuration operator-(void) const;
   TimeDuration operator-=(TimeDuration pRight);
   TimeDuration operator*(double pRight) const;
   friend TimeDuration operator*(double  pRight,TimeDuration pLeft);
   double operator/(TimeDuration pRight) const;
   TimeDuration operator/(double pRight) const;
   TimeDuration operator%(TimeDuration pRight) const;
   TimeDuration operator%=(TimeDuration pRight);

   TimeDuration operator=(TimeDuration pRight);
   TimeDuration abs(void) const;

   enum Comparison {LessThan, Equal, GreaterThan};

   Comparison compare  (TimeDuration pRight) const;

   bool operator==(TimeDuration pRight) const;
   bool operator!=(TimeDuration pRight) const;
   bool operator>(TimeDuration pRight) const;
   bool operator>=(TimeDuration pRight) const;
   bool operator<(TimeDuration pRight) const;
   bool operator<=(TimeDuration pRight) const;

   bool isCloseTo(Ris::TimeDuration pValue,Ris::TimeDuration pTolerance);

protected:
   double mTimeDurationInNanoseconds;
public:

   static const double mConversionTable[8];

};

}//namespace

#endif


