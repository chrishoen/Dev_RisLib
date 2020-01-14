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
   mNumBytes = 0;
   mMemory = 0;
}

SharedMemory::~SharedMemory()
{
   delete mSpecific;
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

   bool tFirstFlag = false;

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

   // Store this.
   mNumBytes = aNumBytes;

   // Open shared memory.
   mSpecific->mFd = shm_open(mName, O_CREAT | O_RDWR, 0777);

   if (mSpecific->mFd < 0)
   {
      printf("SharedMemory::initialize error101 %d\n", errno);
      return false;
   }

   printf("SharedMemory::initialize shm_open %d %s %d\n", tFirstFlag,mName,mNumBytes);

   // Set the memory size.
   ftruncate(mSpecific->mFd, mNumBytes);

   // Map the memory to process address space.
   mMemory = mmap(0, (size_t)mNumBytes, PROT_WRITE | PROT_READ, MAP_SHARED, mSpecific->mFd, 0);

   // Close the file.
   close(mSpecific->mFd);

   if (mMemory == 0)
   {
      printf("SharedMemory::initialize error102 %d\n", errno);
      return false;
   }

   // Done.
   return tFirstFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the shared memory.

void SharedMemory::finalize()
{
   printf("SharedMemory::finalize");
   // Guard.
   if (mMemory == 0) return;

   // Do this first.
   int tRet = 0;

   // nnmap the memory to process address space.
   tRet = munmap(mMemory, (size_t)mNumBytes);

   if (tRet < 0)
   {
      printf("SharedMemory::finalize error101 %d", errno);
   }

   // Remove the shared memory.
   tRet = shm_unlink(mName);

   if (tRet < 0)
   {
      printf("SharedMemory::finalize error102 %d", errno);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
