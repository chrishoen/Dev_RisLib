#ifndef _RISTIME_H_
#define _RISTIME_H_
/*==============================================================================
Time class with internal representation of Int64 mTimeInHalfNanoseconds.
Time zero corresponds to 00:00:00 1 Jan 1970.
==============================================================================*/

#include <stdio.h>
#include "risPortableTypes.h"
#include "risTimeDuration.h"

namespace Ris
{

class  Time
{
public:
   //---------------------------------------------------------------------------
   // Constructors

   Time();

   Time(int aSeconds, int aHalfNanoseconds);

   Time(
      int aYear,      //1970
      int aMonth,     //0..11
      int aDay,       //0..30
      int aHours,     //0..23
      int aMinutes,   //0..59
      Ris::TimeDuration aSeconds);

   Time(
      int aYear,     //1970..
      int aDay,      //1..366
      int aHours,    //0..23
      int aMinutes,  //0..59
      Ris::TimeDuration aSeconds);

   char* asString(char* aString);
   char* asString2(char* aString);
   char* asString3(char* aString);

   //---------------------------------------------------------------------------
   // Conversions

   void setNanoseconds(Int64 aNanoseconds);
   void setNs(Int64 aNanoseconds);
   static Time fromNanoseconds(Ris::Int64 aNanoseconds);
   static Time fromNs(Ris::Int64 aNanoseconds);
   static Time fromNs(double aNanoseconds);
   Int64 inNanoseconds(void) const;
   Int64 inNs(void) const;
   Int64 inHalfNanoseconds(void) const;
   void setHalfNanoseconds(Int64 aHalfNanoseconds);
   void inSecondsAndHalfNanoseconds(int& aSeconds, int& aHalfNanoseconds) const;
   void setSecondsAndHalfNanoseconds(int aSeconds, int aHalfNanoseconds);
   void setSeconds(int aSeconds);
   void setToAverageOfTwo(const Time aValue1,const Time aValue2);
   Time nearestMinute(void) const;

   void convertFromSecondsAndHalfNanoseconds (int  aSeconds, int  aHalfNanoseconds);
   void convertToSecondsAndHalfNanoseconds   (int& aSeconds, int& aHalfNanoseconds) const;

   void setZero();
   bool isZero();
   static const Time cZero;

   //---------------------------------------------------------------------------
   // Operators

   Time operator+(TimeDuration aRight) const;
   friend Time operator+(TimeDuration aLeft, Time aRight);
   Time operator+=(TimeDuration aRight);
   void addNanoseconds(Ris::Uint64 aNanoseconds);

   TimeDuration operator-(Time aRight) const;
   Time operator-(TimeDuration aRight) const;
   Time operator-=(TimeDuration aRight);

   Time operator=(Time aRight);

   //---------------------------------------------------------------------------
   // Comparisons

   enum Comparison {LessThan, Equal, GreaterThan};
   Comparison Comaare  (Time aRight) const;
   bool operator==(Time aRight) const;
   bool operator!=(Time aRight) const;
   bool operator>(Time aRight) const;
   bool operator>=(Time aRight) const;
   bool operator<(Time aRight) const;
   bool operator<=(Time aRight) const;

   //---------------------------------------------------------------------------
   // Conversions

   void convertFromDate(
      int aYear,      //1970..
      int aMonth,     //0..11
      int aDay,       //0..30
      int aHours,     //0..23
      int aMinutes,   //0..59
      Ris::TimeDuration& aSeconds);

   void convertToDate(
      int& aYear,     //1970..
      int& aMonth,    //0..11
      int& aDay,      //0..30
      int& aHours,    //0..23
      int& aMinutes,  //0..59
      Ris::TimeDuration& aSeconds) const;

   void convertFromIrig(
      int aYear,      //1970..
      int aDay,       //1..366
      int aHours,     //0..23
      int aMinutes,   //0..59
      Ris::TimeDuration& aSeconds);

   void convertToIrig(
      int& aYear,     //1970..
      int& aDay,      //1..366
      int& aHours,    //0..23
      int& aMinutes,  //0..59
      Ris::TimeDuration& aSeconds) const;

private:

   //---------------------------------------------------------------------------
   // Members

   Int64 mTimeInHalfNanoseconds;

   static const Ris::Int64 mConversionTable[8];

   enum Units {Sec, Ms, Us , Ns, Seconds,  Milliseconds, Microseconds, Nanoseconds};

   explicit Time(Int64 aTimeInHalfNanoseconds)
   {
      mTimeInHalfNanoseconds = aTimeInHalfNanoseconds;
   } 

};
}//namespace

#endif

