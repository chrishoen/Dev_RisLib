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
// Constructor

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

}//namespace

