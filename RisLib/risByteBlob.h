#pragma once

/*==============================================================================
ByteContent byte blob class template.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include "risByteContent.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a class template for byte blobs, which are opaque sections 
// of heap allocated memoty.
// 
// It inherits from ByteContent and instances are usually members of classes
// that inherit from byte content.
// 
// It encapsulates a chunk of heap allocated memory that can be copied
// to/from byte buffers. The heap memory is allocated/deallocated by the
// constructor/destructor. The template parameter specifies the number
// of bytes to allocate on the heap;

template <int AllocateSize>
class ByteBlob : public ByteContent
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Allocated memory.
   char* mMemory;

   // Number of bytes to copy to/from a byte buffer. This is not copied
   // to/from the given byte buffer. The parent class must explicely 
   // set it before doing the copy.
   int mCopySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   ByteBlob()
   {
      mMemory = malloc(AllocateSize);
      mCopySize = 0;
   }

   ~ByteBlob()
   {
      free(mMemory);
   }

   // Copy allocated memory to/from a byte buffer. The copy size must be
   // explicitly set before calling this.
   void copyToFrom(Ris::ByteBuffer* aBuffer) override
   {
      aBuffer->copyBlock(mMemory, mCopySize);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

