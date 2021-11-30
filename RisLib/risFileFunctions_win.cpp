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
// Delete a file.

void deleteFile(const char* aFilePath)
{
   char* tString = new char[2000];
   sprintf(tString, "del %s", aFilePath);
   Ris::doSystemCommand(tString);
   delete[] tString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Touch a file.

void touchFile(const char* aFilePath)
{
   char* tString = new char[2000];
   sprintf(tString, "touch %s", aFilePath);
   Ris::doSystemCommand(tString);
   delete[] tString;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Open a lock file and lock it. Return a file descriptor to the
// opened lock file.

int doLockFile_OpenAndLock(const char* aLockName)
{
   return 0;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Unlock on opened lock file and close it.

void doLockFile_UnlockAndClose(int aFileDesc)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// chmod a file for 666.

void doFilePermissions666(const char* aFilePath)
{
}

// chmod a file for 666.
void doFilePermissions666(const std::string& aFilePath)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
