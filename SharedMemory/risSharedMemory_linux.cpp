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
   mCreateFlag = false;
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

bool SharedMemory::initialize(const char* aName, int aNumBytes, bool aCreateFlag)
{
   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Do this first.

   int tRet = 0;
   mMemory = 0;
   mNumBytes = aNumBytes;
   mCreateFlag = aCreateFlag;

   // Save and set the umask.
   mode_t save_umask = umask(0);
   mode_t tMode = 0666;

   printf("umask %03o %03o\n", tMode, save_umask);

   // Default so that this is the first one.
   bool tFirstFlag = true;

   // Create the key.
   mSpecific->mKey = ftok("/opt/prime/special/myftok",101);

   printf("ftok key %d\n", mSpecific->mKey);
   if (mSpecific->mKey == -1)
   {
      printf("ftok error101 %d\n", errno);
      exit(1);
      return false;
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Create or open the shared memory.

   if (aCreateFlag)
   {
      // Create a shared memory region.
      mSpecific->mFd = shmget(mSpecific->mKey, mNumBytes, tMode | IPC_CREAT);
      tFirstFlag = true;
   }
   else
   {
      // Open a shared memory region exclusively.
      mSpecific->mFd = shmget(mSpecific->mKey, mNumBytes, tMode);
      tFirstFlag = false;
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

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Map the shared memory.

   // Map  the memory to process address space.
   mMemory = shmat(mSpecific->mFd, NULL, 0);

   if (mMemory == (void*)-1)
   {
      printf("shmat error102 %d\n", errno);
      exit(1);
      return false;
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Lock the shared memory.

   if (mCreateFlag && false)
   {

      printf("shmctl SHM_LOCK\n");
      shmctl(mSpecific->mFd, SHM_LOCK, NULL);
      if (tRet == -1)
      {
         printf("lock error101 %d\n", errno);
         exit(1);
         return false;
      }
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Done.

   printf("shm_open %d\n", tFirstFlag);

   return tFirstFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the shared memory.

void SharedMemory::finalize(bool aUnlink)
{
   int tRet = 0;

   // unmap the shared memory.
   printf("shmdt\n");
   tRet = shmdt(mMemory);
   
   if (tRet == -1)
   {
      printf("finalize error102 %d", errno);
      return;
   }

   // Unlink the shared memory.
   if (aUnlink)
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
