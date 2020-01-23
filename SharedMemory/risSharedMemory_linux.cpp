/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "risSystemCalls.h"
#include "risFileFunctions.h"
#include "risSharedMemory.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// pimpl idiom.
 
class SharedMemory::Specific
{
public:
   key_t mKey;
   int mFd;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SharedMemory::SharedMemory()
{
   mSpecific = new Specific;
   mNumBytes = 0;
   mMemory = 0;
}

SharedMemory::~SharedMemory()
{
   delete mSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// If the shared memory region does not already exist, then create it and
// return true. If it does already exist, then open it and return false.

bool SharedMemory::initialize(const char* aName, int aNumBytes)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Store these.
   int tRet = 0;
   mMemory = 0;
   mNumBytes = aNumBytes;
   strcpy(mName, aName);
   mNumAttached = 0;

   // Save and set the umask.
   mode_t save_umask = umask(0);
   mode_t tMode = 0666;

   printf("umask %03o %03o\n", tMode, save_umask);

   // Default so that this is the first one.
   bool tFirstFlag = true;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Create the key.

   // Get a temp file path.
   char* tFilePath = new char[2000];

   if (aName[0] != '/')
   {
      strcpy(tFilePath, "/var/tmp/");
      strcat(tFilePath, aName);
   }
   else
   {
      strcpy(tFilePath, "/var/tmp");
      strcat(tFilePath, aName);
   }

   // Create the temp file.
   Ris::touchFile(tFilePath);

   // Create the key.
   mSpecific->mKey = ftok(tFilePath,101);

   printf("ftok key %s %d\n", tFilePath, mSpecific->mKey);

   delete tFilePath;

   if (mSpecific->mKey == -1)
   {
      printf("ftok error101 %d\n", errno);
      exit(1);
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Create or open the shared memory.

   // Try to open the shared memory region.
   mSpecific->mFd = shmget(mSpecific->mKey, mNumBytes, tMode);
   tFirstFlag = false;

   // If the open didn't work then the shared memory doesn't exist.
   if (mSpecific->mFd == -1)
   {
      // Create the shared memory region.
      mSpecific->mFd = shmget(mSpecific->mKey, mNumBytes, tMode | IPC_CREAT);
      tFirstFlag = true;
   }

   // Guard.
   if (mSpecific->mFd == -1)
   {
      printf("shmget error101 %d\n", errno);
      exit(1);
      return false;
   }

   // Restore the umask.
   umask(save_umask);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Map the shared memory.

   // Attach the shared memory to the process address space.
   mMemory = shmat(mSpecific->mFd, NULL, 0);

   if (mMemory == (void*)-1)
   {
      printf("shmat error102 %d\n", errno);
      exit(1);
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Get the number of attaches.

   struct shmid_ds tBuff;
   memset(&tBuff, 0, sizeof(struct shmid_ds));
   tRet = shmctl(mSpecific->mFd, IPC_STAT, &tBuff);
   if (tRet != 0)
   {
      printf("shctl error101 %d\n", errno);
      exit(1);
      return false;
   }

   mNumAttached = tBuff.shm_nattch;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   printf("shm_open %d %s\n", mNumAttached, my_string_from_bool(tFirstFlag));

   return tFirstFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the shared memory.

void SharedMemory::finalize()
{
   int tRet = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Get the number of attaches.

   struct shmid_ds tBuff;
   memset(&tBuff, 0, sizeof(struct shmid_ds));
   tRet = shmctl(mSpecific->mFd, IPC_STAT, &tBuff);
   if (tRet != 0)
   {
      printf("shctl error101 %d\n", errno);
      exit(1);
      return;
   }

   mNumAttached = tBuff.shm_nattch;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Unmap the shared memory.

   // Detach the shared memory.
   printf("shmdt\n");
   tRet = shmdt(mMemory);
   
   // Set to zero.
   mMemory = 0;

   if (tRet == -1)
   {
      printf("finalize error102 %d", errno);
      return;
   }

   // Unlink the shared memory.
   if (mNumAttached == 1)
   {
      printf("shmctl RMID\n");
      shmctl(mSpecific->mFd, IPC_RMID, NULL);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show info.

void SharedMemory::show()
{
   int tRet = 0;
   struct shmid_ds tBuff;
   memset(&tBuff, 0, sizeof(struct shmid_ds));
   tRet = shmctl(mSpecific->mFd, IPC_STAT, &tBuff);
   if (tRet != 0)
   {
      printf("show error101 %d\n", errno);
      exit(1);
      return;
   }
   printf("perm.uid    %d\n", tBuff.shm_perm.uid);
   printf("perm.gid    %d\n", tBuff.shm_perm.gid);
   printf("perm.cuid   %d\n", tBuff.shm_perm.cuid);
   printf("perm.cgid   %d\n", tBuff.shm_perm.cgid);
   printf("perm.mode   %o\n", tBuff.shm_perm.mode);
   printf("cpid        %d\n", tBuff.shm_cpid);
   printf("lpid        %d\n", tBuff.shm_lpid);
   printf("attach      %d\n", tBuff.shm_nattch);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
