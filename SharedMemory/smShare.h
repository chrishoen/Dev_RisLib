#pragma once

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace SM
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class implements a general purpose shared storage object. It contains
// status counters and flags.

class Share
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Resource counter. This is automatically set to zero when the share is 
   // created, because the shared memory region is zeroed whn it is created. 
   // It is incremented when the share is initialized. It is decremented when
   // it is finalized.
   int mResourceCount;

   // Some variables.
   int mX1;
   int mX2;
   int mX3;
   int mX4;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Initialize, there's no constructor. This is called by the process who
   // first creates the shared memory.
   void initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   void show(int aPrintFilter = 0);
   void show2();

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance initialize and finalize.

// Create a shared memory region for the global instance and call placement
// new with the shared memory address. If this is the first process to access
// the region (it was created created, not opened) then initialize the global
// instance. Otherwise, do not initialize it, because the first process
// already did.
void initializeShare();

// Close the shared memory region for the global instance.
void finalizeShare();

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _SMSHARE_CPP_
          Share* gShare;
#else
   extern Share* gShare;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

