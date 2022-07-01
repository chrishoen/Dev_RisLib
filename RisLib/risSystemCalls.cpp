/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include <iostream>
#include "prnPrint.h"
#include "risFileFunctions.h"
#include "risSystemCalls.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regionals

   
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute a system command. return zero if successful.

int doSystemCommand(const char* aCommand)
{
   int tRet = system(aCommand);
   if (tRet)
   {
      printf("doSystemCommand FAIL1 %s\n",aCommand);
      return -1;
   }
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute a system command. return zero if successful.

int doSystemCommandSuppress(const char* aCommand)
{
   char* tString = new char[1000];
   strcpy(tString, aCommand);
   strcat(tString, " >/dev/null 2>&1");
   int tRet = system(tString);
   delete[] tString;
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute a system command. return zero if successful.
// Also return the output of the command into a list of strings.

int doSystemCommand(const char* aCommand, std::vector<std::string>& aResponse)
{
   // Do this first.
   aResponse.clear();

   // Get a random temp file name.
   std::string tTempFileName = doGetRandomFileName();

   // Construct a command string from the input command string and redirect
   // the command output to the random temp file.
   char* tString = new char[1000];
   sprintf(tString, "%s > %s", aCommand, tTempFileName.c_str());
  
   // Execute the system command with the output redireced to the random
   // temp file.
   int tRet = system(tString);
   // Read the system command response into the output response list.
   std::ifstream tTempFileStream(tTempFileName);
   for (std::string tFileLine; std::getline(tTempFileStream, tFileLine); )
   {
      aResponse.push_back(tFileLine);
   }

   // Delete the temp file.
   deleteFile(tTempFileName.c_str());
   delete[] tString;
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

