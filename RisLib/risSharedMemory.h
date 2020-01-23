#pragma once

/*==============================================================================
Shared Memory class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This creates or opens a region of shared memory.
//
// The following rules are for shared memory regions that are shared between
// different processes (who therefor have different address spaces):
// 
// 1) No constructors.
// 2) No pointers.
// 3) No dynamic memory, this means no std::string, no std::vector, ...
// 4) No file handles or file descriptors.

class SharedMemory
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 100;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // The name of the shared memory.
   char mName[cMaxStringSize];

   // The size of the shared memory.
   int mNumBytes;

   // The address of the shared memory that was created or opened.
   // If this is zero then the shared memory is not open.
   void* mMemory;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Print filter.
   int mPF;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SharedMemory();
   ~SharedMemory();

   // If the shared memory does not already exist, then create it and
   // return true. If it does already exist, then open it and return false.
   bool initialize(const char* aName, int aNumBytes, int aPrintFilter = 0);

   // Detach from the shared memory. Remove it if there are no more
   // processes attached to it.
   void finalize();

   // Show.
   void show();

   // Get number of processes that are attached to the shared memory.
   int getNumAttached();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Specific implementation.
protected:
   class Specific;
   Specific* mSpecific;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


