/*==============================================================================
==============================================================================*/
#include "stdafx.h"

#include <time.h>
#include <sys/time.h>

#include "risHiresTime.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current high resolution timer value in nanoseconds. 

unsigned long long int getCurrentHiresTime()
{
   struct timespec tTimespec;
   clock_gettime(CLOCK_MONOTONIC_RAW, &tTimespec);

   unsigned long long int tSeconds = (unsigned long long int)tTimespec.tv_sec;
   unsigned long long int tNanoseconds = (unsigned long long int)tTimespec.tv_nsec;
   unsigned long long int tTimeNs = tSeconds * 1000 * 1000 * 1000 + tNanoseconds;
   return tTimeNs;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

