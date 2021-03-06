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

double getCurrentProgramTime()
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

double getCurrentProgramTimeMS()
{
   return getCurrentProgramTime()*1E3;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current program time in microseconds.

double getCurrentProgramTimeUS()
{
   return getCurrentProgramTime()*1E6;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current program time in nanoseconds.

long long int getCurrentProgramTimeNS()
{
   return (long long int)(getCurrentProgramTime()*1E9);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

