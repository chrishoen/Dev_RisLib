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

#include "prnPrint.h"
#include "my_functions.h"
#include "risPortableCalls.h"
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
// If the shared memory does not already exist, then create it and
// return true. If it does already exist, then open it and return false.

bool SharedMemory::initialize(const char* aName, int aNumBytes, int aPrintFilter)
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
   mPF = aPrintFilter;

   // Save and set the umask.
// mode_t save_umask = umask(0);
   mode_t tMode = 0666;

// Prn::print(mPF, "umask %03o %03o", tMode, save_umask);

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
      strcpy(tFilePath, "/tmp/aaa_shm_");
      strcat(tFilePath, aName);
   }
   else
   {
      strcpy(tFilePath, "/tmp/aaa_shm_");
      strcat(tFilePath, &aName[1]);
   }

   // Create the temp file.
   if (!Ris::portableFilePathExists(tFilePath))
   {
      Ris::touchFile(tFilePath);
   }

   // Create the key.
   mSpecific->mKey = ftok(tFilePath,101);

// Prn::print(mPF, "ftok key %s %d", tFilePath, mSpecific->mKey);

   delete[] tFilePath;

   if (mSpecific->mKey == -1)
   {
      Prn::print(mPF, "ftok error101 %d", errno);
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
      Prn::print(mPF, "shmget error101 %d", errno);
      exit(1);
      return false;
   }

   // Restore the umask.
// umask(save_umask);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Map the shared memory.

   // Attach the shared memory to the process address space.
   mMemory = shmat(mSpecific->mFd, NULL, 0);

   if (mMemory == (void*)-1)
   {
      Prn::print(mPF, "shmat error102 %d", errno);
      exit(1);
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

// Prn::print(mPF, "shm_open %d %d %s",
//    mSpecific->mFd,
//    getNumAttached(),
//    my_string_from_bool(tFirstFlag));

   return tFirstFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Detach from the shared memory. Remove it if there are no more
// processes attached to it.

void SharedMemory::finalize()
{
   int tRet = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Get the number of attaches.

   int tNumAttached = getNumAttached();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Unmap the shared memory.

   // Detach the shared memory.
   Prn::print(mPF, "shmdt");
   tRet = shmdt(mMemory);
   
   // Set to zero.
   mMemory = 0;

   if (tRet == -1)
   {
      Prn::print(mPF, "finalize error102 %d", errno);
      exit(1);
      return;
   }

   // Unlink the shared memory.
   if (tNumAttached == 1)
   {
      Prn::print(mPF, "shmctl RMID");
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
      Prn::print(mPF, "show error101 %d", errno);
      exit(1);
      return;
   }
   Prn::print(mPF, "perm.uid    %d", tBuff.shm_perm.uid);
   Prn::print(mPF, "perm.gid    %d", tBuff.shm_perm.gid);
   Prn::print(mPF, "perm.cuid   %d", tBuff.shm_perm.cuid);
   Prn::print(mPF, "perm.cgid   %d", tBuff.shm_perm.cgid);
   Prn::print(mPF, "perm.mode   %o", tBuff.shm_perm.mode);
   Prn::print(mPF, "cpid        %d", tBuff.shm_cpid);
   Prn::print(mPF, "lpid        %d", tBuff.shm_lpid);
   Prn::print(mPF, "attach      %d", tBuff.shm_nattch);



   struct shm_info tInfo;
   memset(&tInfo, 0, sizeof(struct shm_info));
   tRet = shmctl(mSpecific->mFd, SHM_INFO, (struct shmid_ds*)&tInfo);
   if (tRet != 0)
   {
      Prn::print(mPF, "show error102 %d", errno);
      exit(1);
      return;
   }
   Prn::print(mPF, "");
   Prn::print(mPF, "info.used_ids    %d", tInfo.used_ids);
   Prn::print(mPF, "info.shm_tot     %d", tInfo.shm_tot);
   Prn::print(mPF, "info.shm_rss     %d", tInfo.shm_rss);
   Prn::print(mPF, "info.shm_swp     %d", tInfo.shm_swp);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get number of processes that are attached to the shared memory.

int SharedMemory::getNumAttached()
{
   int tRet = 0;
   struct shmid_ds tBuff;
   memset(&tBuff, 0, sizeof(struct shmid_ds));
   tRet = shmctl(mSpecific->mFd, IPC_STAT, &tBuff);
   if (tRet != 0)
   {
      Prn::print(mPF, "shctl error101 %d", errno);
      exit(1);
      return false;
   }

   int tNumAttached = tBuff.shm_nattch;
   return tNumAttached;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
