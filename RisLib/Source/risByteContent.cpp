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
   mBasePtr=0;
   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
   mMaxLength=0;
   mMemAllocCode=0;
   mCopyDirection=COPY_TO;
   mError=0;
   mByteSwapping=gByteBuffer_DoByteSwapping;
}

ByteBuffer::ByteBuffer (int aSize)
{
   memAlloc(aSize);
   mCopyDirection=COPY_TO;
   mError=0;
   mByteSwapping=gByteBuffer_DoByteSwapping;
}

ByteBuffer::ByteBuffer (char* aAddress,int aSize)
{
   mBasePtr=aAddress;
   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
   mMaxLength=aSize;
   mMemAllocCode=0;
   mCopyDirection=COPY_TO;
   mError=0;
   mByteSwapping=gByteBuffer_DoByteSwapping;
}

ByteBuffer::ByteBuffer (const ByteBuffer& buffer)
{
   mBasePtr       = buffer.mBasePtr;
   mWorkingPtr    = buffer.mWorkingPtr;
   mWorkingLength = buffer.mWorkingLength;
   mMaxLength     = buffer.mMaxLength;
   mMemAllocCode  = 0;
   mCopyDirection = buffer.mCopyDirection;
   mError         = buffer.mError;
   mByteSwapping  = buffer.mByteSwapping;
}

ByteBuffer& ByteBuffer::operator= (const ByteBuffer& buffer)
{
   mBasePtr       = buffer.mBasePtr;
   mWorkingPtr    = buffer.mWorkingPtr;
   mWorkingLength = buffer.mWorkingLength;
   mMaxLength     = buffer.mMaxLength;
   mMemAllocCode  = 0;
   mCopyDirection = buffer.mCopyDirection;
   mError         = buffer.mError;
   mByteSwapping  = buffer.mByteSwapping;
   return *this;
}

ByteBuffer::~ByteBuffer ()
{
   if(mMemAllocCode)
   {
      free((void*)mBasePtr);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Memory management

void ByteBuffer::memAlloc (int aSize)
{
   mBasePtr = (char*) malloc(aSize);

   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
   mMaxLength=aSize;
   mMemAllocCode=1;
}

//******************************************************************************

void ByteBuffer::memFree ()
{
   if(mMemAllocCode)
   {
      free((void*)mBasePtr);
   }

   mBasePtr=0;
   mWorkingPtr=0;
   mMemAllocCode=0;
   mWorkingLength=0;
   mMaxLength=0;
}

//******************************************************************************

int ByteBuffer::getMaxLength()
{
   return mMaxLength;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Buffer base address

void ByteBuffer::setBaseAddress (char* aAddress,int aSize)
{
   memFree();

   mBasePtr=aAddress;
   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
   mMaxLength=aSize;
   mMemAllocCode=0;
   mError=0;
}

char* ByteBuffer::getBaseAddress ()
{
   return mBasePtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Access some members

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

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pointer operations 

void ByteBuffer::reset ()
{
   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
}

//******************************************************************************

void ByteBuffer::rewind ()
{
   mWorkingPtr=mBasePtr;
}

//******************************************************************************

void ByteBuffer::forward (int aSize)
{
   if (aSize + mWorkingPtr - mBasePtr > mMaxLength) return;

   if (mCopyDirection==COPY_TO)
   {
      mWorkingPtr    += aSize;
      mWorkingLength += aSize;
   }
   else
   {
      mWorkingPtr    += aSize;
   }
}

//******************************************************************************

void ByteBuffer::advance (int aSize)
{
   if (aSize + mWorkingPtr - mBasePtr > mMaxLength) return;

   mWorkingPtr    += aSize;
   mWorkingLength += aSize;
}

//******************************************************************************

void ByteBuffer::fillZero (int aSize)
{
   if (aSize + mWorkingPtr - mBasePtr > mMaxLength) return;

   if (mCopyDirection==COPY_TO)
   {
      for (int i=0;i<aSize;i++)
      {
         mWorkingPtr[i] = 0;
      }

      mWorkingPtr    += aSize;
      mWorkingLength += aSize;
   }
   else
   {
      mWorkingPtr    += aSize;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Buffer position

bool ByteBuffer::setPosition (char* aAddress)
{
   mWorkingPtr=aAddress;
   return true;
}

bool ByteBuffer::setPosition (int aIndex)
{
   if (aIndex>=mMaxLength) return false;

   mWorkingPtr=&mBasePtr[aIndex];

   return true;
}

char* ByteBuffer::getPosition ()
{
   return mWorkingPtr;
}

char* ByteBuffer::getPositionC ()
{
   return mWorkingPtr;
}

void* ByteBuffer::getPositionV ()
{
   return (void*)mWorkingPtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy direction

void ByteBuffer::setCopyTo()
{
   mCopyDirection=COPY_TO;
}

void ByteBuffer::setCopyFrom ()
{
   mCopyDirection=COPY_FROM;
}

bool ByteBuffer::isCopyTo()
{
   return mCopyDirection==COPY_TO;
}

bool ByteBuffer::isCopyFrom ()
{
   return mCopyDirection==COPY_FROM;
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

static bool BB_copyValue(ByteBuffer* aBuffer, void* aValue, int aSize)
{
   //---------------------------------------------------------------------------
   // Guard

   if (aValue == 0)
   {
      aBuffer->mError = ByteErrorCode::BAD_POINTER;
      return false;
   }


   // Guard
   if (aBuffer->mWorkingPtr > aBuffer->mBasePtr + aBuffer->mMaxLength - aSize)
   {
      aBuffer->mError = ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }

   //---------------------------------------------------------------------------
   // Copy value to byte buffer

   if (aBuffer->mCopyDirection == ByteBuffer::COPY_TO)
   {
      // Source pointer
      char* tSource = (char*)aValue;

      if (aBuffer->mByteSwapping)
      {
         // Copy bytes to buffer, swapping bytes
         for (int byteIndex = 0; byteIndex < aSize; byteIndex++)
         {
            aBuffer->mWorkingPtr[byteIndex] = tSource[aSize - byteIndex - 1];
         }
      }
      else
      {
         // Copy bytes to buffer, not swapping bytes
         for (int byteIndex = 0; byteIndex < aSize; byteIndex++)
         {
            aBuffer->mWorkingPtr[byteIndex] = tSource[byteIndex];
         }
      }

      // Adjust buffer members
      aBuffer->mWorkingPtr    += aSize;
      aBuffer->mWorkingLength += aSize;

      return true;
   }
   //---------------------------------------------------------------------------
   else
   {
      // Destination pointer
      char* tDestin = (char*)aValue;

      // Copy bytes from buffer, swapping bytes
      if (aBuffer->mByteSwapping)
      {
         for (int byteIndex = 0; byteIndex < aSize; byteIndex++)
         {
            tDestin[byteIndex] = aBuffer->mWorkingPtr[aSize - byteIndex - 1];
         }
      }
      else
      {
         // Copy bytes from buffer, not swapping bytes
         for (int byteIndex = 0; byteIndex < aSize; byteIndex++)
         {
            tDestin[byteIndex] = aBuffer->mWorkingPtr[byteIndex];
         }
      }

      // adjust members
      aBuffer->mWorkingPtr += aSize;

      return true;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations for value types

bool ByteBuffer::copy (unsigned char*      aValue) { return BB_copyValue(this, aValue, 1); }
bool ByteBuffer::copy (unsigned short*     aValue) { return BB_copyValue(this, aValue, 2); }
bool ByteBuffer::copy (unsigned int*       aValue) { return BB_copyValue(this, aValue, 4); }
bool ByteBuffer::copy (unsigned long long* aValue) { return BB_copyValue(this, aValue, 8); }
bool ByteBuffer::copy (char*               aValue) { return BB_copyValue(this, aValue, 1); }
bool ByteBuffer::copy (short*              aValue) { return BB_copyValue(this, aValue, 2); }
bool ByteBuffer::copy (int*                aValue) { return BB_copyValue(this, aValue, 4); }
bool ByteBuffer::copy (long long*          aValue) { return BB_copyValue(this, aValue, 8); }
bool ByteBuffer::copy (float*              aValue) { return BB_copyValue(this, aValue, 4); }
bool ByteBuffer::copy (double*             aValue) { return BB_copyValue(this, aValue, 8); }

bool ByteBuffer::copy (bool*               aValue)
{
   if (mCopyDirection == ByteBuffer::COPY_TO)
   {
      // Copy boolean value to one byte in the buffer
      unsigned char tFlag = aValue ? 1 : 0;
      return BB_copyValue(this, &tFlag, 1);
   }
   else
   {
      // Copy boolean value from one byte in the buffer
      unsigned char tFlag = 0;
      if (BB_copyValue(this, &tFlag, 1))
      {
         *aValue = (tFlag != 0);
         return true;
      }
      else
      {
         return false;
      }
   }
   return false;
}

bool ByteBuffer::copyEnum (int* aValue) { return BB_copyValue(this, aValue, 4); }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations for null terminated string type

bool ByteBuffer::copyZString (char* aString,int aUpperBound) 
{
   //--------------------------------------------------------------------------
   // Guard

   if(aString==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   //--------------------------------------------------------------------------
   // Get string length

   int tSize = 0;
   if (mCopyDirection == COPY_TO)
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
         if (mWorkingPtr[tSize++] == 0) break;
         if (tSize == aUpperBound)  break;
      }
   }

   //--------------------------------------------------------------------------
   // Guard

   if(mWorkingPtr > mBasePtr + mMaxLength - tSize)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }

   //--------------------------------------------------------------------------
   // Copy

   if (mCopyDirection == COPY_TO)
   {
      // Copy the string to the buffer, without the null character
      memcpy(mWorkingPtr, aString, tSize - 1);

      // copy the null character to the buffer
      mWorkingPtr[tSize - 1] = 0;

      // Adjust buffer members
      mWorkingPtr += tSize;
      mWorkingLength += tSize;
   }
   else
   {
      // Copy the string from the buffer, without the null character
      memcpy(aString, mWorkingPtr, tSize - 1);

      // Copy the null character to the buffer
      aString[tSize - 1] = 0;

      // Adjust buffer members
      mWorkingPtr += tSize;
   }

   return true;
}

bool ByteBuffer::copyZString(unsigned char* aString, int aUpperBound) { return copyZString((char*)aString, aUpperBound); }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations for fixed size string type

bool ByteBuffer::copyFString (char* aString,int aSize) 
{
   //--------------------------------------------------------------------------
   // Guard

   if(aString==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   //--------------------------------------------------------------------------
   // Guard

   if(mWorkingPtr > mBasePtr + mMaxLength - aSize)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }

   //--------------------------------------------------------------------------
   // Copy

   if (mCopyDirection == COPY_TO)
   {
      // Copy the string to the buffer
      memcpy(mWorkingPtr, aString, aSize);

      // Adjust buffer members
      mWorkingPtr    += aSize;
      mWorkingLength += aSize;
   }
   else
   {
      // Copy the string from the buffer, without the null character
      memcpy(aString, mWorkingPtr, aSize);
      aString[aSize] = 0;

      // Adjust buffer members
      mWorkingPtr += aSize;
   }

   return true;
}

bool ByteBuffer::copyFString(unsigned char* aString, int aSize) { return copyFString((char*)aString, aSize); }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy for object that inherits from ByteContent

bool ByteBuffer::copy (ByteContent* content) 
{
   if(content==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   content->copyToFrom(this);

   return true;
}

bool ByteBuffer::putToBuffer (ByteContent* content) 
{
   // guard
   if(content==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // set copy direction
   setCopyTo();

   // call ByteContent supplied member function to do the put
   content->copyToFrom(this);

   return true;
}

bool ByteBuffer::getFromBuffer (ByteContent* content) 
{
   if(content==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // set copy direction
   setCopyFrom();

   // call ByteContent supplied member function to do the get
   content->copyToFrom(this);

   return true;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy raw data

bool ByteBuffer::copyData (void* aData,int aSize) 
{
   // guard
   if(aData==0)
   {
      mError=ByteErrorCode::BAD_POINTER;
      return false;
   }

   // Guard
   if(mWorkingPtr > mBasePtr + mMaxLength - aSize)
   {
      mError=ByteErrorCode::BUFF_OVERFLOW;
      return false;
   }

   if (mCopyDirection==COPY_TO)
   {
      // Copy the data to the buffer
      memcpy(mWorkingPtr,aData,aSize);

      // Adjust members
      mWorkingPtr    += aSize;
      mWorkingLength += aSize;
   }
   else
   {
      // Copy the fixed size data
      memcpy(aData,mWorkingPtr,aSize);

      // Adjust members
      mWorkingPtr += aSize;
   }

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace



