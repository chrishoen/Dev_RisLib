/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risSharedMemory.h"

#define  _SMSHARE_CPP_
#include "smShare.h"

namespace SM
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shared memory object for the global instance.

Ris::SharedMemory gSharedMemory;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize, there's no constructor. This is called by the process who
// first creates the shared memory.

void Share::initialize()
{
   mX1 = 101;
   mX2 = 102;
   mX3 = 103;
   mX4 = 104;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Share::show(int aPF)
{
   Prn::print(aPF, "mResourceCount   %d", mResourceCount);
   Prn::print(aPF, "mX1              %d", mX1);
   Prn::print(aPF, "mX2              %d", mX2);
   Prn::print(aPF, "mX3              %d", mX3);
   Prn::print(aPF, "mX4              %d", mX4);
}

void Share::show2()
{
   gSharedMemory.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Create a shared memory region for the global instance and call placement
// new with the shared memory address. If this is the first process to access
// the region (it was created created, not opened) then initialize the global
// instance. Otherwise, do not initialize it, because the first process
// already did.

void initializeShare(bool aCreateFlag)
{
   // Create the shared memory region.
   gSharedMemory.initialize("AAAASHARE2", 4096, aCreateFlag);

   // Create the global instance.
   gShare = new (gSharedMemory.mMemory) Share;

   // Increment the resource count. If this the first time that the shared
   // memory region was created then initialize.
   if (++gShare->mResourceCount == 1)
   {
      printf("initialize\n");
      gShare->initialize();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the shared memory region for the global instance.

void finalizeShare()
{
   // Decrement the resource count.
   gShare->mResourceCount--;

   // Finalize.
   gSharedMemory.finalize(gShare->mResourceCount == 0);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
