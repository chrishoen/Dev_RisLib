/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 
#include <conio.h>
#include <io.h>
#include <direct.h>
#include <time.h>
#include <ctime.h>

#include "risPortableCalls.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regionals

bool rAbortWaitForKbhit=false;

//******************************************************************************
//******************************************************************************
//******************************************************************************

void portableSleep(int aTicks)
{
   Sleep(aTicks);  
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void portableHalt(const char* aString)
{
   printf("PROGRAM HALT PROGRAM HALT PROGRAM HALT PROGRAM HALT\n");
   if (aString)
   {
      printf("%s\n", aString);
   }

   while (true)
   {
      Sleep(1000);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
static char rProgramDir[400];

char* portableGetProgramDir()
{
   int tIndex = GetModuleFileName(NULL, rProgramDir, 400);

   while (--tIndex > 0)
   {
      if (rProgramDir[tIndex] == '\\')
      {
         rProgramDir[tIndex + 1] = 0;
         break;
      }
   }

   return rProgramDir;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
static char rCurrentDir[400];

char* portableGetCurrentDir()
{
   GetCurrentDirectory(400,rCurrentDir);
   strcat(rCurrentDir,"\\");
   return rCurrentDir;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void portableWaitForKbhit()
{
   while(!kbhit())
   {
      if (rAbortWaitForKbhit) return;;
      Sleep(100);
   }
}

bool portableKbhit()
{
   return kbhit()!=0;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

unsigned long long int portableGetHiResCounter()
{
   LARGE_INTEGER tPerformanceCount;
   QueryPerformanceCounter(&tPerformanceCount);
   return tPerformanceCount.QuadPart;
}

unsigned long long int portableGetHiResFrequency()
{
   LARGE_INTEGER tPerformanceFrequency;
   QueryPerformanceFrequency(&tPerformanceFrequency);
   return tPerformanceFrequency.QuadPart;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void portableStrupr(char* aString)
{
   strupr(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool portableFilePathExists (const char* aFilePath)
{
   return  _access(aFilePath, 0) == 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the program current working directory.

void portableChdir(const char* aFilePath)
{
   BOOL tRet = SetCurrentDirectory(aFilePath);
   if (!tRet) printf("portableChdir FAIL %s", aFilePath);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current time as a string.

char* portableGetCurrentTimeAsString(char* aBuffer, bool aSecondDecimals)
{
   // Get the current time.
   struct timespec tSystemTime;
   timespec_get(&tSystemTime, TIME_UTC);

   // Print the time to a string with decimals in the seconds.
   if (aSecondDecimals)
   {
      char tTemp[40];
      strftime(tTemp, 40, "%F %T", localtime(&tSystemTime.tv_sec));
      sprintf(aBuffer, "%s.%03ld", tTemp, tSystemTime.tv_nsec / 1000000);
   }
   // Print the time to a string without decimals in the seconds.
   else
   {
      strftime(aBuffer, 40, "%F %T", localtime(&tSystemTime.tv_sec));
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

