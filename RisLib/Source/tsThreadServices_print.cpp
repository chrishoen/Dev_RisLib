//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdarg.h>

#include "my_functions.h"
#include "risAlphaDir.h"

#include "tsShare.h"
#include "tsThreadServices.h"

namespace TS
{

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Filtered print, if the corresponding entry in the filter table is true
// then the print is executed.

void print(int aLevel, const char* aFormat, ...)
{
   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Strings.

   // Input string buffer.
   char  tInputString[cMaxStringSize];
   int   tInputSize;

   // Print string buffer.
   char  tPrintString[cMaxStringSize];
   int   tPrintSize;

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Do a vsprintf with variable arg list into the input string buffer.

   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tInputSize = vsnprintf(tInputString, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Do an sprintf with the thread name and the input string into the
   // print string. Append a newline \n.

   tPrintSize = sprintf(tPrintString,"%-20s $$ %s\n",
      tls()->mThreadName,
      tInputString);

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Print the string.

   // Print to stdout.
   if (aLevel <= tls()->mPrintLevel)
   {
      fputs(tPrintString, stdout);
//    puts(tPrintString);
   }

   // Print to the log file.
   if (gShare.mLogFile)
   {
      fputs(tPrintString, gShare.mLogFile);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

