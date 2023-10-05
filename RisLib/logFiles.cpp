//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdarg.h>

#include "risPortableCalls.h"
#include "my_functions.h"
#include "risAlphaDir.h"
#include "logFiles.h"

namespace Log
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regional variables

static const int cMaxStringSize = 400;
static const int cMaxNumFiles = 200;

FILE* mFile [cMaxNumFiles];
bool  mTimestampEnable [cMaxNumFiles];

//******************************************************************************
//******************************************************************************
//******************************************************************************

void reset()
{
   for (int i = 0; i < cMaxNumFiles; i++)
   {
      mFile[i]=0;
      mTimestampEnable [i] = false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool openFile(int aLogNum, const char* aFilePath)
{            
   mFile[aLogNum] = fopen(aFilePath,"w");

   if (mFile[aLogNum]==0)
   {
      printf("Log::openFile OPEN FAIL %s\n", aFilePath);
      return false;
   }

   return true;
}

bool openFile(int aLogNum, std::unique_ptr<std::string> aFilePath)
{
   bool tPass = openFile(aLogNum, aFilePath->c_str());
   return tPass;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool openFileAppend(int aLogNum, const char* aFilePath)
{            
   mFile[aLogNum] = fopen(aFilePath, "a");

   if (mFile[aLogNum]==0)
   {
      printf("Log::openFile OPEN FAIL %s\n", aFilePath);
      return false;
   }

   return true;
}

bool openFileAppend(int aLogNum, std::unique_ptr<std::string> aFilePath)
{
   bool tPass = openFileAppend(aLogNum, aFilePath->c_str());
   return tPass;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void closeFile(int aLogNum)
{            
   if (mFile[aLogNum] != 0)
   {
      fclose(mFile[aLogNum]);
   }
   mFile[aLogNum]=0;
}

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
//******************************************************************************
// Write to the file

void write(int aLogNum, const char* aFormat, ...)
{
   //-----------------------------------------------------
   // Guard

   if (mFile[aLogNum] == 0)
   {
      return;
   }

   //-----------------------------------------------------
   // Do a vsprintf with variable arg list into print string

   char tPrintString[cMaxStringSize];

   int tPrintSize;
   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintSize = vsnprintf(tPrintString, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   if (tPrintSize < cMaxStringSize - 3)
   {
      tPrintString[tPrintSize++] = '\n';
      tPrintString[tPrintSize++] = 0;
   }

   //-----------------------------------------------------
   // Print the string

   fputs(tPrintString, mFile[aLogNum]);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the file

void write2 (int aLogNum, const char* aFormat, ...)
{
   //-----------------------------------------------------
   // Guard

   if (mFile[aLogNum]==0)
   {
      return;
   }

   //-----------------------------------------------------
   // Do a vsprintf with variable arg list into print string

   char* tString1 = new char[cMaxStringSize + 1];
   char* tString2 = new char[cMaxStringSize + 1];

   int tPrintSize;
   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintSize = vsnprintf(tString1, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);
   tString1[cMaxStringSize] = 0;

   if (tPrintSize < cMaxStringSize - 3)
   {
      tString1[tPrintSize++] = '\n';
      tString1[tPrintSize++] = 0;
   }

   //-----------------------------------------------------
   // Add a timestamp.

   char tTimetag[40];
   my_timestamp2(tTimetag);
   sprintf(tString2, "%s %s", tTimetag, tString1);


   //-----------------------------------------------------
   // Print the string

   fputs(tString2, mFile[aLogNum]);
   fflush(mFile[aLogNum]);

   //-----------------------------------------------------
   // done

   delete[] tString1;
   delete[] tString2;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the file

void write3(int aLogNum, const char* aFormat, ...)
{
   //-----------------------------------------------------
   // Guard

   if (mFile[aLogNum] == 0)
   {
      return;
   }

   //-----------------------------------------------------
   // Do a vsprintf with variable arg list into print string

   char* tString1 = new char[cMaxStringSize + 1];
   char* tString2 = new char[cMaxStringSize + 1];

   int tPrintSize;
   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintSize = vsnprintf(tString1, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);
   tString1[cMaxStringSize] = 0;

   if (tPrintSize < cMaxStringSize - 3)
   {
      tString1[tPrintSize++] = '\n';
      tString1[tPrintSize++] = 0;
   }

   //-----------------------------------------------------
   // Add a timestamp.

   char tTimetag[40];
   my_timestamp3(tTimetag);
   sprintf(tString2, "%s %s", tTimetag, tString1);


   //-----------------------------------------------------
   // Print the string

   fputs(tString2, mFile[aLogNum]);
   fflush(mFile[aLogNum]);

   //-----------------------------------------------------
   // done

   delete[] tString1;
   delete[] tString2;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

