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

   // Try to open shared memory.
   mSpecific->mFd = shm_open(mName, O_RDWR, 0666);

   // if open failed then the shared memory does not exist.
   if (mSpecific->mFd < 0)
   {
      // Create the shared memory.
      mSpecific->mFd = shm_open(mName, O_CREAT | O_RDWR, 0666);

      // This is the first one.
      tFirstFlag = true;
   }

   // Set the size
   ftruncate(mSpecific->mFd, aNumBytes);

   // Map  the memory to process address space.
   mMemory = mmap(0, aNumBytes, PROT_WRITE | PROT_READ, MAP_SHARED, mSpecific->mFd, 0);

   printf("shm_open %d\n", tFirstFlag);

   // Done.
   return tFirstFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the shared memory.

void SharedMemory::finalize()
{
   // Remove the shared memory.
   shm_unlink(mName);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
