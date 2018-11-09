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
   // Exit if print not enabled.

   if (aLevel > tls()->mPrintLevel) return;

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Buffers.

   // Instantiate a local input string buffer and a pointer.
   char* tInputStr = 0;
   char  tInputBuffer[cMaxStringSize];
   int   tInputStrSize;
   tInputStr = &tInputBuffer[0];

   // Instantiate a local print string buffer and a pointer.
   char* tPrintStr = 0;
   char  tPrintBuffer[cMaxStringSize];
   int   tPrintStrSize;
   tPrintStr = &tPrintBuffer[0];

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Do a vsprintf with variable arg list into the input string pointer.

   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tInputStrSize = vsnprintf(tInputStr, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);

   tInputStr[tInputStrSize++] = 0;

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Do an sprintf with the thread name and the input string into the
   // print string.

   tPrintStrSize = sprintf(tPrintBuffer,"%-20s $$ %s",
         tls()->mThreadName,
         tInputStr);

   //*************************************************************************
   //*************************************************************************
   //*************************************************************************
   // Print the string.

   // Print to stdout.
   puts(tPrintStr);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

