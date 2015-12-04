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
   mWorkingPtr=0;
   mWorkingLength=0;                               
   mMaxLength=0;
   mMemAllocCode=0;
   mCopyDirection=COPY_TO;
   mError=0;
   mByteSwapping=gByteBuffer_DoByteSwapping;
}

ByteBuffer::ByteBuffer (char* address,int size)
{
   mBasePtr=address;
   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
   mMaxLength=size;
   mMemAllocCode=0;
   mCopyDirection=COPY_TO;
   mError=0;
   mByteSwapping=gByteBuffer_DoByteSwapping;
}

ByteBuffer::ByteBuffer (int size,bool fillZero)
{
   memAlloc(size);
   mCopyDirection=COPY_TO;
   mError=0;
   mByteSwapping=gByteBuffer_DoByteSwapping;

   if (fillZero)
   {
      memset(mBasePtr,0,size);
   }
}

ByteBuffer::~ByteBuffer ()
{
  if(mMemAllocCode)
  {
     free((void*)mBasePtr);
  }
}

//******************************************************************************
// Buffer base address

void ByteBuffer::setBaseAddress (char* address,int size)
{
   memFree();

   mBasePtr=address;
   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
   mMaxLength=size;
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
// Memory management

void ByteBuffer::memAlloc (int size)
{
   mBasePtr = (char*) malloc(size);

   mWorkingPtr=mBasePtr;
   mWorkingLength=0;
   mMaxLength=size;
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

void ByteBuffer::forward (int size)
{
   if (size + mWorkingPtr - mBasePtr > mMaxLength) return;

   if (mCopyDirection==COPY_TO)
   {
      mWorkingPtr    += size;
      mWorkingLength += size;
   }
   else
   {
      mWorkingPtr    += size;
   }
}

//******************************************************************************

void ByteBuffer::advance (int size)
{
   if (size + mWorkingPtr - mBasePtr > mMaxLength) return;

   mWorkingPtr    += size;
   mWorkingLength += size;
}

//******************************************************************************

void ByteBuffer::fillZero (int size)
{
   if (size + mWorkingPtr - mBasePtr > mMaxLength) return;

   if (mCopyDirection==COPY_TO)
   {
      for (int i=0;i<size;i++)
      {
         mWorkingPtr[i] = 0;
      }

      mWorkingPtr    += size;
      mWorkingLength += size;
   }
   else
   {
      mWorkingPtr    += size;
   }
}

//******************************************************************************
// buffer position

bool ByteBuffer::setPosition (char* address)
{
   mWorkingPtr=address;
   return true;
}

bool ByteBuffer::setPosition (int index)
{
   if (index>=mMaxLength) return false;

   mWorkingPtr=&mBasePtr[index];

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
void ByteBuffer::show()
{
   int tLength = mMaxLength;
   if (tLength>50) tLength=50;
	printf(" 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9\n");
	for (int i=0;i<tLength;i++)
	{
		printf("%02X",(unsigned char)mBasePtr[i]);
	}
   printf("\n\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// these only set or get values of some member variables

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

int ByteBuffer::getError()
{
   return mError;
}

void ByteBuffer::setError(int value)
{
   mError=value;
}

int ByteBuffer::getLength()
{
   return mWorkingLength;
}

void ByteBuffer::setLength(int value)
{
   mWorkingLength=value;
}

int ByteBuffer::getMaxLength()
{
   return mMaxLength;
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

//------------------------------------------------------------------------------

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
}//namespace



