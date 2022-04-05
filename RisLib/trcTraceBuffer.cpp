//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
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
   mDefaultBufNum = 0;
   mDefaultShowSize = 40;
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
// If the buffer number is -1 then start all buffers. 

void TraceBuffer::doStart(int aBufNum)
{
   if (aBufNum < -1 || aBufNum >= cNumBuffers) return;
   if (aBufNum == -1)
   {
      for (int i = 0; i < cNumBuffers; i++)
      {
         doStart(i);
      }
      return;
   }
   mMutex[aBufNum].lock();
   mNextWriteIndex[aBufNum] = 0;
   mWriteEnableFlag[aBufNum] = true;
   mMutex[aBufNum].unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stop a trace on a buffer pair. Disable writes.
// If the buffer number is -1 then stop all buffers.

void TraceBuffer::doStop(int aBufNum)
{
   if (aBufNum < -1 || aBufNum >= cNumBuffers) return;
   if (aBufNum == -1)
   {
      for (int i = 0; i < cNumBuffers; i++)
      {
         doStop(i);
      }
      return;
   }
   mMutex[aBufNum].lock();
   mWriteEnableFlag[aBufNum] = false;
   mMutex[aBufNum].unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Resume a stopped trace on a buffer pair. Enable writes. Don't change
// the write index. If the buffer number is -1 then resume all buffers. 

void TraceBuffer::doResume(int aBufNum)
{
   if (aBufNum < -1 || aBufNum >= cNumBuffers) return;
   if (aBufNum == -1)
   {
      for (int i = 0; i < cNumBuffers; i++)
      {
         doResume(i);
      }
      return;
   }
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
   if (aBufNum < 0 || aBufNum >= cNumBuffers) return;
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
   if (aBufNum < 0 || aBufNum >= cNumBuffers) return;
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
      tStartIndex = 0;
   }
   else
   {
      tLoopSize = tNextWriteIndex;
      tStartIndex = 0;
   }
   for (long long i = 0; i < tLoopSize; i++)
   {
      long long tReadIndex = tStartIndex + i;
      printf("%4lld $ %s\n", tReadIndex, elementAtFirst(aBufNum, tReadIndex));
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stop tracing and show a last buffer.

void TraceBuffer::doShowLast(int aBufNum, int aShowSize)
{
   if (aBufNum < 0 || aBufNum >= cNumBuffers) return;
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
      tStartIndex = tNextWriteIndex - tLoopSize;
   }
   else
   {
      tLoopSize = tNextWriteIndex;
      tStartIndex = tNextWriteIndex - tLoopSize;
   }
   for (long long i = 0; i < tLoopSize; i++)
   {
      long long tReadIndex = tStartIndex + i;
      printf("%4lld $ %s\n", tReadIndex, elementAtLast(aBufNum, tReadIndex));
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show trace buffer status.

void TraceBuffer::doShowStatus()
{
   printf("TRACE STATUS*****************************\n");
   printf("DefaultBufNum     %d\n", mDefaultBufNum);
   printf("DefaultShowSize   %d\n", mDefaultShowSize);
   printf("\n");
   for (int i = 0; i < cNumBuffers; i++)
   {
      printf("%5lld %s\n", mNextWriteIndex[i], my_string_from_bool(mWriteEnableFlag[i]));
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute a command line command to manage and show the trace buffers.

void TraceBuffer::execute(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(2, mDefaultBufNum);
   aCmd->setArgDefault(3, mDefaultShowSize);
   int tBufNum = aCmd->argInt(2);
   int tShowSize = aCmd->argInt(3);
   if (aCmd->isArgString(1, "STATUS"))
   {
      printf("TRACE SHOW\n");
      doShowStatus();
   }
   else if (aCmd->isArgString(1, "START"))
   {
      printf("TRACE START  %d\n", tBufNum);
      doStart(tBufNum);
   }
   else if (aCmd->isArgString(1, "STOP"))
   {
      printf("TRACE STOP   %d\n", tBufNum);
      doStop(tBufNum);
   }
   else if (aCmd->isArgString(1, "RESUME"))
   {
      printf("TRACE RESUME %d\n", tBufNum);
      doResume(tBufNum);
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
   else if (aCmd->isArgString(1, "B"))
   {
      mDefaultBufNum = aCmd->argInt(2);
   }
   else if (aCmd->isArgString(1, "S"))
   {
      mDefaultShowSize = aCmd->argInt(2);
   }
   else
   {
      printf("TRACE INVALID COMMAND\n");
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