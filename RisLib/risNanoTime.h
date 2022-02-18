#pragma once

/*==============================================================================
Nanosecond time utilities.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <time.h>

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current high resolution timer value in nanoseconds. 

// Nanoseconds.
inline long long int get_Nanotime()
{
   struct timespec tTimespec;
   clock_gettime(CLOCK_MONOTONIC, &tTimespec);

   long long int tSeconds = (long long int)tTimespec.tv_sec;
   long long int tNanoseconds = (long long int)tTimespec.tv_nsec;
   long long int tTimeNs = tSeconds * 1000 * 1000 * 1000 + tNanoseconds;

   return tTimeNs;
}

// Nanoseconds from milliseconds.
inline long long  get_NsFromMs(int aTimeMs)
{
   return ((long long)aTimeMs) * (1000 * 1000);
}

// Nanoseconds from microseconds.
inline long long  get_NsFromUs(int aTimeMs)
{
   return ((long long)aTimeMs) * (1000);
}

// Microseconds from nanoseconds
inline double get_UsFromNs(long long  aTimeNs)
{
   return (double)(aTimeNs / 1000.0);
}

// Timespec from nanoseconds.
inline struct timespec get_TimespecFromNs(long long aTimeNs)
{
   struct timespec tTimespec;
   tTimespec.tv_sec = (int)(aTimeNs / (1000 * 1000 * 1000));
   tTimespec.tv_nsec = (int)(aTimeNs % (1000 * 1000 * 1000));
   return tTimespec;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

