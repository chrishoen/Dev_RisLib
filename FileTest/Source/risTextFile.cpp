/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "my_functions.h"
#include "risContainers.h"

#include "risTextFile.h"


namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Log file writer

LogFileWriter::LogFileWriter()
{
   mFile=0;
}

//******************************************************************************
// Open the file

bool LogFileWriter::open(char* aFilename)
{            
   mFile = fopen(aFilename,"w");

   if (mFile==0)
   {
      return false;
   }

   return true;
}

//******************************************************************************
// Close the file

void LogFileWriter::close()
{       
   if (mFile)
   {
      fclose(mFile);
   }
}


//******************************************************************************
// Write to the file

void LogFileWriter::write(const char* aFormat, ...)
{
   //-----------------------------------------------------
   // Do a vsprintf with variable arg list into print string

   int tPrintSize;
   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintSize = vsnprintf(mString, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   mString[tPrintSize++] = '\n';
   mString[tPrintSize++] = 0;

   //-----------------------------------------------------
   // Print the string

   fputs(mString,mFile);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// CSV file writer

CsvFileWriter::CsvFileWriter()
{
   mFile=0;
}

//******************************************************************************
// Open the file

bool CsvFileWriter::open(char* aFilename)
{            
   mFile = fopen(aFilename,"w");

   if (mFile==0)
   {
      return false;
   }

   return true;
}

//******************************************************************************
// Close the file

void CsvFileWriter::close()
{       
   if (mFile)
   {
      fclose(mFile);
   }
}


//******************************************************************************
// Write to the file

void CsvFileWriter::write(const char* aFormat, ...)
{
   //-----------------------------------------------------
   // Do a vsprintf with variable arg list into print string

   int tPrintSize;
   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintSize = vsnprintf(mString, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   mString[tPrintSize++] = '\n';
   mString[tPrintSize++] = 0;

   //-----------------------------------------------------
   // Print the string

   fputs(mString,mFile);
}

//******************************************************************************
// Write to the file

void CsvFileWriter::writeN(int aNumArgs, ...)
{
   va_list valist;
   va_start(valist,aNumArgs);
   for (int i=0;i<aNumArgs;i++)
   {
      double tValue = va_arg(valist,double);
      fprintf(mFile, "%f,",tValue);
   }
   va_end(valist);

   fprintf(mFile, "\n");
}

//******************************************************************************
// Write to the file

void CsvFileWriter::write(double aX1)
{
   fprintf(mFile, "%f,\n",aX1);
}

void CsvFileWriter::write(double aX1,double aX2)
{
   fprintf(mFile, "%f,%f,\n",aX1,aX2);
}

void CsvFileWriter::write(double aX1,double aX2,double aX3)
{
   fprintf(mFile, "%f,%f,%f,\n",aX1,aX2,aX3);
}

void CsvFileWriter::write(double aX1,double aX2,double aX3,double aX4)
{
   fprintf(mFile, "%f,%f,%f,%f,\n",aX1,aX2,aX3,aX4);
}


}//namespace

