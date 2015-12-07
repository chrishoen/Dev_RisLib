/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "risByteContent.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Here is a global flag that gets assigned a value at load time.
// If the machine that this executes on is little endian then it is true
// else it is false

   unsigned int gByteBuffer_ThisMachineIsLittleEndian_TestValue = 0x01020304;

   bool gByteBuffer_ThisMachineIsLittleEndian =
      *((char*)&gByteBuffer_ThisMachineIsLittleEndian_TestValue)==0x04;

// Here is a global flag that gets assigned a value at load time.
// It sets the default for byte swapping to non network order.

   bool gByteBuffer_DoByteSwapping = !gByteBuffer_ThisMachineIsLittleEndian;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructors

ByteBuffer::ByteBuffer ()
{
   mBaseBytes     = 0;
   mWorkingIndex  = 0;
   mWorkingLength = 0;
   mMaxLength     = 0;
   mMemAllocCode  = 0;
   mCopyDirection = cCopyTo;
   mError         = 0;
   mByteSwapping  = gByteBuffer_DoByteSwapping;
}

ByteBuffer::ByteBuffer (int aSize)
{
   memAlloc(aSize);
   mCopyDirection = cCopyTo;
   mError = 0;
   mByteSwapping=gByteBuffer_DoByteSwapping;
}

ByteBuffer::ByteBuffer (char* aAddress,int aSize)
{
   mBaseBytes     = aAddress;
   mWorkingIndex  = 0;
   mWorkingLength = 0;
   mMaxLength     = aSize;
   mMemAllocCode  = 0;
   mCopyDirection = cCopyTo;
   mError         = 0;
   mByteSwapping  = gByteBuffer_DoByteSwapping;
}

ByteBuffer::~ByteBuffer ()
{
   if(mMemAllocCode)
   {
      free((void*)mBaseBytes);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Memory management

void ByteBuffer::memAlloc (int aSize)
{
   mBaseBytes = (char*) malloc(aSize);

   mWorkingIndex  = 0;
   mWorkingLength = 0;
   mMaxLength     = aSize;
   mMemAllocCode  = 1;
}

//******************************************************************************

void ByteBuffer::memFree ()
{
   if(mMemAllocCode)
   {
      free((void*)mBaseBytes);
   }

   mBaseBytes     = 0;
   mWorkingIndex  = 0;
   mWorkingLength = 0;
   mMaxLength     = 0;
   mMemAllocCode  = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pointer operations 

void ByteBuffer::reset ()
{
   mWorkingIndex  = 0;
   mWorkingLength = 0;
}

//******************************************************************************

void ByteBuffer::rewind ()
{
   mWorkingIndex = 0;
}

//******************************************************************************

void ByteBuffer::forward (int aSize)
{
   if (aSize + mWorkingIndex > mMaxLength) return;

   if (mCopyDirection==cCopyTo)
   {
      mWorkingIndex  += aSize;
      mWorkingLength += aSize;
   }
   else
   {
      mWorkingIndex    += aSize;
   }
}

//******************************************************************************

void ByteBuffer::advance (int aSize)
{
   if (aSize + mWorkingIndex > mMaxLength) return;

   mWorkingIndex  += aSize;
   mWorkingLength += aSize;
}

//******************************************************************************

void ByteBuffer::fillZero (int aSize)
{
   if (aSize + mWorkingIndex > mMaxLength) return;

   if (mCopyDirection==cCopyTo)
   {
      for (int i=0; i<aSize; i++)
      {
         mBaseBytes[mWorkingIndex + i] = 0;
      }

      mWorkingIndex  += aSize;
      mWorkingLength += aSize;
   }
   else
   {
      mWorkingIndex  += aSize;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Buffer position

// Get the address of the start of the buffer.
char* ByteBuffer::getBaseAddress ()
{
   return mBaseBytes;
}

// Set the buffer working index.
void ByteBuffer::setPosition (int aIndex)
{
   // Set index
   mWorkingIndex = aIndex;

   // Guard
   if (mWorkingIndex > mMaxLength - 1)
   {
      mWorkingIndex = mMaxLength - 1;
   }
}

// Get the buffer working index.
int ByteBuffer::getPosition ()
{
   return mWorkingIndex;
}

// Get the buffer address at the working index.
char* ByteBuffer::getPositionC ()
{
   return &mBaseBytes[mWorkingIndex];
}

void* ByteBuffer::getPositionV ()
{
   return (void*)&mBaseBytes[mWorkingIndex];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Access some members

int ByteBuffer::getMaxLength()
{
   return mMaxLength;
}

void ByteBuffer::setLength(int aValue)
{
   mWorkingLength=aValue;
}

int ByteBuffer::getLength()
{
   return mWorkingLength;
}

int ByteBuffer::getError()
{
   return mError;
}

void ByteBuffer::setError(int aValue)
{
   mError=aValue;
}

void ByteBuffer::setCopyTo()
{
   mCopyDirection=cCopyTo;
}

void ByteBuffer::setCopyFrom ()
{
   mCopyDirection=cCopyFrom;
}

bool ByteBuffer::isCopyTo()
{
   return mCopyDirection==cCopyTo;
}

bool ByteBuffer::isCopyFrom ()
{
   return mCopyDirection==cCopyFrom;
}

void ByteBuffer::setNetworkOrder (bool aNetworkOrder)
{
   if (gByteBuffer_ThisMachineIsLittleEndian)
   {
      mByteSwapping = aNetworkOrder;
   }
   else
   {
      mByteSwapping = !aNetworkOrder;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations

static void BB_copyValue(ByteBuffer* aBuffer, void* aValue, int aSize)
{
   //---------------------------------------------------------------------------
   // Guard

   if (aSize == 0)
   {
      return;
   }

   if (aBuffer->isCopyTo())
   {
      if (aSize + aBuffer->mWorkingIndex > aBuffer->mMaxLength)
      {
         aBuffer->setError(ByteBuffer::cBufferOverflow);
         return;
      }
   }
   else
   {
      if (aSize + aBuffer->mWorkingIndex > aBuffer->mWorkingLength)
      {
         aBuffer->setError(ByteBuffer::cBufferOverflow);
         return;
      }
   }

   //---------------------------------------------------------------------------
   // Copy value to byte buffer

   if (aBuffer->isCopyTo())
   {
      // Source pointer
      char* tSource = (char*)aValue;

      // Destination Buffer pointer
      char* tBytes = &aBuffer->mBaseBytes[aBuffer->mWorkingIndex];

      if (aBuffer->mByteSwapping)
      {
         // Copy bytes to buffer, swapping bytes
         for (int i = 0; i < aSize; i++)
         {
            tBytes[i] = tSource[aSize - i - 1];
         }
      }
      else
      {
         // Copy bytes to buffer, not swapping bytes
         char* tBytes = &aBuffer->mBaseBytes[aBuffer->mWorkingIndex];

         for (int i = 0; i < aSize; i++)
         {
            tBytes[i] = tSource[i];
         }
      }

      // Adjust buffer members
      aBuffer->mWorkingIndex  += aSize;
      aBuffer->mWorkingLength += aSize;
   }
   //---------------------------------------------------------------------------
   else
   {
      // Destination pointer
      char* tDestin = (char*)aValue;

      // Source Buffer pointer
      char* tBytes = &aBuffer->mBaseBytes[aBuffer->mWorkingIndex];
      int   tWorkingIndex = aBuffer->mWorkingIndex;

      // Copy bytes from buffer, swapping bytes

      if (aBuffer->mByteSwapping)
      {
         for (int i = 0; i < aSize; i++)
         {
            tDestin[i] = tBytes[aSize - i - 1];
         }
      }
      else
      {
         // Copy bytes from buffer, not swapping bytes
         for (int i = 0; i < aSize; i++)
         {
            tDestin[i] = tBytes[i];
         }
      }

      // adjust members
      aBuffer->mWorkingIndex += aSize;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations for value types

void ByteBuffer::copy (unsigned char*      aValue) { BB_copyValue(this, aValue, 1); }
void ByteBuffer::copy (unsigned short*     aValue) { BB_copyValue(this, aValue, 2); }
void ByteBuffer::copy (unsigned int*       aValue) { BB_copyValue(this, aValue, 4); }
void ByteBuffer::copy (unsigned long long* aValue) { BB_copyValue(this, aValue, 8); }
void ByteBuffer::copy (char*               aValue) { BB_copyValue(this, aValue, 1); }
void ByteBuffer::copy (short*              aValue) { BB_copyValue(this, aValue, 2); }
void ByteBuffer::copy (int*                aValue) { BB_copyValue(this, aValue, 4); }
void ByteBuffer::copy (long long*          aValue) { BB_copyValue(this, aValue, 8); }
void ByteBuffer::copy (float*              aValue) { BB_copyValue(this, aValue, 4); }
void ByteBuffer::copy (double*             aValue) { BB_copyValue(this, aValue, 8); }

void ByteBuffer::copy (bool*               aValue)
{
   if (isCopyTo())
   {
      // Copy boolean value to one byte in the buffer
      unsigned char tFlag = aValue ? 1 : 0;
      BB_copyValue(this, &tFlag, 1);
   }
   else
   {
      // Copy boolean value from one byte in the buffer
      unsigned char tFlag = 0;
      BB_copyValue(this, &tFlag, 1);
      *aValue = (tFlag != 0);
   }
}

void ByteBuffer::copyEnum (int* aValue) { BB_copyValue(this, aValue, 4); }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations for null terminated string type

void ByteBuffer::copyZString (char* aString,int aUpperBound) 
{
   //--------------------------------------------------------------------------
   // Get string length

   int tSize = 0;
   if (isCopyTo())
   {
      // tSize is the length of the string to copy to the buffer
      // plus the null character
      while (1)
      {
         if (aString[tSize++] == 0) break;
         if (tSize == aUpperBound)  break;
      }
   }
   else
   {
      // tSize is the length of the string to copy from the buffer
      // plus the null character
      while (1)
      {
         if (mBaseBytes[mWorkingIndex + tSize++] == 0) break;
         if (tSize == aUpperBound)  break;
      }
   }

   //--------------------------------------------------------------------------
   // Guard

   if (isCopyTo())
   {
      if (tSize + mWorkingIndex > mMaxLength)
      {
         setError(ByteBuffer::cBufferOverflow);
         return;
      }
   }
   else
   {
      if (tSize + mWorkingIndex > mWorkingLength)
      {
         setError(ByteBuffer::cBufferOverflow);
         return;
      }
   }

   //--------------------------------------------------------------------------
   // Copy

   if (mCopyDirection == cCopyTo)
   {
      // Copy the string to the buffer, without the null character
      char* tWorkingPtr = &mBaseBytes[mWorkingIndex];
      memcpy(tWorkingPtr, aString, tSize - 1);

      // copy the null character to the buffer
      tWorkingPtr[tSize - 1] = 0;

      // Adjust buffer members
      mWorkingIndex  += tSize;
      mWorkingLength += tSize;
   }
   else
   {
      // Copy the string from the buffer, without the null character
      char* tWorkingPtr = &mBaseBytes[mWorkingIndex];
      memcpy(aString, tWorkingPtr, tSize - 1);

      // Copy the null character to the buffer
      aString[tSize - 1] = 0;

      // Adjust buffer members
      mWorkingIndex += tSize;
   }
}

void ByteBuffer::copyZString(unsigned char* aString, int aUpperBound) { copyZString((char*)aString, aUpperBound); }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy for object that inherits from ByteContent

void ByteBuffer::copy (ByteContent* aContent) 
{
   // Call ByteContent overload to copy to/from the buffer
   aContent->copyToFrom(this);
}

void ByteBuffer::putToBuffer (ByteContent* aContent) 
{
   // Set copy direction
   setCopyTo();

   // Call ByteContent overload to copy to the buffer
   aContent->copyToFrom(this);
}

void ByteBuffer::getFromBuffer (ByteContent* aContent) 
{
   // Set copy direction
   setCopyFrom();

   // Call ByteContent overload to copy to the buffer
   aContent->copyToFrom(this);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy raw data

void ByteBuffer::copyData (void* aData,int aSize) 
{
   //--------------------------------------------------------------------------
   // Guard

   if (isCopyTo())
   {
      if (aSize + mWorkingIndex > mMaxLength)
      {
         setError(ByteBuffer::cBufferOverflow);
         return;
      }
   }
   else
   {
      if (aSize + mWorkingIndex > mWorkingLength)
      {
         setError(ByteBuffer::cBufferOverflow);
         return;
      }
   }
   if (mCopyDirection==cCopyTo)
   {
      // Copy the data to the buffer
      char* tWorkingPtr = &mBaseBytes[mWorkingIndex];
      memcpy(tWorkingPtr,aData,aSize);

      // Adjust members
      mWorkingIndex  += aSize;
      mWorkingLength += aSize;
   }
   else
   {
      // Copy the fixed size data
      char* tWorkingPtr = &mBaseBytes[mWorkingIndex];
      memcpy(aData,tWorkingPtr,aSize);

      // Adjust members
      mWorkingIndex += aSize;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace



