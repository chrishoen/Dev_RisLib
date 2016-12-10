/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h> 
#include <direct.h> 
#include <string.h> 
#include <stdio.h>
#include <conio.h>
#include <direct.h>
#include <io.h>

#include "risPortableCalls.h"

namespace Ris
{

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

const char* portableGetSettingsDir()
{
   return "C:\\Alpha\\Settings\\";
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

const char* portableGetHomeDir()
{
   return "C:\\RedInc\\";
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
static char rCurrentWorkingDir[200];

char* portableGetCurrentWorkingDir()
{
   int tIndex = GetModuleFileName(NULL, rCurrentWorkingDir, 200);

   while (--tIndex > 0)
   {
      if (rCurrentWorkingDir[tIndex] == '\\')
      {
         rCurrentWorkingDir[tIndex + 1] = 0;
         break;
      }
   }

   return rCurrentWorkingDir;
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

long long int portableGetHiResCounter()
{
   LARGE_INTEGER tPerformanceCount;
   QueryPerformanceCounter(&tPerformanceCount);
   return tPerformanceCount.QuadPart;
}

long long int portableGetHiResFrequency()
{
   LARGE_INTEGER tPerformanceFrequency;
   QueryPerformanceFrequency(&tPerformanceFrequency);
   return tPerformanceFrequency.QuadPart;
}

//******************************************************************************
void portableStrupr(char* aString)
{
   strupr(aString);
}
//******************************************************************************

void portableSetConsoleTitle(char* aTitle)
{
   SetConsoleTitle(aTitle);
}

//******************************************************************************

bool portableFilePathExists (char* aFilePath)
{
   return  _access(aFilePath, 0) == 0;
}

}//namespace

