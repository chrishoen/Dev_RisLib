/*==============================================================================
Print utility
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

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
// Copy a file.

void copyFile(const char* aDestinFilePath, const char* aSourceFilePath)
{
   char* tString = new char[2000];
   sprintf(tString, "cp %s %s", aSourceFilePath, aDestinFilePath);
   Ris::doSystemCommand(tString);
   delete tString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Move a file.

void moveFile(const char* aDestinFilePath, const char* aSourceFilePath)
{
   char* tString = new char[2000];
   sprintf(tString, "mv %s %s", aSourceFilePath, aDestinFilePath);
   Ris::doSystemCommand(tString);
   delete tString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Delete a file.

void deleteFile(const char* aFilePath)
{
   char* tString = new char[2000];
   sprintf(tString, "rm %s", aFilePath);
   Ris::doSystemCommand(tString);
   delete tString;
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
// sprintf(tString, "chmod 666 %s", aFilePath);
// Ris::doSystemCommand(tString);
   delete tString;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Open a lock file and lock it. Return a file descriptor to the
// opened lock file.

int doLockFile_OpenAndLock(const char* aLockName)
{
   char tFilePath[200];
   sprintf(tFilePath, "/var/lock/%s", aLockName);
   int tFileDesc = open(tFilePath, O_RDWR | O_CREAT, 0666);

   struct flock lock;
   lock.l_type = F_WRLCK;    /* read/write (exclusive versus shared) lock */
   lock.l_whence = SEEK_SET; /* base for seek offsets */
   lock.l_start = 0;         /* 1st byte in file */
   lock.l_len = 0;           /* 0 here means 'until EOF' */
   lock.l_pid = getpid();    /* process id */

   fcntl(tFileDesc, F_SETLKW, &lock);

   return tFileDesc;
}     

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Unlock on opened lock file and close it.

void doLockFile_UnlockAndClose(int aFileDesc)
{
   struct flock lock;
   lock.l_type = F_WRLCK;    /* read/write (exclusive versus shared) lock */
   lock.l_whence = SEEK_SET; /* base for seek offsets */
   lock.l_start = 0;         /* 1st byte in file */
   lock.l_len = 0;           /* 0 here means 'until EOF' */
   lock.l_pid = getpid();    /* process id */

   fcntl(aFileDesc, F_SETLK, &lock);
   close(aFileDesc);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// chmod a file for 666.

void doFilePermissions666(const char* aFilePath)
{
   chmod(aFilePath, 0666);
}

// chmod a file for 666.
void doFilePermissions666(const std::string& aFilePath)
{
   chmod(aFilePath.c_str(), 0666);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace
