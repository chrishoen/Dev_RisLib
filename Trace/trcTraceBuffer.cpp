//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"

#define  _TRCTRACEBUFFER_CPP_
#include "trcTraceBuffer.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Trc
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TraceBuffer::TraceBuffer()
{
   reset();
}

void TraceBuffer::reset()
{
   for (int i = 0; i < cNumBuffers; i++)
   {
      mNextWriteIndex[i] = 0;
      mWriteEnableFlag[i] = false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pointers.

// Return a pointer to an element in a first buffer, based on an index.
char* TraceBuffer::elementAtFirst(int aBufNum, long long aIndex)
{
   aIndex = aIndex < cNumElements ? aIndex : cNumElements - 1;
   return mBufferFirst[aBufNum][aIndex];
}

// Return a pointer to an element in a last buffer, based on an index
// modulo the number of elements.
char* TraceBuffer::elementAtLast(int aBufNum, long long aIndex)
{
   aIndex %= cNumElements;
   return mBufferLast[aBufNum][aIndex];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Start a trace on a buffer pair. Reset the write index and enable writes.

void TraceBuffer::doStart(int aBufNum)
{
   mMutex[aBufNum].lock();
   mNextWriteIndex[aBufNum] = 0;
   mWriteEnableFlag[aBufNum] = true;
   mMutex[aBufNum].unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stop a trace on a buffer pair. Disable writes.

void TraceBuffer::doStop(int aBufNum)
{
   mMutex[aBufNum].lock();
   mWriteEnableFlag[aBufNum] = false;
   mMutex[aBufNum].unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Resume a stopped trace on a buffer pair. Enable writes. Don't change
// the write index.

void TraceBuffer::doResume(int aBufNum)
{
   mMutex[aBufNum].lock();
   mWriteEnableFlag[aBufNum] = true;
   mMutex[aBufNum].unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// If enabled, write a string to a buffer pair at the write index and
// advance the write index. For the first buffer of the pair this only 
// write the first N strings. For the last buffer of the pair this writes
// circulary modulo N.

void TraceBuffer::doWrite(int aBufNum, const char* aString)
{
   // Guard.
   if (mWriteEnableFlag[aBufNum]) return;

   // Lock.
   mMutex[aBufNum].lock();

   // String destination pointers are determined by the write index.
   char* tDestinFirst = elementAtFirst(aBufNum, mNextWriteIndex[aBufNum]);
   char* tDestinLast = elementAtLast(aBufNum, mNextWriteIndex[aBufNum]);

   // Copy to buffer.
   if (mNextWriteIndex[aBufNum] < cNumElements)
   {
      strncpy(tDestinFirst, aString, cMaxStringSize);
      tDestinFirst[cMaxStringSize] = 0;
   }

   // Copy to buffer.
   strncpy(tDestinLast, aString, cMaxStringSize);
   tDestinLast[cMaxStringSize] = 0;

   // Advance the write index.
   mNextWriteIndex[aBufNum]++;

   // Unlock.
   mMutex[aBufNum].unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stop tracing and show a first buffer.

void TraceBuffer::doShowFirst(int aBufNum, int aNumStrings)
{
   printf("TRACE FIRST*******************************\n");
   if (mNextWriteIndex[aBufNum] == 0) return;
   doStop(aBufNum);
   long long tNextWriteIndex = mNextWriteIndex[aBufNum];
   long long tNumStrings = (long long)aNumStrings;
   long long tLoopSize = tNextWriteIndex > tNumStrings ? tNumStrings : tNextWriteIndex;
   long long tStartIndex = 0;
   for (long long i = 0; i < tLoopSize; i++)
   {
      long long tReadIndex = tStartIndex + i;
      printf("%3lld $ %s\n", tReadIndex, elementAtFirst(aBufNum, tReadIndex));
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stop tracing and show a last buffer.

void TraceBuffer::doShowLast(int aBufNum, int aNumStrings)
{
   printf("TRACE LAST*******************************\n");
   if (mNextWriteIndex[aBufNum] == 0) return;
   doStop(aBufNum);
   long long tNextWriteIndex = mNextWriteIndex[aBufNum];
   long long tNumStrings = (long long)aNumStrings;
   long long tLoopSize = tNextWriteIndex > tNumStrings ? tNumStrings : tNextWriteIndex;
   long long tStartIndex = tNextWriteIndex - 1 - tLoopSize;
   for (long long i = 0; i < tLoopSize; i++)
   {
      long long tReadIndex = tStartIndex + i;
      printf("%3lld $ %s\n", tReadIndex, elementAtFirst(aBufNum, tReadIndex));
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace