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
static char rProgramDir[1000];

char* portableGetBinDir()
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
static char rCurrentDir[1000];

char* portableGetCurrentDir()
{
   GetCurrentDirectory(1000,rCurrentDir);
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
// Fill the input buffer with a full file path, removing relative
// references (../../ etc). If the input buffer is null then
// create one. Also return the filled buffer.

char* portableFullPath(char* aBuffer, const char* aFilePath)
{
   if (aBuffer == 0) aBuffer = (char*)malloc(_MAX_PATH);
   return _fullpath(aBuffer, aFilePath, _MAX_PATH);
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
// Set the program current working directory up one level from the 
// program bin directory.

void portableChdirUpFromBin()
{
   char* tString = new char[1000];
   strncpy(tString, portableGetBinDir(), 999);
   tString[999] = 0;
   strncat(tString, "..", 998);
   tString[998] = 0;
   portableChdir(tString);
   delete[] tString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the program current working directory to the program bin
// directory.

void portableChdirToBin()
{
   char* tString = new char[1000];
   strncpy(tString, portableGetBinDir(), 999);
   tString[999] = 0;
   portableChdir(tString);
   delete tString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the current time as a string.

char* portableGetCurrentTimeAsString(char* aBuffer, bool aSecondDecimals)
{
   // declaring argument of time() 
   time_t my_time = time(NULL);

   // ctime() used to give the present time 
   sprintf(aBuffer,"%s", ctime(&my_time));

   return aBuffer;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return true if running windows, false if running linux.

bool portableIsWindows() { return true; }


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

