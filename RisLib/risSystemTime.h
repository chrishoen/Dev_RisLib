#pragma once

/*==============================================================================
System time functions.
==============================================================================*/

#include <Windows.h>

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

namespace Ris
{
   // Read current time and return it as a formatted string. This is given
   // an input string as memory to write to. It returns the input string.
   // It is also given prefix and postfix string which, if not null, are
   // prepended and appended to the formatted string.
   char* getCurrentSystemTimeAsString1(
      char* aInputStr,
      char* aPrefixStr,
      char* aPostfixStr);

   char* getCurrentSystemTimeAsString2(
      char* aInputStr,
      char* aPrefixStr,
      char* aPostfixStr);

   // Convert a system time and return it as a formatted string.
   char* getSystemTimeAsString1(
      SYSTEMTIME* aSystemTime,
      char* aInputStr,
      char* aPrefixStr,
      char* aPostfixStr);

   char* getSystemTimeAsString2(
      SYSTEMTIME* aSystemTime,
      char* aInputStr,
      char* aPrefixStr,
      char* aPostfixStr);

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
}//namespace

