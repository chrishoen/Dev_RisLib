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

// Copy a file.
void copyFile(const char* aDestinFilePath, const char* aSourceFilePath);

// Move a file.
void moveFile(const char* aDestinFilePath, const char* aSourceFilePath);

// Delete a file.
void deleteFile(const char* aFilePath);

// Touch a file.
void touchFile(const char* aFilePath);

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

// chmod a file for 666.
void doFilePermissions666(const char* aFilePath);

// chmod a file for 666.
void doFilePermissions666(const std::string& aFilePath);

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Methods.

// Open a lock file and lock it. Return a file descriptor to the
// opened lock file.
int doLockFile_OpenAndLockForRead(const char* aLockName);
int doLockFile_OpenAndLockForWrite(const char* aLockName);

// Unlock on opened lock file and close it.
void doLockFile_UnlockAndClose(int aFileDesc);

// Open a file and lock it. Return a file handle.
FILE* doOpenAndLockForRead(const char* aFilePath);
FILE* doOpenAndLockForWrite(const char* aFilePath);
FILE* doOpenAndLockForReadWrite(const char* aFilePath);

// Unlock an opened and locked file and close it.
void doUnlockAndClose(FILE* aFile);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
