#include "risTimeDuration.h"
#include "risTime.h"
/*==============================================================================
==============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace Ris
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
const Time Time::cZero(0,0);

const long long Time::mConversionTable[8] =
   {2000000000,2000000,2000,2,2000000000,2000000,2000,2};

//******************************************************************************

Time::Time()
{ 
   mTimeInHalfNanoseconds=0;
}

//******************************************************************************

Time::Time(int aSeconds, int aHalfNanoseconds)
{
   mTimeInHalfNanoseconds = long long(aSeconds) * mConversionTable[Seconds] + aHalfNanoseconds;
}

//******************************************************************************

Time::Time(
   int aYear,
   int aMonth,
   int aDay,
   int aHours, 
   int aMinutes, 
   TimeDuration aSeconds)
{
   convertFromDate(aYear,aMonth,aDay,aHours,aMinutes,aSeconds);
}

//******************************************************************************

Time::Time(
   int aYear,
   int aDayInYear,
   int aHours,
   int aMinutes,
   TimeDuration aSeconds)
{
   convertFromIrig(aYear,aDayInYear,aHours,aMinutes,aSeconds);
}

//******************************************************************************

void Time::setNanoseconds(long long aNanoseconds)
{
   mTimeInHalfNanoseconds = aNanoseconds*2;
}

//******************************************************************************

void Time::setNs(long long aNanoseconds)
{
   mTimeInHalfNanoseconds = aNanoseconds*2;
}

//******************************************************************************

Time Time::fromNanoseconds(long long aNanoseconds)
{
   Time temp;
   temp.setNanoseconds(aNanoseconds);
   return temp;
}

//******************************************************************************

Time Time::fromNs(long long aNanoseconds)
{
   Time temp;
   temp.setNanoseconds(aNanoseconds);
   return temp;
}

//******************************************************************************

Time Time::fromNs(double aNanoseconds)
{
   Time temp;
   temp.setNanoseconds(long long(aNanoseconds));
   return temp;
}

//******************************************************************************

long long Time::inNanoseconds(void) const
{
   return this->mTimeInHalfNanoseconds >> 1;
}

//******************************************************************************

long long Time::inNs(void) const
{
   return this->mTimeInHalfNanoseconds >> 1;
}

//******************************************************************************

long long Time::inHalfNanoseconds(void) const
{
   return this->mTimeInHalfNanoseconds;
}

//******************************************************************************

void Time::setHalfNanoseconds(long long aHalfNanoseconds)
{
   mTimeInHalfNanoseconds = aHalfNanoseconds;
}

//******************************************************************************

void Time::inSecondsAndHalfNanoseconds(int& aSeconds, int& aHalfNanoseconds) const
{
   aSeconds = (int) (this->mTimeInHalfNanoseconds / mConversionTable[Seconds]);
   aHalfNanoseconds = (int) (this->mTimeInHalfNanoseconds - (((long long) aSeconds) * mConversionTable[Seconds]));
}

//******************************************************************************

void Time::setSecondsAndHalfNanoseconds(int aSeconds, int aHalfNanoseconds)
{
   *this = Time(aSeconds,aHalfNanoseconds);
}

//******************************************************************************

void Time::setSeconds(int aSeconds)
{
   *this = Time(aSeconds,0);
}

//******************************************************************************

void Time::convertToSecondsAndHalfNanoseconds(int& aSeconds, int& aHalfNanoseconds) const
{
   aSeconds = (int) (this->mTimeInHalfNanoseconds / mConversionTable[Seconds]);
   aHalfNanoseconds = (int) (this->mTimeInHalfNanoseconds - (((long long) aSeconds) * mConversionTable[Seconds]));
}

//******************************************************************************

void Time::convertFromSecondsAndHalfNanoseconds(int aSeconds, int aHalfNanoseconds)
{
   mTimeInHalfNanoseconds = long long(aSeconds) * mConversionTable[Seconds] + aHalfNanoseconds;
}

//******************************************************************************

void Time::setToAverageOfTwo(const Time aValue1,const Time aValue2)
{
   mTimeInHalfNanoseconds = (aValue1.mTimeInHalfNanoseconds + aValue2.mTimeInHalfNanoseconds)/2;
}

//******************************************************************************

Time Time::nearestMinute(void) const
{
   Time returnValue;
   returnValue = Time(
       mTimeInHalfNanoseconds - (mTimeInHalfNanoseconds % (mConversionTable[Seconds] * 60)));
   return returnValue;
}

//******************************************************************************

void Time::setZero()
{ 
   mTimeInHalfNanoseconds=0;
}
//******************************************************************************

bool Time::isZero()
{ 
   return mTimeInHalfNanoseconds==0;
}

//******************************************************************************

Time Time::operator+(TimeDuration aRight) const
{
   unsigned long long aRightInHalfNanoSeconds = unsigned long long(aRight.inNanoseconds()) << 1;
   return Time( mTimeInHalfNanoseconds + aRightInHalfNanoSeconds );
}

//******************************************************************************

Time operator+(TimeDuration aLeft, Time aRight) 
{
   return(Time( (unsigned long long(aLeft.inNanoseconds()) << 1) + aRight.mTimeInHalfNanoseconds));
}

//******************************************************************************

Time Time::operator+=(TimeDuration aRight)
{
   mTimeInHalfNanoseconds += (unsigned long long(aRight.inNanoseconds()) << 1);
   return *this;
}

//******************************************************************************

void Time::addNanoseconds(unsigned long long aNanoseconds)
{
   mTimeInHalfNanoseconds += (aNanoseconds << 1);
}

//******************************************************************************

TimeDuration Time::operator-(Time aRight) const
{
   return(TimeDuration(double(mTimeInHalfNanoseconds - aRight.mTimeInHalfNanoseconds)/2.0, TimeDuration::Nanoseconds));
}

//******************************************************************************

Time Time::operator-(TimeDuration aRight) const
{
   long long aRightInHalfNanoSeconds = unsigned long long(aRight.inNanoseconds()) << 1;
   return Time( mTimeInHalfNanoseconds - aRightInHalfNanoSeconds );
}

//******************************************************************************

Time Time::operator-=(TimeDuration aRight)
{
   long long aRightInHalfNanoSeconds = unsigned long long(aRight.inNanoseconds()) << 1;
   mTimeInHalfNanoseconds -= aRightInHalfNanoSeconds;
   return *this;
}

//******************************************************************************

Time Time::operator=(Time aRight)
{
   this->mTimeInHalfNanoseconds = aRight.mTimeInHalfNanoseconds;
   return *this;
}

//******************************************************************************

Time::Comparison Time::Comaare  (Time aRight) const
{
   return (mTimeInHalfNanoseconds < aRight.mTimeInHalfNanoseconds ? LessThan:
   (mTimeInHalfNanoseconds == aRight.mTimeInHalfNanoseconds ? Equal : GreaterThan));
}

//******************************************************************************

bool Time::operator==(Time aRight) const
{
   return(mTimeInHalfNanoseconds == aRight.mTimeInHalfNanoseconds);
}

//******************************************************************************

bool Time::operator!=(Time aRight) const
{
   return(mTimeInHalfNanoseconds != aRight.mTimeInHalfNanoseconds);
}

//******************************************************************************

bool Time::operator>(Time aRight) const
{
   return(mTimeInHalfNanoseconds > aRight.mTimeInHalfNanoseconds);
}

//******************************************************************************

bool Time::operator>=(Time aRight) const
{
   return(mTimeInHalfNanoseconds >= aRight.mTimeInHalfNanoseconds);
}

//******************************************************************************

bool Time::operator<(Time aRight) const
{
   return(mTimeInHalfNanoseconds < aRight.mTimeInHalfNanoseconds);
}

//******************************************************************************

bool Time::operator<=(Time aRight) const
{
   return(mTimeInHalfNanoseconds <= aRight.mTimeInHalfNanoseconds);
}
//******************************************************************************
// NOTE:
//  struct tm
//  tm_sec     Seconds after minute (0 – 59).
//  tm_min     Minutes after hour (0 – 59).
//  tm_hour    Hours after midnight (0 – 23).
//  tm_mday    Day of month (1 – 31).
//  tm_mon     Month (0 – 11; January = 0).
//  tm_year    Year (current year minus 1900).
//  tm_wday    Day of week (0 – 6; Sunday = 0).
//  tm_yday    Day of year (0 – 365; January 1 = 0).
//  tm_isdst   Daylight savings time

void Time::convertFromDate(
   int aYear,      //1970..
   int aMonth,     //0..11
   int aDay,       //0..30
   int aHours,     //0..23
   int aMinutes,   //0..59
   Ris::TimeDuration& aSeconds)
{
   struct tm tTm = {0};
   time_t tTt    = {0};

   int tSeconds;
   int tHalfNs;
   aSeconds.convertToSecondsAndHalfNanoseconds(tSeconds,tHalfNs);

   tTm.tm_sec     = tSeconds;
   tTm.tm_min     = aMinutes;
   tTm.tm_hour    = aHours; 
   tTm.tm_mday    = aDay + 1;
   tTm.tm_mon     = aMonth;
   tTm.tm_year    = aYear - 1900;
   tTm.tm_wday    = 0;
   tTm.tm_yday    = 0;
   tTm.tm_isdst   = 0;

#if defined(WIN32)
   tTt = _mkgmtime32(&tTm);
#else
   tTt = mktime(&tTm);
#endif
 
   tSeconds = (unsigned)tTt;

   convertFromSecondsAndHalfNanoseconds(tSeconds,tHalfNs);
}

//******************************************************************************
// NOTE:
//  struct tm
//  tm_sec     Seconds after minute (0 – 59).
//  tm_min     Minutes after hour (0 – 59).
//  tm_hour    Hours after midnight (0 – 23).
//  tm_mday    Day of month (1 – 31).
//  tm_mon     Month (0 – 11; January = 0).
//  tm_year    Year (current year minus 1900).
//  tm_wday    Day of week (0 – 6; Sunday = 0).
//  tm_yday    Day of year (0 – 365; January 1 = 0).
//  tm_isdst   Daylight savings time

void Time::convertToDate(
   int& aYear,     //1970..
   int& aMonth,    //0..11
   int& aDay,      //0..30
   int& aHours,    //0..23 
   int& aMinutes,  //0..59
   Ris::TimeDuration& aSeconds) const
{
   struct tm* tTm;
   time_t tTt    = {0};

   int tSeconds;
   int tHalfNs;
   convertToSecondsAndHalfNanoseconds(tSeconds,tHalfNs);

   tTt = (time_t)tSeconds;
#if defined(WIN32)
   tTm = gmtime(&tTt);
#else
 //tTm = localtime(&tTt);
   tTm = gmtime(&tTt);
#endif

   aYear     = tTm->tm_year + 1900;
   aMonth    = tTm->tm_mon;
   aDay      = tTm->tm_mday - 1; 
   aHours    = tTm->tm_hour; 
   aMinutes  = tTm->tm_min;
   tSeconds  = tTm->tm_sec;
   aSeconds.convertFromSecondsAndHalfNanoseconds(tSeconds,tHalfNs);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

const int cDaysInMonthTable_LeapNo[12] = 
   {31,28,31,30,31,30,31,31,30,31,30,31};

const int cDaysInMonthTable_LeapYes[12] = 
   {31,29,31,30,31,30,31,31,30,31,30,31};

const int cDaysInYearTable_LeapNo[12] = 
   {0,31,59,90,120,151,181,212,243,273,304,334};

const int cDaysInYearTable_LeapYes[12] = 
   {0,31,60,91,121,152,182,213,244,274,305,335};

//******************************************************************************

void Time::convertFromIrig(
   int aYear,      //1970..
   int aDayInYear, //1..366
   int aHours,     //0..23
   int aMinutes,   //0..59
   Ris::TimeDuration& aSeconds)
{
   bool tLeapYear = (aYear % 4)==0;

   int tMonth=0;
   int tDay=0;

   const int* tDaysInYear = tLeapYear ? cDaysInYearTable_LeapYes : cDaysInYearTable_LeapNo;

   for (int i=0;i<11;i++)
   {
      if (aDayInYear >= tDaysInYear[i] && aDayInYear < tDaysInYear[i+1])
      {
         tMonth=i;
         tDay = aDayInYear - tDaysInYear[i] - 1;
      }  
   }
   if (aDayInYear >= tDaysInYear[11])
   {
      tMonth=11;
      tDay = aDayInYear - tDaysInYear[11] - 1;
   }  

   convertFromDate(aYear,tMonth,tDay,aHours,aMinutes,aSeconds);
}

//******************************************************************************

void Time::convertToIrig(
   int& aYear,      //1970..
   int& aDayInYear, //1..366
   int& aHours,     //0..23 
   int& aMinutes,   //0..59
   Ris::TimeDuration& aSeconds) const
{
   int tMonth;
   int tDay;

   convertToDate(aYear,tMonth,tDay,aHours,aMinutes,aSeconds);

   bool tLeapYear = (aYear % 4)==0;
   const int* tDaysInYear = tLeapYear ? cDaysInYearTable_LeapNo : cDaysInYearTable_LeapNo;

   aDayInYear = tDaysInYear[tMonth] + tDay + 1;
}

//******************************************************************************

char* Time::asString(char* aString)
{
   int tYear;
   int tDayInYear;
   int tHours;
   int tMinutes;
   TimeDuration tSeconds;

   convertToIrig(tYear,tDayInYear,tHours,tMinutes,tSeconds);

   int tSecond=0;
   int tHalfNs=0;
   tSeconds.convertToSecondsAndHalfNanoseconds(tSecond,tHalfNs);

   sprintf(aString,"%04d:%03d:%02d:%02d:%02d",tYear,tDayInYear,tHours,tMinutes,tSecond);
   return aString;
}

//******************************************************************************

char* Time::asString2(char* aString)
{
   int tYear;
   int tDayInYear;
   int tHours;
   int tMinutes;
   TimeDuration tSeconds;

   convertToIrig(tYear,tDayInYear,tHours,tMinutes,tSeconds);

   sprintf(aString,"%04d\t%03d\t%02d\t%02d\t%012.9lf",tYear,tDayInYear,tHours,tMinutes,tSeconds.inSeconds());
   return aString;
}

//******************************************************************************
char* Time::asString3(char* aString)
{
   int tYear;
   int tDayInYear;
   int tHours;
   int tMinutes;
   TimeDuration tSeconds;

   convertToIrig(tYear,tDayInYear,tHours,tMinutes,tSeconds);

   sprintf(aString,"%04d:%03d:%02d:%02d:%012.9lf",tYear,tDayInYear,tHours,tMinutes,tSeconds.inSeconds());
   return aString;
}

}//namespace

