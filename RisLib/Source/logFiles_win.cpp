/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "risPortableCalls.h"
#include "risSystemTime.h"
#include "my_functions.h"
#include "logFiles.h"

namespace Log
{

//****************************************************************************
//****************************************************************************
// Regional variables

   static const int cMaxStringSize = 400;
   static const int cMaxNumFiles = 200;

   FILE* mFile [cMaxNumFiles];
   bool  mTimestampEnable [cMaxNumFiles];

//****************************************************************************
//****************************************************************************
//****************************************************************************

void reset()
{
   for (int i = 0; i < cMaxNumFiles; i++)
   {
      mFile[i]=0;
      mTimestampEnable [i] = false;
   }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

bool openFile(int aLogNum, char* aFileName)
{            
   mFile[aLogNum] = fopen(aFileName,"w");

   if (mFile[aLogNum]==0)
   {
      return false;
   }

   return true;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

bool openFileAppend(int aLogNum, char* aFileName)
{            
   mFile[aLogNum] = fopen(aFileName,"a");

   if (mFile[aLogNum]==0)
   {
      return false;
   }

   return true;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

bool openFile(int aLogNum, char* aFileDir,char* aFileName)
{            
   // Construct file path.
   char tFilePath[cMaxStringSize];
   strcpy(tFilePath,aFileDir);
   strcat(tFilePath,"\\");
   strcat(tFilePath,aFileName);

   // Open file.
   return openFile(aLogNum,tFilePath);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

bool openFileAppend(int aLogNum, char* aFileDir,char* aFileName)
{            
   // Construct file path.
   char tFilePath[cMaxStringSize];
   strcpy(tFilePath,aFileDir);
   strcat(tFilePath,"\\");
   strcat(tFilePath,aFileName);

   // Open file.
   return openFileAppend(aLogNum,tFilePath);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void closeAllFiles()
{            
   for (int i = 0; i < cMaxNumFiles; i++)
   {
      if (mFile[i] != 0)
      {
         fclose(mFile[i]);
      }
      mFile[i]=0;
   }

}

//******************************************************************************
//******************************************************************************
// Write to the file

void write (int aLogNum, const char* aFormat, ...)
{
   //-----------------------------------------------------
   // Guard

   if (mFile[aLogNum]==0)
   {
      return;
   }

   //-----------------------------------------------------
   // Get system time string.

   char  tTimeString[40];
   tTimeString[0]=0;

   if (mTimestampEnable[aLogNum])
   {
      Ris::getCurrentSystemTimeAsString1(
         tTimeString,
         0,
         " >> ");
   }

   //-----------------------------------------------------
   // Do a vsprintf with variable arg list into print string

   char tPrintString[cMaxStringSize];

   int tPrintSize;
   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintSize = vsnprintf(tPrintString, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   tPrintString[tPrintSize++] = '\n';
   tPrintString[tPrintSize++] = 0;

   //-----------------------------------------------------
   // Print the string

   char tOutputString[cMaxStringSize];
   strcpy(tOutputString,tTimeString);
   strcat(tOutputString,tPrintString);
   fputs(tOutputString,mFile[aLogNum]);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to log file

void writeTimeStamp(int aLogNum, const char* aLabel)
{
   char tBuffer[40];
   write(aLogNum,"%s $$ %s",aLabel,my_timestamp(tBuffer));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// 
void enableTimeStamp(int aLogNum, bool aEnableFlag)
{
   mTimestampEnable [aLogNum] = aEnableFlag;
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

