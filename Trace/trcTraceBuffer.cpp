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
   if (!mWriteEnableFlag[aBufNum]) return;

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

void TraceBuffer::doShowFirst(int aBufNum, int aShowSize)
{
   printf("TRACE FIRST****************************** %d %d\n",aBufNum, aShowSize);
   if (mNextWriteIndex[aBufNum] == 0)
   {
      printf("EMPTY\n");
      return;
   }
   if (aShowSize > cNumElements) aShowSize = cNumElements;
   doStop(aBufNum);
   long long tNextWriteIndex = mNextWriteIndex[aBufNum];
   long long tShowSize = (long long)aShowSize;
   long long tStartIndex = 0;
   long long tLoopSize = 0;
   if (tNextWriteIndex > tShowSize)
   {
      tLoopSize = tShowSize;
      long long tStartIndex = 0;
   }
   else
   {
      tLoopSize = tNextWriteIndex;
      long long tStartIndex = 0;
   }
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

void TraceBuffer::doShowLast(int aBufNum, int aShowSize)
{
   printf("TRACE LAST******************************* %d %d\n", aBufNum, aShowSize);
   if (mNextWriteIndex[aBufNum] == 0)
   {
      printf("EMPTY\n");
      return;
   }
   if (aShowSize > cNumElements) aShowSize = cNumElements;
   doStop(aBufNum);
   long long tNextWriteIndex = mNextWriteIndex[aBufNum];
   long long tShowSize = (long long)aShowSize;
   long long tStartIndex = 0;
   long long tLoopSize = 0;
   if (tNextWriteIndex > tShowSize)
   {
      tLoopSize = tShowSize;
      long long tStartIndex = tNextWriteIndex - tLoopSize;
   }
   else
   {
      tLoopSize = tNextWriteIndex;
      long long tStartIndex = tNextWriteIndex - tLoopSize;
   }
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
// Execute a command line command to manage and show the trace buffers.

void TraceBuffer::execute(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(2, 1);
   aCmd->setArgDefault(3, 20);
   int tBufNum = aCmd->argInt(2);
   int tShowSize = aCmd->argInt(3);
   if (aCmd->isArgString(1, "START"))
   {
      printf("TRACE START %d\n", tBufNum);
      doStart(tBufNum);
   }
   else if (aCmd->isArgString(1, "STOP"))
   {
      printf("TRACE STOP  %d\n", tBufNum);
      doStop(tBufNum);
   }
   else if (aCmd->isArgString(1, "F"))
   {
      aCmd->setArgDefault(3, 20);
      doShowFirst(tBufNum, tShowSize);
   }
   else if (aCmd->isArgString(1, "L"))
   {
      doShowLast(tBufNum, tShowSize);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace