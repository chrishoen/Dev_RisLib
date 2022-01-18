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
   char* mBytes;

   // Number of bytes to copy to/from a byte buffer. This is less than or equal
   // to the allocated size. This variable is not copied to/from the given byte
   // buffer. The parent class must explicitly set it before doing a copy to.
   // The parent class must also explicitly set it inside a copy from.
   int mCopySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   ByteBlob()
   {
      mBytes = (char*)malloc(AllocateSize);
      mCopySize = 0;
   }

   ~ByteBlob()
   {
      free(mBytes);
   }

   // Copy allocated memory to/from a byte buffer. The copy size must be
   // explicitly set before calling this.
   void copyToFrom(Ris::ByteBuffer* aBuffer) override
   {
      aBuffer->copyBlock(mBytes, mCopySize);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

