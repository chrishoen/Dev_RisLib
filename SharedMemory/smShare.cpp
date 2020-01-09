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
// Return the state as a string.

void Share::show(int aPF)
{
   Prn::print(aPF, "mX1   %d", mX1);
   Prn::print(aPF, "mX2   %d", mX2);
   Prn::print(aPF, "mX3   %d", mX3);
   Prn::print(aPF, "mX4   %d", mX4);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Create a shared memory region for the global instance and call placement
// new with the shared memory address. If this is the first process to access
// the region (it was created created, not opened) then initialize the global
// instance. Otherwise, do not initialize it, because the first process
// already did.

void initializeShare()
{
   // Create the shared memory region.
   bool tFirst = gSharedMemory.initialize("AAAASHARE", 16384);

   // Create the global instance.
   gShare = new (gSharedMemory.mMemory) Share;

   // If this the first time that the shared memory region was created
   // then initialize.
   if (tFirst)
   {
      gShare->initialize();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the shared memory region for the global instance.

void finalizeShare()
{
   gSharedMemory.finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
