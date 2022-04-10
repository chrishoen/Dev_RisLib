/*==============================================================================
==============================================================================*/

#include "stdafx.h"

#include <time.h>
#include <sys/time.h>

#include "risProgramTime.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regionals.

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current high resolution timer value in nanoseconds. 

inline long long int my_get_hires_count()
{
   struct timespec tTimespec;
   clock_gettime(CLOCK_MONOTONIC, &tTimespec);

   long long int tSeconds = (long long int)tTimespec.tv_sec;
   long long int tNanoseconds = (long long int)tTimespec.tv_nsec;
   long long int tTimeNs = tSeconds * 1000 * 1000 * 1000 + tNanoseconds;

   return tTimeNs;
}

// High resolution timer count at program startup in nanoseconds.
long long int gProgramStartHiresCount = my_get_hires_count();
   
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current program time in seconds.

double getProgramTime()
{
   // Get current hires count.
   long long int tCurrentHiresCount = my_get_hires_count();

   // Return the current program time in seconds.
   return (double)(tCurrentHiresCount - gProgramStartHiresCount)*1E-9;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current program time in milliseconds.

double getProgramTimeMS()
{
   // Get current hires count.
   long long int tCurrentHiresCount = my_get_hires_count();

   // Return the current program time in milliseconds.
   return (double)(tCurrentHiresCount - gProgramStartHiresCount)*1E-6;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current program time in microseconds.

double getProgramTimeUS()
{
   // Get current hires count.
   long long int tCurrentHiresCount = my_get_hires_count();

   // Return the current program time in seconds.
   return (double)(tCurrentHiresCount - gProgramStartHiresCount)*1E-3;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current program time in nanoseconds.

long long int getProgramTimeNS()
{
   // Get current hires count.
   long long int tCurrentHiresCount = my_get_hires_count();

   // Return the current program time in nanoseconds.
   return tCurrentHiresCount - gProgramStartHiresCount;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current cpu time in milliseconds.

unsigned int getCpuTimeUIntMS()
{
   // Get current hires count.
   long long int tCurrentHiresCount = my_get_hires_count();

   // Return the current program time in milliseconds.
   long long int tCurrentProgramTimeMS =
      (tCurrentHiresCount) / (1000 * 1000);

   return (unsigned int)(tCurrentProgramTimeMS & 0xffffffff);
}

// Return the current cpu time in milliseconds.
long long getCpuTimeLongLongMS()
{
   // Get current hires count.
   long long int tCurrentHiresCount = my_get_hires_count();

   // Return the current program time in milliseconds.
   long long int tCurrentProgramTimeMS =
      (tCurrentHiresCount) / (1000 * 1000);

   return tCurrentProgramTimeMS;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

