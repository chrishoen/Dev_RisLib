/*==============================================================================
==============================================================================*/

#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "risPortableCalls.h"
#include "risProgramTime.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

char* getCurrentSystemTimeAsString1(
   char* aInputStr,
   char* aPrefixStr,
   char* aPostfixStr)
{
   // Get the system time.
   SYSTEMTIME tSystemTime;
   GetSystemTime(&tSystemTime);

   // Write to a temp string with a formateted system time.
   char tTempStr[40];
   sprintf(tTempStr,"%4d.%02d.%02d_%02d:%02d:%02d",
      tSystemTime.wYear,
      tSystemTime.wMonth,
      tSystemTime.wDay,
      tSystemTime.wHour,
      tSystemTime.wMinute,
      tSystemTime.wSecond);

   // Add the prefix string.
   if (aPrefixStr)
   {
      strcpy(aInputStr, aPrefixStr);
      strcat(aInputStr, tTempStr);
   }
   else
   {
      strcpy(aInputStr, tTempStr);
   }

   // Add the postfix string.
   if (aPostfixStr)
   {
      strcat(aInputStr, aPostfixStr);
   }

   // Return the input string.
   return aInputStr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* getCurrentSystemTimeAsString2(
   char* aInputStr,
   char* aPrefixStr,
   char* aPostfixStr)
{
   // Get the system time.
   SYSTEMTIME tSystemTime;
   GetSystemTime(&tSystemTime);

   // Write to a temp string with a formateted system time.
   char tTempStr[40];
   sprintf(tTempStr,"%4d_%02d_%02d_%02d:%02d:%02d",
      tSystemTime.wYear,
      tSystemTime.wMonth,
      tSystemTime.wDay,
      tSystemTime.wHour,
      tSystemTime.wMinute,
      tSystemTime.wSecond);

   // Add the prefix string.
   if (aPrefixStr)
   {
      strcpy(aInputStr, aPrefixStr);
      strcat(aInputStr, tTempStr);
   }
   else
   {
      strcpy(aInputStr, tTempStr);
   }

   // Add the postfix string.
   if (aPostfixStr)
   {
      strcat(aInputStr, aPostfixStr);
   }

   // Return the input string.
   return aInputStr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

char* getSystemTimeAsString1(
   SYSTEMTIME* aSystemTime,
   char* aInputStr,
   char* aPrefixStr,
   char* aPostfixStr)
{
   // Get the system time.
   SYSTEMTIME tSystemTime = *aSystemTime;

   // Write to a temp string with a formateted system time.
   char tTempStr[40];
   sprintf(tTempStr,"%4d.%02d.%02d_%02d:%02d:%02d",
      tSystemTime.wYear,
      tSystemTime.wMonth,
      tSystemTime.wDay,
      tSystemTime.wHour,
      tSystemTime.wMinute,
      tSystemTime.wSecond);

   // Add the prefix string.
   if (aPrefixStr)
   {
      strcpy(aInputStr, aPrefixStr);
      strcat(aInputStr, tTempStr);
   }
   else
   {
      strcpy(aInputStr, tTempStr);
   }

   // Add the postfix string.
   if (aPostfixStr)
   {
      strcat(aInputStr, aPostfixStr);
   }

   // Return the input string.
   return aInputStr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* getSystemTimeAsString2(
   SYSTEMTIME* aSystemTime,
   char* aInputStr,
   char* aPrefixStr,
   char* aPostfixStr)
{
   // Get the system time.
   SYSTEMTIME tSystemTime = *aSystemTime;

   // Write to a temp string with a formateted system time.
   char tTempStr[40];
   sprintf(tTempStr,"%4d_%02d_%02d_%02d:%02d:%02d",
      tSystemTime.wYear,
      tSystemTime.wMonth,
      tSystemTime.wDay,
      tSystemTime.wHour,
      tSystemTime.wMinute,
      tSystemTime.wSecond);

   // Add the prefix string.
   if (aPrefixStr)
   {
      strcpy(aInputStr, aPrefixStr);
      strcat(aInputStr, tTempStr);
   }
   else
   {
      strcpy(aInputStr, tTempStr);
   }

   // Add the postfix string.
   if (aPostfixStr)
   {
      strcat(aInputStr, aPostfixStr);
   }

   // Return the input string.
   return aInputStr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

