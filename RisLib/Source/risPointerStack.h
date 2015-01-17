#ifndef _POINTERSTACK_H_
#define _POINTERSTACK_H_
/*==============================================================================

This defines a stack of void* pointers. It can be used by specialized heaps
and pools as an allocator. It is not thread safe.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class PointerStack
{
public:
   //---------------------------------------------------------------------------
   // Methods

   // Constructor
   PointerStack();
  ~PointerStack();

   // Initialize and allocate memory for the stack.
   void initialize(int aAllocate);

   // Push an element onto the stack.
   // Return false if stack is full.
   bool  push(void* aPointer);

   // Pop an element off of the stack
   // Return null if stack is empty.
   void* pop();

   //---------------------------------------------------------------------------
   // Members

   // Array of void*
   void** mArray;

   // Index into the array
   int mIndex;

   // Size of the array
   int mAllocate;
};

//******************************************************************************

}//namespace
#endif

