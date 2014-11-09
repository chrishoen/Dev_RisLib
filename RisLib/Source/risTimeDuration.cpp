#include "risTimeDuration.h"
/*==============================================================================
==============================================================================*/
#include <stdlib.h>

namespace Ris
{


//******************************************************************************
//******************************************************************************
//******************************************************************************

const double TimeDuration::mConversionTable[8] =
   {1000000000.0,1000000.0,1000.0,1.0,1000000000.0,1000000.0,1000.0,1.0};

const TimeDuration TimeDuration::cZero(0.0);

// Set MAX in Seconds value shows as 0:00:00 if use higher value e.g., 24 hours
const TimeDuration TimeDuration::cDurationMax(23.9 *60.0 *60.0 * 1000000000.0);
//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************

TimeDuration::TimeDuration(double pValue)
{mTimeDurationInNanoseconds = pValue;};

//******************************************************************************

TimeDuration::TimeDuration()
{
   mTimeDurationInNanoseconds=0.0;
}

//******************************************************************************

TimeDuration::TimeDuration(double pValue, Units pUnit)
{
   mTimeDurationInNanoseconds = pValue * mConversionTable[pUnit];
}

//******************************************************************************

TimeDuration TimeDuration::fromSeconds(double pTimeDuration)
{
   return TimeDuration(pTimeDuration * 1000000000.0);
}

//******************************************************************************

TimeDuration TimeDuration::fromMilliseconds(double pTimeDuration)
{
   return TimeDuration(pTimeDuration * 1000000.0);
}

//******************************************************************************

TimeDuration TimeDuration::fromMicroseconds(double pTimeDuration)
{
   return TimeDuration(pTimeDuration * 1000.0);
}

//******************************************************************************

TimeDuration TimeDuration::fromNanoseconds(double pTimeDuration)
{
   return TimeDuration(pTimeDuration);
}

//******************************************************************************

TimeDuration TimeDuration::fromMs(double pTimeDuration)
{
   return TimeDuration(pTimeDuration * 1000000.0);
}

//******************************************************************************

TimeDuration TimeDuration::fromUs(double pTimeDuration)
{
   return TimeDuration(pTimeDuration * 1000.0);
}

//******************************************************************************

TimeDuration TimeDuration::fromNs(double pTimeDuration)
{
   return TimeDuration(pTimeDuration);
}

//******************************************************************************

double TimeDuration::inSeconds(void) const
{
   return (mTimeDurationInNanoseconds/1000000000.0);
}

//******************************************************************************

double TimeDuration::inMilliseconds(void) const
{
   return (mTimeDurationInNanoseconds/1000000.0);
}

//******************************************************************************

double TimeDuration::inMicroseconds(void) const
{
   return (mTimeDurationInNanoseconds/1000.0);
}

//******************************************************************************

double TimeDuration::inNanoseconds(void) const
{
   return (mTimeDurationInNanoseconds);
}

//******************************************************************************

double TimeDuration::inNs(void) const
{
   return (mTimeDurationInNanoseconds);
}

//******************************************************************************

//******************************************************************************

void TimeDuration::setSeconds(double pTimeInSeconds)
{
   mTimeDurationInNanoseconds = pTimeInSeconds*mConversionTable[Seconds];
}

//******************************************************************************

void TimeDuration::setMilliseconds(double pTimeInMilliSeconds)
{
   mTimeDurationInNanoseconds = pTimeInMilliSeconds*mConversionTable[Milliseconds];
}

//******************************************************************************

void TimeDuration::setMicroseconds(double pTimeInMicroSeconds)
{
   mTimeDurationInNanoseconds = pTimeInMicroSeconds*mConversionTable[Microseconds];
}

//******************************************************************************

void TimeDuration::setNanoseconds(double pTimeInNanoseconds)
{
   mTimeDurationInNanoseconds = pTimeInNanoseconds;
}

//******************************************************************************

void TimeDuration::setNanoseconds(Int64 pTimeInNanoseconds)
{
   mTimeDurationInNanoseconds = double(pTimeInNanoseconds);
}

//******************************************************************************

void TimeDuration::setNs(double pTimeInNanoseconds)
{
   mTimeDurationInNanoseconds = pTimeInNanoseconds;
}

//******************************************************************************

char* TimeDuration::asString(char* aString)
{
   int   tTimeSeconds  = (int)inSeconds();
   float tTimeSecondsF = (float)inSeconds();

   int  tSecInOneMin   = 60;
   int  tSecInOneHour  = 60*60;
   int  tSecInOneDay   = 60*60*24;

   int  tSecInDay  = tTimeSeconds  % tSecInOneDay;
   int  tSecInHour = tTimeSeconds  % tSecInOneHour;
   int  tSecInMin  = tTimeSeconds  % tSecInOneMin;

   int  tHourInDay = tSecInDay  / tSecInOneHour;
   int  tMinInHour = tSecInHour / tSecInOneMin;

   sprintf(aString,"%02d:%02d:%02d",tHourInDay,tMinInHour,tSecInMin);
   return aString;
}

//******************************************************************************

void TimeDuration::fromString(char* aString)
{
   int tTimeSeconds = (int)inSeconds();

   int tSecInOneMin  = 60;
   int tSecInOneHour = 60*60;
   int tSecInOneDay  = 60*60*24;

   int    tHourInDay;
   int    tMinInHour;
   int    tSecInMin;
   int    tTenthsOfSec;

   // 00:00:00.0
   if (aString[2]==':')
   {
      sscanf(aString,"%02d:%02d:%02d.%1d",&tHourInDay,&tMinInHour,&tSecInMin,&tTenthsOfSec);
      double tSeconds = tSecInOneHour*tHourInDay + tSecInOneMin*tMinInHour + tSecInMin + double(tTenthsOfSec)*0.1;
      setSeconds(tSeconds);
   }
   else
   {
      double tSeconds = atof(aString);
      setSeconds(tSeconds);
   }
}

//******************************************************************************

void TimeDuration::setNs(Int64 pTimeInNanoseconds)
{
   mTimeDurationInNanoseconds = double(pTimeInNanoseconds);
}

//******************************************************************************

void TimeDuration::convertFromSecondsAndHalfNanoseconds (int aSeconds, int  aHalfNanoseconds)
{
   mTimeDurationInNanoseconds = double(aSeconds)*1000000000 + double(aHalfNanoseconds/2);
}

//******************************************************************************

void TimeDuration::convertToSecondsAndHalfNanoseconds   (int& aSeconds,int& aHalfNanoseconds) const
{
   aSeconds = int(mTimeDurationInNanoseconds/1000000000);
   //aHalfNanoseconds = int(fmod(mTimeDurationInNanoseconds,1000000000))*2;
   aHalfNanoseconds = int(mTimeDurationInNanoseconds - (double(aSeconds) * 1000000000.0))*2;
}

//******************************************************************************

void TimeDuration::setZero()
{ 
   mTimeDurationInNanoseconds=0;
}
//******************************************************************************

bool TimeDuration::isZero()
{ 
   return mTimeDurationInNanoseconds==0;
}

//******************************************************************************

void TimeDuration::operator()(double pValue, Units pUnit)
{
   mTimeDurationInNanoseconds = pValue * mConversionTable[pUnit];
}

//******************************************************************************

double TimeDuration::operator()(Units pUnit)
{
   return mTimeDurationInNanoseconds / mConversionTable[pUnit];
}

//******************************************************************************

TimeDuration TimeDuration::operator+(TimeDuration pRight) const
{
   return(TimeDuration(mTimeDurationInNanoseconds + pRight.mTimeDurationInNanoseconds));
}

//******************************************************************************

TimeDuration TimeDuration::operator+=(TimeDuration pRight)
{
   mTimeDurationInNanoseconds += pRight.mTimeDurationInNanoseconds;
   return *this;
};

//******************************************************************************

TimeDuration TimeDuration::operator-(TimeDuration pRight) const
{
   return(TimeDuration(mTimeDurationInNanoseconds - pRight.mTimeDurationInNanoseconds));
}

//******************************************************************************

TimeDuration TimeDuration::operator-(void) const
{
   return(TimeDuration(- mTimeDurationInNanoseconds));
}

//******************************************************************************

TimeDuration TimeDuration::operator-=(TimeDuration pRight)
{
   mTimeDurationInNanoseconds -= pRight.mTimeDurationInNanoseconds;
   return *this;
};

//******************************************************************************

TimeDuration TimeDuration::operator*(double pRight) const
{
   return(TimeDuration(mTimeDurationInNanoseconds * pRight));
}

//******************************************************************************

TimeDuration operator*(double pRight,TimeDuration pLeft)
{
   return(TimeDuration(pLeft.mTimeDurationInNanoseconds * pRight));
}

//******************************************************************************

double TimeDuration::operator/(TimeDuration pRight) const
{
   return(mTimeDurationInNanoseconds / pRight.mTimeDurationInNanoseconds);
}

//******************************************************************************

TimeDuration TimeDuration::operator/(double pRight) const
{
   return TimeDuration(mTimeDurationInNanoseconds / pRight);
}

//******************************************************************************

TimeDuration TimeDuration::operator%(TimeDuration pRight) const
{
   return TimeDuration(fmod(mTimeDurationInNanoseconds,pRight.mTimeDurationInNanoseconds));
}

//******************************************************************************

TimeDuration TimeDuration::operator%=(TimeDuration pRight)
{
   mTimeDurationInNanoseconds = fmod(mTimeDurationInNanoseconds,pRight.mTimeDurationInNanoseconds);
   return *this;
};

//******************************************************************************

TimeDuration TimeDuration::operator=(TimeDuration pRight)
{
   this->mTimeDurationInNanoseconds = pRight.mTimeDurationInNanoseconds;
   return *this;
}

//******************************************************************************

TimeDuration TimeDuration::abs(void) const
{
   return TimeDuration((mTimeDurationInNanoseconds < 0.0) ? -mTimeDurationInNanoseconds : mTimeDurationInNanoseconds);
}

//******************************************************************************

TimeDuration::Comparison TimeDuration::compare  (TimeDuration pRight) const
{
   return (mTimeDurationInNanoseconds < pRight.mTimeDurationInNanoseconds ? LessThan:
     (mTimeDurationInNanoseconds == pRight.mTimeDurationInNanoseconds ? Equal : GreaterThan));
}

//******************************************************************************

bool TimeDuration::operator==(TimeDuration pRight) const
{
   return(mTimeDurationInNanoseconds == pRight.mTimeDurationInNanoseconds);
}

//******************************************************************************

bool TimeDuration::operator!=(TimeDuration pRight) const
{
   return(mTimeDurationInNanoseconds != pRight.mTimeDurationInNanoseconds);
}

//******************************************************************************

bool TimeDuration::operator>(TimeDuration pRight) const
{
   return(mTimeDurationInNanoseconds > pRight.mTimeDurationInNanoseconds);
}

//******************************************************************************

bool TimeDuration::operator>=(TimeDuration pRight) const
{
   return(mTimeDurationInNanoseconds >= pRight.mTimeDurationInNanoseconds);
}

//******************************************************************************

bool TimeDuration::operator<(TimeDuration pRight) const
{
   return(mTimeDurationInNanoseconds < pRight.mTimeDurationInNanoseconds);
}

//******************************************************************************

bool TimeDuration::operator<=(TimeDuration pRight) const
{
   return(mTimeDurationInNanoseconds <= pRight.mTimeDurationInNanoseconds);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool TimeDuration::isCloseTo(Ris::TimeDuration pValue,Ris::TimeDuration pTolerance)
{
   return (*this > (pValue - pTolerance)) && ((*this < pValue + pTolerance)); 
}

}//namespace
