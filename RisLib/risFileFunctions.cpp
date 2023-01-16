/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"
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
      for (int i = 0; i < (int)strlen(aFilePath); i++)
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
   Json::StreamWriterBuilder tBuilder;
   Json::StreamWriter* tWriter = tBuilder.newStreamWriter();
   tWriter->write(aJsonValue, &tOutputFile);
   delete tWriter;
   tOutputFile.close();
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
   Json::CharReaderBuilder tBuilder;
   Json::CharReader* tReader = tBuilder.newCharReader();
   std::string tError;
   if (!Json::parseFromStream(tBuilder, tInputFile, &aJsonValue, &tError))
   {
      printf("doReadJsonFromFile %s", tError.c_str());
   }
   delete tReader;
   tInputFile.close();
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace
