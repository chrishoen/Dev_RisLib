/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <Windows.h>

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
   HANDLE mShareFileMap;
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

   // Create shared memory.
   mSpecific->mShareFileMap=CreateFileMapping(
      INVALID_HANDLE_VALUE,
      NULL,
      PAGE_READWRITE,
      0,
      aNumBytes,
      aName);

   // Get the last error. If it is zero then the region did not already
   // exist and a new one was created.
   int tCreateStatus = GetLastError();

   // Map the file.
   if (mSpecific->mShareFileMap != 0)
   {
      char* tMemory = (char*)MapViewOfFile(
         mSpecific->mShareFileMap,
         FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

      // Store the address.
      mMemory = tMemory;
   }

   // Done.
   return tCreateStatus == 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the shared memory.

void SharedMemory::finalize()
{
   CloseHandle(mSpecific->mShareFileMap);

    if (mSpecific) delete mSpecific;
    mSpecific = 0;
    mMemory = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
