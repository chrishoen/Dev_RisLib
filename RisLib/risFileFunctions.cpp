/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"
#include <sys/stat.h>
#include <random>

#include "risSystemCalls.h"
#include "risFileFunctions.h"

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

//******************************************************************************
//******************************************************************************
//******************************************************************************
// get a unique file based on a random number.

std::string doGetRandomFileName()
{
   std::random_device tRandomDevice;
   std::mt19937 tRandomGenerator(tRandomDevice());
   std::uniform_int_distribution<> tRandomDistribution(0,1000*1000*1000);
   int tRandomInt = tRandomDistribution(tRandomGenerator);
   char tFileName[40];
   sprintf(tFileName, "some_file_%d", tRandomInt);
   return std::string(tFileName);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write a json value to a file.

void doWriteJsonToFile(
   Json::Value& aJsonValue,          // Input
   const std::string& aFilePath)     // Input
{
   // Write the json value to the output file.
   std::ofstream tOutputFile(aFilePath, std::ofstream::out);
   Json::StyledStreamWriter tWriter;
   tWriter.write(tOutputFile, aJsonValue);
   tOutputFile.close();
   // Set permissions.
   chmod(aFilePath.c_str(), 0666);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a json value from a file.

void doReadJsonFromFile(
   Json::Value& aJsonValue,          // Output
   const std::string& aFilePath)     // Input
{
   // Read the json value from the input file.
   std::ifstream tInputFile(aFilePath);
   tInputFile >> aJsonValue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// chmod a file for 666.

void doFilePermissions666(const char* aFilePath)
{
   char tString[200];
   sprintf(tString, "sudo chmod 666 %s", aFilePath);
   Ris::doSystemCommand(tString);
   chmod(aFilePath, 0666);
}

// chmod a file for 666.
void doFilePermissions666(std::string& aFilePath)
{
   char tString[200];
   sprintf(tString, "sudo chmod 666 %s", aFilePath.c_str());
   Ris::doSystemCommand(tString);
   chmod(aFilePath.c_str(), 0666);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace
