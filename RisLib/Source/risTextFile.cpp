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

void CsvFileWriter::writeRow(const char* aFormat, ...)
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

void CsvFileWriter::writeRowN(int aNumArgs, ...)
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

void CsvFileWriter::writeRow(double aX1)
{
   fprintf(mFile, "%f,\n",aX1);
}

void CsvFileWriter::writeRow(double aX1,double aX2)
{
   fprintf(mFile, "%f,%f,\n",aX1,aX2);
}

void CsvFileWriter::writeRow(double aX1,double aX2,double aX3)
{
   fprintf(mFile, "%f,%f,%f,\n",aX1,aX2,aX3);
}

void CsvFileWriter::writeRow(double aX1,double aX2,double aX3,double aX4)
{
   fprintf(mFile, "%f,%f,%f,%f,\n",aX1,aX2,aX3,aX4);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// CSV file reader

CsvFileReader::CsvFileReader()
{
   mFile=0;
}

//******************************************************************************
// Open the file

bool CsvFileReader::open(char* aFilename)
{            
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Initialize

   strcpy(mDelimiters," ,\t");

   mFile = fopen(aFilename,"r");

   if (mFile==0)
   {
      return false;
   }

   return true;
}

//******************************************************************************
// Close the file

void CsvFileReader::close()
{       
   if (mFile)
   {
      fclose(mFile);
   }
}

//******************************************************************************
// Read from the file

bool CsvFileReader::readRow()
{            
   mCols=0;
   // Read the row
   if (fgets(mString, cMaxStringSize, mFile) == NULL)
   {
      return false;
   }

   // Trim the end of the string
   trimString(mString);

   // Extract column values for the row
   char* tPtr = strtok (mString,mDelimiters);
   while (tPtr != NULL)
   {
      e(mCols++) = atof(tPtr);
      tPtr = strtok (NULL,mDelimiters);
   }

   return true;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

double& CsvFileReader::e (int aCol)
{
   return mValues[aCol];
}

double& CsvFileReader::operator() (int aCol)
{
   return mValues[aCol];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CsvFileReader::show()
{
   for (int j=0;j<mCols;j++)
   {
      printf("%12.8f ",e(j));
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Helpers

void CsvFileReader::trimString(char* aString)
{
   // Remove any character that is not a digit at end of the string
   unsigned tStringLen = (unsigned)strlen(aString);

   int tIndex=tStringLen-1;
   while (true)
   {
      if (isdigit(aString[tIndex]))
      {
         break;
      }
      else
      {
         aString[tIndex]=0;
         if (--tIndex==0) break;
      }
   }
}

}//namespace

