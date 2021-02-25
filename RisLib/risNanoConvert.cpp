/*==============================================================================
==============================================================================*/
#include "stdafx.h"

#include <time.h>

#include "risNanoConvert.h"

namespace Ris
{
namespace NanoConvert
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Unit conversions

// Nanoseconds from seconds
long long  getNsFromSec  (double aTimeSec)
{
   return (long long)(aTimeSec*(1000*1000*1000));
}

// Nanoseconds from milliseconds
long long  getNsFromMs   (int    aTimeMs)
{
   return ((long long)aTimeMs)*(1000*1000);
}

// Nanoseconds from microseconds
long long  getNsFromUs   (int    aTimeUs)
{
   return ((long long)aTimeUs)*1000;
}

// Seconds from nanoseconds
double getSecFromNs  (long long  aTimeNs)
{
   return (double)(aTimeNs/(double)(1000*1000*1000));
}

// Milliseconds from nanoseconds
int    getMsFromNs   (long long  aTimeNs)
{
   return (int)(aTimeNs/ (1000*1000));
}

// Microseconds from nanoseconds
int    getUsFromNs   (long long  aTimeNs)
{
   return (int)(aTimeNs/1000);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// timespec conversions

long long getNsFromTimespec(struct timespec* aTimespec)
{
   return (long long)(aTimespec->tv_sec)*1000*1000*1000 + (long long)aTimespec->tv_nsec;
}

struct timespec  getTimespecFromNs(long long aTimeNs)
{
   struct timespec tTimespec;
   tTimespec.tv_sec   = (int)(aTimeNs / (1000*1000*1000));
   tTimespec.tv_nsec  = (int)(aTimeNs % (1000*1000*1000));
   return tTimespec;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// tm conversions

void getNsFromTm(long long& aTimeNs, struct tm* aTm)
{
   struct timespec tTimespec;
   tTimespec.tv_sec = mktime(aTm);
   tTimespec.tv_nsec = 0;

   aTimeNs = getNsFromTimespec(&tTimespec);
}

void getTmFromNs(struct tm* aTm, long long aTimeNs)
{
   struct timespec tTimespec = getTimespecFromNs(aTimeNs);

   tm* tTm = localtime(&tTimespec.tv_sec);
   memcpy((void*)aTm, tTm, sizeof(tm));
}

}//namespace
}//namespace

