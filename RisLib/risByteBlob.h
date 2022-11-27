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
// It inherits from ByteContent and instances are usually members of
// classes that inherit from byte content.
// 
// It encapsulates a chunk of memory that is allocated in place that can
// be copied to/from byte buffers. The template parameter specifies the
// number of bytes to allocate in place;

template <int AllocateSize>
class ByteBlob : public ByteContent
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Allocated memory and a pointer to it.
   char mMemory[AllocateSize];
   char* mBytes;

   // Number of bytes to copy to/from a byte buffer. This is less than or equal
   // to the allocated size. This variable is not copied to/from the given byte
   // buffer. The parent class must explicitly set it before doing a copy to.
   // The parent class must also explicitly set it inside a copy from.
   int mCopySize;

   // Maximum copy size.
   int mMaxCopySize;

   // Index for get byte operations.
   int mGetIndex;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   ByteBlob()
   {
      mBytes = &mMemory[0];
      mCopySize = 0;
      mMaxCopySize = AllocateSize;
      mGetIndex = 0;
   }

   ~ByteBlob()
   {
   }

   // Copy allocated memory to/from a byte buffer. The copy size must be
   // explicitly set before calling this.
   void copyToFrom(Ris::ByteBuffer* aBuffer) override
   {
      aBuffer->copyBlock(mBytes, mCopySize);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Return true if there is enough room to add some more bytes.
   bool testRemainingSize(int aSize)
   {
      return mCopySize + aSize <= mMaxCopySize;
   }

   // Put some bytes to the allocated memory array. Increment the copy size.
   // Return true if successful.
   bool putBytes(void* aBytes, int aSize)
   {
      // Guard.
      if (mCopySize + aSize > mMaxCopySize) return false;

      // Copy some bytes from the function argument byte pointer.
      char* tBytes = (char*)aBytes;
      for (int i = 0; i < aSize; i++)
      {
         mBytes[mCopySize + i] = tBytes[i];
      }

      // Increment the copy size.
      mCopySize += aSize;
      return true;
   }

   // Get some bytes from the allocated memory array at the current get
   // index. Increment the get index. Return true if successful.
   bool getBytes(void* aBytes, int aSize)
   {
      // Guard.
      if (mGetIndex + aSize > mCopySize) return false;

      // Copy some bytes to the function argument byte pointer.
      char* tBytes = (char*)aBytes;
      for (int i = 0; i < aSize; i++)
      {
         tBytes[i] = mBytes[mGetIndex + i];
      }

      // Increment the get index.
      mGetIndex += aSize;
      return true;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

