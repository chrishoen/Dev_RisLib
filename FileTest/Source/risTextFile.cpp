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
// Read from the file

bool CsvFileReader::read(char* aFilename)
{            
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Initialize

   strcpy(mDelimiters," ,\t");

   mValidFlag = false;

   //---------------------------------------------------------------------------
   // Open the file

   mFile = fopen(aFilename,"r");

   if (mFile==0)
   {
      return false;
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Read from the file to get the number of rows and columns

   mRows=0;
   mCols=0;
   while (true)
   {
      // Read each row
      if (fgets(mString, cMaxStringSize, mFile) != NULL)
      {
         mRows++;

         // Trim the end of the string
         trimString(mString);

         // Read tokens in the row to get the number of columns
         if (mRows == 1)
         {
            char* tPtr = strtok (mString,mDelimiters);
            while (tPtr != NULL)
            {
               tPtr = strtok (NULL,mDelimiters);
               mCols++;
            }
         }
      }
      else
      {
         break;
      }
   }

   // Rewind the file
   rewind(mFile);

   printf("READING %s %d %d\n",aFilename,mRows,mCols);

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Allocate memory for the table

   mValues = new double[mRows*mCols];

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Read from the file to get the table values

   // For all of the rows
   for (int tRow=0;tRow<mRows;tRow++)
   {
      // Read the row
      if (fgets(mString, cMaxStringSize, mFile) == NULL)
      {
         printf("FILE READ ERROR 101\n");
         return false;
      }

      // Trim the end of the string
      trimString(mString);

      // Extract column values for the row
      int tCol=0;
      char* tPtr = strtok (mString,mDelimiters);
      while (tPtr != NULL)
      {
         e(tRow, tCol++) = atof(tPtr);
         tPtr = strtok (NULL,mDelimiters);
      }
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Done

   mValidFlag = true;
   return true;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

double& CsvFileReader::e (int aRow,int aCol)
{
   int tIndex = (aRow)*mCols + (aCol);
   return mValues[tIndex];
}

double& CsvFileReader::operator() (int aRow,int aCol)
{
   int tIndex = (aRow)*mCols + (aCol);
   return mValues[tIndex];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CsvFileReader::show(int aRow)
{
   if (!mValidFlag)
   {
      printf("CsvFileReader NOT VALID\n");
      return;
   }

   for (int j=0;j<mCols;j++)
   {
      printf("%12.8f ",e(aRow,j));
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

