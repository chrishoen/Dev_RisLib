/*==============================================================================
==============================================================================*/
#include "stdafx.h"

#include <windows.h> 
#include <conio.h>
#include <io.h>
#include <ProfileApi.h>

#include "risHiresTime.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class HiresTimeInitClass
{
public:
   LARGE_INTEGER mPerformanceFrequency;

   HiresTimeInitClass()
   {
      return;
      QueryPerformanceFrequency(&mPerformanceFrequency);
      printf("QueryPerformanceFrequency %lld\n", mPerformanceFrequency.QuadPart);
   }
};

HiresTimeInitClass gHiresTimeInitClass;
LARGE_INTEGER gPerformanceFrequency = gHiresTimeInitClass.mPerformanceFrequency;

//******************************************************************************
//******************************************************************************
//******************************************************************************



//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current high resolution timer value in nanoseconds. 

unsigned long long int getCurrentHiresTime()
{
   LARGE_INTEGER tPerformanceCount;
   LARGE_INTEGER tPerformanceFrequency;

   QueryPerformanceCounter(&tPerformanceCount);
   QueryPerformanceFrequency(&tPerformanceFrequency);

   if (false)
   {
      tPerformanceCount.QuadPart *= 1000000;
      tPerformanceCount.QuadPart /= tPerformanceFrequency.QuadPart;
      return tPerformanceCount.QuadPart * 1000;
   }
   else
   {
      double tDoubleCount = (double)tPerformanceCount.QuadPart;
      double tDoubleFrequency = (double)tPerformanceFrequency.QuadPart;
      double tDoubleTime = (tDoubleCount / tDoubleFrequency) * 1E9;
      return (unsigned long long int)tDoubleTime;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

// printf("QueryPerformanceCounter   %lld\n", tPerformanceCount.QuadPart);
// printf("QueryPerformanceFrequency %lld\n", tPerformanceFrequency.QuadPart);



