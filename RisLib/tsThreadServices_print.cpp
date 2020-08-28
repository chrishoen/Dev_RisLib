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
// Print. This is input a print level 0,1,2 and the signature for a
// printf. If the input level is less than or equal to the print level
// that is located in thread local storage then the print is performed
// else it is ignored. If the print is performed it prints a string
// to stdout and and to the log file.

void print(int aLevel, const char* aFormat, ...)
{
   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Do this first.

   // Guard.
   if (!gShare.mPrintEnableFlag) return;
   if (!isEnabled()) return;
   if (aLevel > tls()->mPrintLevel) return;

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Strings.

   // Input string buffer.
   char  tInputString[cMaxStringSize];
   int   tInputSize;

   // Print string buffer.
   char  tOutputString[cMaxStringSize];
   int   tOutputSize;

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

   tOutputSize = sprintf(tOutputString,"%-20s $$ %s",
      tls()->mThreadName,
      tInputString);

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Print the string.

   puts(tOutputString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

