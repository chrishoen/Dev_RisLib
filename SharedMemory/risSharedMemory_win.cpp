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
// Create the shared memory region, if it does not already exist. Open it
// if it does already exist. Return true if the region was created, false
// if it was opened.

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

   int tCreateStatus = GetLastError();
   //printf("CreateFileMapping %d\n",tCreateStatus);

   if (mSpecific->mShareFileMap != 0)
   {
      char* tMemory = (char*)MapViewOfFile(
         mSpecific->mShareFileMap,
         FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

      // Store the address.
      mMemory = tMemory;
   }

   // Done
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
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
