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

char* portableGetSettingsDir()
{
   return "C:\\Prime\\Settings\\";
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
static char rCurrentWorkingDir[200];
char* portableGetCurrentWorkingDir()
{
   _getcwd(rCurrentWorkingDir,200);
   strcat(rCurrentWorkingDir,"\\");
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

}//namespace

