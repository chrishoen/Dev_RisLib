/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risByteContent.h"
#include "risByteBuffer.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructors.

ByteBuffer::ByteBuffer ()
{
   mWorkingIndex = 0;
   mWorkingLength = 0;
   mSaveIndex = 0;
   mSaveLength = 0;
   mMarkerStartIndex = 0;
   mMarkerEndIndex = 0;
   mMarkerLength = 0;
   mCopyDirection = cCopyTo;
   mLittleEndian = true;
   mError = 0;

   mBaseBytes = 0;
   mMaxLength = 0;
   mMemAllocCode = 0;
}

ByteBuffer::ByteBuffer (int aSize)
{
   mWorkingIndex = 0;
   mWorkingLength = 0;
   mSaveIndex = 0;
   mSaveLength = 0;
   mMarkerStartIndex = 0;
   mMarkerEndIndex = 0;
   mMarkerLength = 0;
   mCopyDirection = cCopyTo;
   mLittleEndian = true;
   mError = 0;

   mBaseBytes = (char*)malloc(aSize);
   mMaxLength = aSize;
   mMemAllocCode = 1;
}

ByteBuffer::ByteBuffer (char* aAddress,int aSize)
{
   mWorkingIndex = 0;
   mWorkingLength = 0;
   mSaveIndex = 0;
   mSaveLength = 0;
   mMarkerStartIndex = 0;
   mMarkerEndIndex = 0;
   mMarkerLength = 0;
   mCopyDirection = cCopyTo;
   mLittleEndian = true;
   mError = 0;

   mBaseBytes = aAddress;
   mMaxLength = aSize;
   mMemAllocCode = 0;
}

void ByteBuffer::initialize(char* aAddress, int aSize)
{
   mWorkingIndex = 0;
   mWorkingLength = 0;
   mSaveIndex = 0;
   mSaveLength = 0;
   mMarkerStartIndex = 0;
   mMarkerEndIndex = 0;
   mMarkerLength = 0;
   mCopyDirection = cCopyTo;
   mLittleEndian = true;
   mError = 0;

   mBaseBytes = aAddress;
   mMaxLength = aSize;
   mMemAllocCode = 0;
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
// Memory management.

void ByteBuffer::memAlloc (int aSize)
{
   mBaseBytes = (char*) malloc(aSize);

   mWorkingIndex  = 0;
   mWorkingLength = 0;
   mMaxLength     = aSize;
   mMemAllocCode  = 1;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Memory management.

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
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Working index.

void ByteBuffer::reset ()
{
   mWorkingIndex  = 0;
   mWorkingLength = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Working index.

void ByteBuffer::rewind ()
{
   mWorkingIndex = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Working index.

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
//******************************************************************************
//******************************************************************************
// Working index.

void ByteBuffer::reverse(int aSize)
{
   if (mWorkingIndex - aSize < 0) return;

   if (mCopyDirection == cCopyTo)
   {
      mWorkingIndex -= aSize;
      mWorkingLength -= aSize;
   }
   else
   {
      mWorkingIndex -= aSize;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Working index.

void ByteBuffer::advance (int aSize)
{
   if (aSize + mWorkingIndex > mMaxLength) return;

   mWorkingIndex  += aSize;
   mWorkingLength += aSize;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Advance to the next byte boundary.

void  ByteBuffer::advanceToNextByteBoundary(int aByteBoundary)
{
   int tOffset = mWorkingLength % aByteBoundary;
   if (tOffset) tOffset = aByteBoundary - tOffset;

   if (tOffset + mWorkingIndex > mMaxLength) return;

   mWorkingIndex += tOffset;
   mWorkingLength += tOffset;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Fill buffer with zeros.

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

void ByteBuffer::fillAll(unsigned char aValue)
{
   for (int i = 0; i < mMaxLength; i++)
   {
      mBaseBytes[i] = (char)aValue;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Buffer position.

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
// Access some members.

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

void ByteBuffer::setLittleEndian()
{
   mLittleEndian = true;
}
void ByteBuffer::setBigEndian()
{
   mLittleEndian = false;
}
bool ByteBuffer::isLittleEndian()
{
   return mLittleEndian;
}
bool ByteBuffer::isBigEndian()
{
   return !mLittleEndian;
}

// Buffer working index marker.
void ByteBuffer::setMarkerStart()
{
   mMarkerStartIndex = mWorkingIndex;
}
int  ByteBuffer::setMarkerEnd()
{
   mMarkerEndIndex = mWorkingIndex;
   mMarkerLength = mMarkerEndIndex - mMarkerStartIndex;
   return mMarkerLength;
}
int ByteBuffer::getMarkerLength()
{
   return mMarkerLength;
}

// Save working index and length.
void ByteBuffer::saveWorking()
{
   mSaveIndex = mWorkingIndex;
   mSaveLength = mWorkingLength;
}

void ByteBuffer::restoreWorking()
{
   mWorkingIndex = mSaveIndex;
   mWorkingLength = mSaveLength;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations.

inline void BB_copyValue(ByteBuffer* aBuffer, void* aValue, int aSize)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Guard.

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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy value to byte buffer.

   if (aBuffer->isCopyTo())
   {
      // Source pointer.
      char* tSource = (char*)aValue;

      // Destination pointer.
      char* tBytes = &aBuffer->mBaseBytes[aBuffer->mWorkingIndex];

      // Copy bytes to buffer.
      for (int i = 0; i < aSize; i++)
      {
         if (aBuffer->mLittleEndian)
         {
            tBytes[i] = tSource[i];
         }
         else
         {
            tBytes[aSize - 1 - i] = tSource[i];
         }
      }

      // Adjust buffer members.
      aBuffer->mWorkingIndex  += aSize;
      aBuffer->mWorkingLength += aSize;
   }
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   else
   {
      // Source pointer.
      char* tBytes = &aBuffer->mBaseBytes[aBuffer->mWorkingIndex];

      // Destination pointer.
      char* tDestin = (char*)aValue;

      // Copy bytes from buffer.
      for (int i = 0; i < aSize; i++)
      {
         if (aBuffer->mLittleEndian)
         {
            tDestin[i] = tBytes[i];
         }
         else
         {
            tDestin[i] = tBytes[aSize - 1 - i];
         }
      }

      // Adjust members.
      aBuffer->mWorkingIndex += aSize;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations for value types.

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
      // Copy boolean value to one byte in the buffer.
      unsigned char tFlag = *aValue ? 1 : 0;
      BB_copyValue(this, &tFlag, 1);
   }
   else
   {
      // Copy boolean value from one byte in the buffer.
      unsigned char tFlag = 0;
      BB_copyValue(this, &tFlag, 1);
      *aValue = (tFlag != 0);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations for null terminated string type. The string is stored in
// the buffer as a two byte unsigned short followed by the string bytes,
// excluding the null termination zero at the end of the string.

void ByteBuffer::copyS (char* aString) 
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy to.

   if (isCopyTo())
   {
      // Get string length.
      int tSize = (int)strlen(aString);

      // Guard.
      if (tSize + mWorkingIndex > mMaxLength)
      {
         setError(ByteBuffer::cBufferOverflow);
         return;
      }

      // Copy string size to two bytes in the buffer.
      unsigned short tUSize = (unsigned short)tSize;
      copy( &tUSize);

      // Copy the string to the buffer, without the null character.
      char* tWorkingPtr = &mBaseBytes[mWorkingIndex];
      memcpy(tWorkingPtr, aString, tSize);

      // Adjust buffer members.
      mWorkingIndex  += tSize;
      mWorkingLength += tSize;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy from.

   else
   {
      // Copy string size from two bytes in the buffer.
      unsigned short tUSize = 0;
      copy( &tUSize);
      int tSize = (int )tUSize;

      // Copy the string from the buffer, there is no null character.
      char* tWorkingPtr = &mBaseBytes[mWorkingIndex];
      memcpy(aString, tWorkingPtr, tSize);

      // Set the string terminating character.
      aString[tSize] = 0;

      // Adjust buffer members.
      mWorkingIndex  += tSize;
   }
}

void ByteBuffer::copyS(unsigned char* aStringPtr) { copyS((char*)aStringPtr); }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations for null terminated string type. The string is stored in
// the buffer as a null terminated string byte,including the null termination
// zero at the end of the string.

void ByteBuffer::copyZ(char* aString, int aMaxStringSize)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy to.

   if (isCopyTo())
   {
      // Get string size, strlen + null termination byte.
      int tSize = (int)strlen(aString) + 1;

      // Guard.
      if (tSize > aMaxStringSize)
      {
         tSize = aMaxStringSize;
      }

      // Guard.
      if (tSize + mWorkingIndex > mMaxLength)
      {
         setError(ByteBuffer::cBufferOverflow);
         return;
      }

      // Copy the string to the buffer, including the null termination byte.
      char* tWorkingPtr = &mBaseBytes[mWorkingIndex];
      memcpy(tWorkingPtr, aString, tSize);

      // Adjust buffer members.
      mWorkingIndex += tSize;
      mWorkingLength += tSize;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy from.

   else
   {
      // Copy the string from the buffer.
      char* tWorkingPtr = &mBaseBytes[mWorkingIndex];
      strncpy(aString, tWorkingPtr, aMaxStringSize - 1);

      // Guard. Set the string null termination byte.
      aString[aMaxStringSize - 1] = 0;

      // Get string size, strlen + null termination byte.
      int tSize = (int)strlen(aString) + 1;

      // Adjust buffer members.
      mWorkingIndex += tSize;
   }
}

void ByteBuffer::copyZ(unsigned char* aStringPtr, int aMaxStringSize) { copyZ((char*)aStringPtr, aMaxStringSize); }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy operations for a block of memory.

void ByteBuffer::copyBlock (void* aValue, int aSize) 
{
   // Guard.
   if (aSize == 0) return;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy to.

   if (isCopyTo())
   {
      // Get string length.
      int tSize = aSize;

      // Guard.
      if (tSize + mWorkingIndex > mMaxLength)
      {
         setError(ByteBuffer::cBufferOverflow);
         return;
      }

      // Copy the block to the buffer.
      char* tWorkingPtr = &mBaseBytes[mWorkingIndex];
      memcpy(tWorkingPtr, aValue, tSize);

      // Adjust buffer members.
      mWorkingIndex  += tSize;
      mWorkingLength += tSize;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy from.

   else
   {
      int tSize = aSize;

      // Copy the value from the buffer.
      char* tWorkingPtr = &mBaseBytes[mWorkingIndex];
      memcpy(aValue, tWorkingPtr, tSize);

      // Adjust buffer members.
      mWorkingIndex  += tSize;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy for object that inherits from ByteContent.

void ByteBuffer::copy (ByteContent* aContent) 
{
   // Call ByteContent overload to copy to/from the buffer.
   aContent->copyToFrom(this);
}

void ByteBuffer::putToBuffer (ByteContent* aContent) 
{
   // Set copy direction.
   setCopyTo();

   // Call ByteContent overload to copy to the buffer.
   aContent->copyToFrom(this);
}

void ByteBuffer::getFromBuffer (ByteContent* aContent) 
{
   // Set copy direction.
   setCopyFrom();

   // Call ByteContent overload to copy to the buffer.
   aContent->copyToFrom(this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Peek operations.

inline void BB_peekValue(ByteBuffer* aBuffer, void* aValue, int aSize)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Guard.

   if (aSize == 0)
   {
      return;
   }

   if (aBuffer->isCopyTo())
   {
      return;
   }

   if (aSize + aBuffer->mWorkingIndex > aBuffer->mWorkingLength)
   {
      aBuffer->setError(ByteBuffer::cBufferOverflow);
      return;
   }

   // Source pointer.
   char* tBytes = &aBuffer->mBaseBytes[aBuffer->mWorkingIndex];

   // Destination pointer.
   char* tDestin = (char*)aValue;

   // Copy bytes from buffer.
   for (int i = 0; i < aSize; i++)
   {
      if (aBuffer->mLittleEndian)
      {
         tDestin[i] = tBytes[i];
      }
      else
      {
         tDestin[i] = tBytes[aSize - 1 - i];
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Peek operations for value types.

void ByteBuffer::peek(unsigned char* aValue) { BB_peekValue(this, aValue, 1); }
void ByteBuffer::peek(unsigned short* aValue) { BB_peekValue(this, aValue, 2); }
void ByteBuffer::peek(unsigned int* aValue) { BB_peekValue(this, aValue, 4); }
void ByteBuffer::peek(char* aValue) { BB_peekValue(this, aValue, 1); }
void ByteBuffer::peek(short* aValue) { BB_peekValue(this, aValue, 2); }
void ByteBuffer::peek(int* aValue) { BB_peekValue(this, aValue, 4); }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void ByteBuffer::show(const char* aLabel, int aSize)
{
   printf("%s %d $ ", aLabel, mWorkingLength);
   for (int i = 0; i < aSize; i++)
   {
      printf("%02x ", (unsigned char)mBaseBytes[i]);
   }
   printf("\n");
}

void ByteBuffer::show(const char* aLabel)
{
   printf("%s $ %d\n", aLabel, mWorkingLength);
   for (int i = 0; i < mWorkingLength; i++)
   {
      if (i % 8 == 0) printf("\n");
      printf("%02x ", (unsigned char)mBaseBytes[i]);
   }
   printf("\n\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace



