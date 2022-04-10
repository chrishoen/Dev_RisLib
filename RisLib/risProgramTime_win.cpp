/*==============================================================================
==============================================================================*/

#include "stdafx.h"

#include <windows.h> 
#include <ProfileApi.h>

#include "risProgramTime.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regionals.

// Return high performance timer frequency.
double my_get_hires_frequency()
{
   LARGE_INTEGER tPerformanceFrequency;
   QueryPerformanceFrequency(&tPerformanceFrequency);
   return (double)tPerformanceFrequency.QuadPart;
}

// Return high performance timer count.
double my_get_hires_count()
{
   LARGE_INTEGER tPerformanceCount;
   QueryPerformanceCounter(&tPerformanceCount);
   return (double)tPerformanceCount.QuadPart;
}

// High performance clock period.
double gHiresPeriod = 1.0/my_get_hires_frequency();

// High performance clock count at program startup.
double gProgramStartHiresCount = my_get_hires_count();
   
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current program time in seconds.

double getProgramTime()
{
   // Get current hires count.
   double tCurrentHiresCount = my_get_hires_count();

   // Return the current program time in seconds.
   return (tCurrentHiresCount - gProgramStartHiresCount)*gHiresPeriod;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current program time in milliseconds.

double getProgramTimeMS()
{
   return getProgramTime()*1E3;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current program time in microseconds.

double getProgramTimeUS()
{
   return getProgramTime()*1E6;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current program time in nanoseconds.

long long int getProgramTimeNS()
{
   return (long long int)(getProgramTime()*1E9);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current cpu time in milliseconds.

unsigned int getCpuTimeUIntMS()
{
   return (unsigned int)getProgramTimeMS();
}

long long getCpuTimeLongLongMS()
{
   return (long long)getProgramTimeMS();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

