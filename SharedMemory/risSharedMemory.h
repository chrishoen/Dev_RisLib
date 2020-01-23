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

   // The name of the shared memory region.
   char mName[cMaxStringSize];

   // The size of the shared memory region.
   int mNumBytes;

   // The address of the shared memory region that was created or opened.
   // If this is zero then the region is not open.
   void* mMemory;

   // The number of attaches to the shared memory.
   int mNumAttached;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SharedMemory();
   ~SharedMemory();

   // If the shared memory region does not already exist, then create it and
   // return true. If it does already exist, then open it and return false.
   bool initialize(const char* aName, int aNumBytes);

   // Close the shared memory region.
   void finalize();

   // Show.
   void show();

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


