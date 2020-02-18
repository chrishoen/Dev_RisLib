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
// Open a lock file and lock it file. Return a file descriptor to the
// opened lock file.

void* doLockFileOpenAndLock(const char* aLockName)
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

   return (void*)tFileDesc;
}     

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Unlock on opened lock file and close it.

void doLockFileUnlockAndClose(void* aFileDesc)
{
   int tFileDesc = (int)aFileDesc;

   struct flock lock;
   lock.l_type = F_WRLCK;    /* read/write (exclusive versus shared) lock */
   lock.l_whence = SEEK_SET; /* base for seek offsets */
   lock.l_start = 0;         /* 1st byte in file */
   lock.l_len = 0;           /* 0 here means 'until EOF' */
   lock.l_pid = getpid();    /* process id */

   fcntl(tFileDesc, F_SETLK, &lock);
   close(tFileDesc);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace
