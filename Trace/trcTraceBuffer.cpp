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
   for (int i = 0; i < cNumTraces; i++)
   {
      mBufferFirst[i] = 0;
      mBufferLast[i] = 0;
      mBufferExists[i] = false;
      mNextWriteIndex[i] = 0;
      mWriteEnable[i] = false;
      mWriteSuspend[i] = false;
      mLogExists[i] = false;
      mLogEnable[i] = false;
      mWriteLevel[i] = 0;
      mLogLevel[i] = 0;
      mMutex[i] = 0;
   }
   mTraceIndexSet.clear();
   mDefaultTraceIndex = 1;
   mDefaultShowSize = 40;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TraceBuffer::initialize()
{}

void TraceBuffer::finalize()
{
   for (const int& tTraceIndex : mTraceIndexSet)
   {
      doStop(tTraceIndex);
      mMutex[tTraceIndex]->lock();
      free(mBufferFirst[tTraceIndex]);
      free(mBufferLast[tTraceIndex]);
      mBufferFirst[tTraceIndex] = 0;
      mBufferLast[tTraceIndex] = 0;
      mBufferExists[tTraceIndex] = false;
      mNextWriteIndex[tTraceIndex] = 0;
      mWriteEnable[tTraceIndex] = false;
      mWriteSuspend[tTraceIndex] = false;
      mLogExists[tTraceIndex] = false;
      mLogEnable[tTraceIndex] = false;
      mWriteLevel[tTraceIndex] = 0;
      mLogLevel[tTraceIndex] = 0;
      mMutex[tTraceIndex]->unlock();
      delete mMutex[tTraceIndex];
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Allocate memory for a trace buffer pair. Set the initial write level
// for the trace.

void TraceBuffer::doCreateBuffer(int aTraceIndex, int aWriteLevel)
{
   // Guard.
   if (aTraceIndex < 1 || aTraceIndex >= cNumTraces) return;

   // Allocate memory for the trace buffer pair and set variables.
   mBufferFirst[aTraceIndex] = (char*)malloc(cNumStrings * (cMaxStringSize + 1));
   mBufferLast[aTraceIndex] = (char*)malloc(cNumStrings * (cMaxStringSize + 1));
   mBufferExists[aTraceIndex] = true;
   mWriteLevel[aTraceIndex] = aWriteLevel;
   mMutex[aTraceIndex] = new Ris::Threads::MutexSemaphore;
   mTraceIndexSet.insert(aTraceIndex);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Validity tests.

// Return true if a trace buffer has been created.
bool TraceBuffer::isValidTrace(int aTraceIndex)
{
   if (aTraceIndex < 1 || aTraceIndex >= cNumTraces) return false;
   return mBufferExists[aTraceIndex];
}

// Return true if a trace buffer log file has been created.
bool TraceBuffer::isValidLog(int aTraceIndex)
{
   if (aTraceIndex < 1 || aTraceIndex >= cNumTraces) return false;
   return mLogExists[aTraceIndex];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pointers.

// Return a pointer to a string element in a first buffer,
// based on an index.
char* TraceBuffer::stringAtFirst(int aTraceIndex, long long aStringIndex)
{
   aStringIndex = aStringIndex < cNumStrings ? aStringIndex : cNumStrings - 1;
   char* tPtr = mBufferFirst[aTraceIndex] + (cMaxStringSize + 1) * aStringIndex;
   return tPtr;
}

// Return a pointer to a string element in a last buffer,
// based on an index modulo the number of elements.
char* TraceBuffer::stringAtLast(int aTraceIndex, long long aStringIndex)
{
   aStringIndex %= cNumStrings;
   char* tPtr = mBufferFirst[aTraceIndex] + (cMaxStringSize + 1) * aStringIndex;
   return tPtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Start a trace on a buffer pair. Reset the write index and enable writes.
// If the buffer number is -1 then start all buffers. 

void TraceBuffer::doStart(int aTraceIndex)
{
   if (!isValidTrace(aTraceIndex)) return;
   mMutex[aTraceIndex]->lock();
   mNextWriteIndex[aTraceIndex] = 0;
   mWriteEnable[aTraceIndex] = true;
   mWriteSuspend[aTraceIndex] = false;
   mMutex[aTraceIndex]->unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stop a trace on a buffer pair. Disable writes.
// If the buffer number is -1 then stop all buffers.

void TraceBuffer::doStop(int aTraceIndex)
{
   if (!isValidTrace(aTraceIndex)) return;
   mMutex[aTraceIndex]->lock();
   mWriteEnable[aTraceIndex] = false;
   mWriteSuspend[aTraceIndex] = false;
   mMutex[aTraceIndex]->unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Resume a stopped trace on a buffer pair. Enable writes. Don't change
// the write index. If the buffer number is -1 then resume all buffers. 

void TraceBuffer::doSuspend(int aTraceIndex)
{
   if (!isValidTrace(aTraceIndex)) return;
   mMutex[aTraceIndex]->lock();
   mWriteSuspend[aTraceIndex] = true;
   mMutex[aTraceIndex]->unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Resume a stopped trace on a buffer pair. Enable writes. Don't change
// the write index. If the buffer number is -1 then resume all buffers. 

void TraceBuffer::doResume(int aTraceIndex)
{
   if (!isValidTrace(aTraceIndex)) return;
   mMutex[aTraceIndex]->lock();
   mWriteSuspend[aTraceIndex] = false;
   mMutex[aTraceIndex]->unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// If enabled, write a string to a buffer pair at the write index and
// advance the write index. For the first buffer of the pair this only 
// write the first N strings. For the last buffer of the pair this writes
// circulary modulo N.

void TraceBuffer::doWrite(int aTraceIndex, int aLevel, const char* aString)
{
   // Guard.
   if (!isValidTrace(aTraceIndex)) return;
   if (!mWriteEnable[aTraceIndex]) return;
   if (mWriteSuspend[aTraceIndex]) return;
   if (mWriteLevel[aTraceIndex] < aLevel) return;

   // Lock.
   mMutex[aTraceIndex]->lock();

   // String destination pointers are determined by the write index.
   char* tDestinFirst = stringAtFirst(aTraceIndex, mNextWriteIndex[aTraceIndex]);
   char* tDestinLast = stringAtLast(aTraceIndex, mNextWriteIndex[aTraceIndex]);

   // Copy to buffer.
   if (mNextWriteIndex[aTraceIndex] < cNumStrings)
   {
      strncpy(tDestinFirst, aString, cMaxStringSize);
      tDestinFirst[cMaxStringSize] = 0;
   }

   // Copy to buffer.
   strncpy(tDestinLast, aString, cMaxStringSize);
   tDestinLast[cMaxStringSize] = 0;

   // Advance the write index.
   mNextWriteIndex[aTraceIndex]++;

   // Unlock.
   mMutex[aTraceIndex]->unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stop tracing and show a first buffer.

void TraceBuffer::doShowFirst(int aTraceIndex, int aShowSize)
{
   if (!isValidTrace(aTraceIndex)) return;
   printf("TRACE FIRST****************************** %d %d\n",aTraceIndex, aShowSize);
   if (mNextWriteIndex[aTraceIndex] == 0)
   {
      printf("EMPTY\n");
      return;
   }
   if (aShowSize > cNumStrings) aShowSize = cNumStrings;
   doStop(aTraceIndex);
   long long tNextWriteIndex = mNextWriteIndex[aTraceIndex];
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
      printf("%4lld $ %s\n", tReadIndex, stringAtFirst(aTraceIndex, tReadIndex));
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stop tracing and show a last buffer.

void TraceBuffer::doShowLast(int aTraceIndex, int aShowSize)
{
   if (!isValidTrace(aTraceIndex)) return;
   printf("TRACE LAST******************************* %d %d\n", aTraceIndex, aShowSize);
   if (mNextWriteIndex[aTraceIndex] == 0)
   {
      printf("EMPTY\n");
      return;
   }
   if (aShowSize > cNumStrings) aShowSize = cNumStrings;
   doStop(aTraceIndex);
   long long tNextWriteIndex = mNextWriteIndex[aTraceIndex];
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
      printf("%4lld $ %s\n", tReadIndex, stringAtLast(aTraceIndex, tReadIndex));
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
   printf("DefaultTraceIndex     %d\n", mDefaultTraceIndex);
   printf("DefaultShowSize       %d\n", mDefaultShowSize);
   printf("\n");
   for (const int& i : mTraceIndexSet)
   {
      printf("%3d %5lld %s\n", i, mNextWriteIndex[i], my_string_from_bool(mWriteEnable[i]));
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute a command line command to manage and show the trace buffers.

void TraceBuffer::execute(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(2, mDefaultTraceIndex);
   aCmd->setArgDefault(3, mDefaultShowSize);
   int tTraceIndex = aCmd->argInt(2);
   int tShowSize = aCmd->argInt(3);
   if (aCmd->isArgString(1, "SHOW"))
   {
      printf("TRACE SHOW\n");
      doShowStatus();
   }
   else if (aCmd->isArgString(1, "START"))
   {
      printf("TRACE START  %d\n", tTraceIndex);
      doStart(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "STOP"))
   {
      printf("TRACE STOP   %d\n", tTraceIndex);
      doStop(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "SUSPEND"))
   {
      printf("TRACE RESUME %d\n", tTraceIndex);
      doSuspend(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "RESUME"))
   {
      printf("TRACE RESUME %d\n", tTraceIndex);
      doResume(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "F"))
   {
      aCmd->setArgDefault(3, 20);
      doSuspend(tTraceIndex);
      doShowFirst(tTraceIndex, tShowSize);
      doResume(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "L"))
   {
      doSuspend(tTraceIndex);
      doShowLast(tTraceIndex, tShowSize);
      doResume(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "B"))
   {
      mDefaultTraceIndex = aCmd->argInt(2);
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