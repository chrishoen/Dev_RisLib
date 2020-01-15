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
   int mFd;
};

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Constructor.

SharedMemory::SharedMemory()
{
   mSpecific = new Specific;
   mName[0] = 0;
   mNumBytes = 0;
   mMemory = 0;
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// If the shared memory region does not already exist, then create it and
// return true. If it does already exist, then open it and return false.

bool SharedMemory::initialize(const char* aName, int aNumBytes)
{
   // Do this first.
   mMemory = 0;
   mNumBytes = aNumBytes;

   // Store the name. Make sure the first char is a '/'
   if (aName[0] == '/')
   {
      strncpy(mName, aName, cMaxStringSize - 2);
      mName[cMaxStringSize - 1] = 0;
   }
   else
   {
      strncpy(mName, "/",1);
      strncat(mName, aName, cMaxStringSize - 3);
      mName[cMaxStringSize - 1] = 0;
   }

   // Save and set the umask.
   mode_t save_umask = umask(0);

   // Default so that this is the first one.
   bool tFirstFlag = true;

   // Try to create shared memory exclusively.
   mSpecific->mFd = shm_open(mName, O_RDWR | O_CREAT | O_EXCL, 0666);

   // if create failed then the shared memory already exists.
   if (mSpecific->mFd < 0)
   {
      // Open the shared memory.
      mSpecific->mFd = shm_open(mName, O_RDWR, 0666);

      // This is the not the first one.
      tFirstFlag = false;
   }

   // Restore the umask.
   umask(save_umask);

   // Set the size.
   if (tFirstFlag)
   {
      ftruncate(mSpecific->mFd, mNumBytes);
   }

   // Map  the memory to process address space.
   mMemory = mmap(0, aNumBytes, PROT_WRITE | PROT_READ, MAP_SHARED, mSpecific->mFd, 0);

   // Close the file handle.
   close(mSpecific->mFd);

   printf("shm_open %d\n", tFirstFlag);

   // Done.
   return tFirstFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the shared memory.

void SharedMemory::finalize(bool aUnlink)
{
   // unmap the shared memory.
   munmap(mMemory, mNumBytes);
   
   // Remove the shared memory.
   if (aUnlink)
   {
      shm_unlink(mName);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
