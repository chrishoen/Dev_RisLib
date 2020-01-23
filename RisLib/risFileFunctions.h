#pragma once

/*==============================================================================
File path utilities.
==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************
//
#include <string>
#include "json.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Filepath functions.

// Return a filepath that can safely be opened, one that has spaces.
// This encloses the input file path with double quotes. The returned
// file path can safely be passed to an fopen call.
char* getSafeFilePath(char* aBuffer, const char* aFilePath);

// Return a filepath that can safely be opened, one that has spaces.
// This encloses the input file path with double quotes. The returned
// file path can safely be passed to a std file open call.
std::string& getSafeFilePath(std::string& aBuffer, const std::string& aFilePath);

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Methods.

// Delete a file.
void deleteFile(const char* aFilePath);

// Touch a file.
void doTouchFile(const char* aFilePath);

// Write a json value to a file.
void doWriteJsonToFile(
   Json::Value& aJsonValue,           // Input
   const std::string& aFilePath);     // Input

// Read a json value from a file.
void doReadJsonFromFile(
   Json::Value& aJsonValue,           // Output
   const std::string& aFilePath);     // Input

// Get a unique file based on a random number.
std::string doGetRandomFileName();

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Methods.

// Open a lock file and lock it file. Return a file descriptor to the
// opened lock file.
void* doLockFileOpenAndLock(const char* aLockName);

// Unlock on opened lock file and close it.
void doLockFileUnlockAndClose(void* aFileDesc);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
