/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risFilePath.h"

//****************************************************************************
//****************************************************************************
//****************************************************************************

namespace Ris
{

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Return a filepath that can safely be opened, one that has spaces.
// This encloses the input file path with double quotes. The returned
// file path can safely be passed to an fopen call.

int gCode = 1;

char* getSafeFilePath(char* aBuffer, const char* aFilePath)
{
   if (gCode == 0)
   {
      sprintf(aBuffer, "%s", aFilePath);
   }
   else if (gCode == 1)
   {
      sprintf(aBuffer, "\"%s\"", aFilePath);
   }
   else if (gCode == 2)
   {
      int j = 0;
      for (int i = 0; i < strlen(aFilePath); i++)
      {
         if (aFilePath[i] == ' ')
         {
            aBuffer[j++] = 20;
         }
         aBuffer[j++] = aFilePath[i];
      }
      aBuffer[j] = 0;
   }
   return aBuffer;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Return a filepath that can safely be opened, one that has spaces.
// This encloses the input file path with double quotes. The returned
// file path can safely be passed to a std file open call.

std::string& getSafeFilePath(std::string& aBuffer, const std::string& aFilePath)
{
   if (gCode == 0)
   {
      aBuffer = aFilePath;
   }
   else if (gCode == 1)
   {
      aBuffer = "\"" + aFilePath + "\"";
   }
   return aBuffer;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace
