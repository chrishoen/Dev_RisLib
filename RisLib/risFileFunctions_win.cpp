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

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write a json value to a file.

void doWriteJsonToFile(
   Json::Value& aJsonValue,    // Input
   const std::string& aFilePath)     // Input
{
   // Write the json value to the output file.
   std::ofstream tOutputFile(aFilePath, std::ofstream::out);
   Json::StyledStreamWriter tWriter;
   tWriter.write(tOutputFile, aJsonValue);
   tOutputFile.close();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a json value from a file.

void doReadJsonFromFile(
   Json::Value& aJsonValue,    // Output
   const std::string& aFilePath)     // Input
{
   // Read the json value from the input file.
   std::ifstream tInputFile(aFilePath);
   tInputFile >> aJsonValue;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace
