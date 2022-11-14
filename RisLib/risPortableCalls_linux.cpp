/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include <time.h>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "risPortableCalls.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regionals

   
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sleep

void portableSleep(int aTicks)
{
   usleep(aTicks*1000);
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
      usleep(1000 * 1000);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
static char rProgramDir[1000];

char* portableGetBinDir()
{
   readlink("/proc/self/exe", rProgramDir, 1000);

   bool tGoing = true;
   int tIndex = strlen(rProgramDir) - 1;
   while (tGoing)
   {
      if (rProgramDir[tIndex] == '/')
      {
         rProgramDir[tIndex + 1] = 0;
         tGoing = false;
      }
      if (--tIndex == 0) tGoing = false;
   }

   return rProgramDir;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
static char rCurrentDir[1000];

char* portableGetCurrentDir()
{
   getcwd(rCurrentDir,1000);
   strcat(rCurrentDir, "/");
   return rCurrentDir;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void portableWaitForKbhit()
{
   // Locals
   struct termios tAttribSave;
   struct termios tAttribNew;
   fd_set tReadFs;

   // Save current attributes
   tcgetattr( STDIN_FILENO, &tAttribSave);
   // Set new attributes
   tAttribNew = tAttribSave;
   tAttribNew.c_lflag &= ~( ICANON );
   tcsetattr( STDIN_FILENO, TCSANOW, &tAttribNew);
   // Wait for keypressed
   FD_ZERO(&tReadFs);
   FD_SET (STDIN_FILENO, &tReadFs);
   select(STDIN_FILENO+1, &tReadFs, NULL, NULL, NULL);
   // Restore saved attributes
   tcsetattr( STDIN_FILENO, TCSANOW, &tAttribSave);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool portableKbhit()
{
   // Locals
   struct termios tAttribSave;
   struct termios tAttribNew;
   fd_set         tReadFs;
   struct timeval tTimeval;
   
   // Save current attributes
   tcgetattr( STDIN_FILENO, &tAttribSave);
   // Set new attributes
   tAttribNew = tAttribSave;
   tAttribNew.c_lflag &= ~( ICANON );
 //tcsetattr( STDIN_FILENO, TCSANOW, &tAttribNew);
   // Wait for keypressed
   FD_ZERO(&tReadFs);
   FD_SET (STDIN_FILENO, &tReadFs);
   tTimeval.tv_sec=0;
   tTimeval.tv_usec=0;
   int tStatus=select(STDIN_FILENO+1, &tReadFs, NULL, NULL, &tTimeval);
   // Restore saved attributes
 //tcsetattr( STDIN_FILENO, TCSANOW, &tAttribSave);

   if (tStatus != 0)
   {
      getc(stdin);
      return true;
   }

   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

unsigned long long int portableGetHiResCounter()
{
   struct timespec tTimespec;
   clock_gettime(CLOCK_MONOTONIC, &tTimespec);

   unsigned long long int tSeconds = (unsigned long long int)tTimespec.tv_sec;
   unsigned long long int tNanoseconds = (unsigned long long int)tTimespec.tv_nsec;
   unsigned long long int tTimeNs = tSeconds * 1000 * 1000 * 1000 + tNanoseconds;
   return tTimeNs;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

unsigned long long int portableGetHiResFrequency()
{
   return 1000 * 1000 * 1000;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Convert string to upper case

void portableStrupr(char* aString)
{
   unsigned char* tPtr = (unsigned char*)aString;
   int tIndex = 0;
   while (tPtr[tIndex] != 0)
   {
      int tValue = (int)tPtr[tIndex];
      if ((tValue >= 97) && (tValue <= 122))
      {
         tValue -= 32;
         tPtr[tIndex] = (unsigned char)tValue;
      }
      tIndex++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool portableFilePathExists(const char* aFilePath)
{
   struct stat tStat;
   return (stat(aFilePath, &tStat) == 0);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Fill the input buffer with a full file path, removing relative
// references (../../ etc). If the input buffer is null then
// create one. Also return the filled buffer.

char* portableFullPath(char* aBuffer, const char* aFilePath)
{
   return realpath(aFilePath, aBuffer);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the program current working directory.

void portableChdir(const char* aFilePath)
{
   int tRet = chdir(aFilePath);
   if (tRet) printf("portableChdir FAIL %s", aFilePath);
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
   delete[] tString;
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
   //Done.
   return aBuffer;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return true if running windows, false if running linux.

bool portableIsWindows() { return false; }

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

