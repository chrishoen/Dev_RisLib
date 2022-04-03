#pragma once

/*==============================================================================
Trace buffer. 
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsSynch.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Trc
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a class that establishes a program trace facility that manages
// the storing and displaying of program trace debug print strings. It
// contains four pairs of trace buffers. Each pair contains a buffer that
// stores the first N strings in a trace and a circular buffer that contains
// the last N strings in a trace. Write access to the buffers are protected
// by mutex semaphores.

class TraceBuffer
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Number of buffers.
   static const int cNumBuffers = 4;

   // Number of string elements per buffer.
   static const int cNumElements = 100;

   // Max string element size.
   static const int cMaxStringSize = 99;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // String buffer that stores the first N writes.
   char mBufferFirst[cNumBuffers][cNumElements][cMaxStringSize];

   // Circular string buffer that stores the last N writes.
   char mBufferLast[cNumBuffers][cNumElements][cMaxStringSize];

   // For each buffer pair, the index of the next string to write to. This is
   // incremented after each write and it increases indefinitely. The first
   // buffer is only written to for the first N strings and the last buffer
   // is written to modulo N, where N is the number of string elements
   // in a buffer.
   long long mNextWriteIndex[cNumBuffers];

   // If true then writes are enabled for a buffer pair.
   bool mWriteEnableFlag[cNumBuffers];

   // Mutexes that protects all starts, stops, and writes.
   Ris::Threads::MutexSemaphore mMutex[cNumBuffers];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infrastucture.

   // Constructor,
   TraceBuffer();
   void reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Write.

   // Start a trace on a buffer pair. Reset the write index and enable writes.
   void doStart(int aBufNum);

   // Stop a trace on a buffer pair. Disable writes.
   void doStop(int aBufNum);

   // Resume a stopped trace on a buffer pair. Enable writes. Don't change
   // the write index.
   void doResume(int aBufNum);

   // If enabled, write a string to a buffer pair at the write index and
   // advance the write index. For the first buffer of the pair this only 
   // write the first N strings. For the last buffer of the pair this writes
   // circulary modulo N.
   void doWrite(int aBufNum, const char* aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Helpers.

   // Return a pointer to an element in a first buffer, based on an index.
   char* elementAtFirst(int aBufNum, long long aIndex);

   // Return a pointer to an element in a last buffer, based on an index
   // modulo the number of elements.
   char* elementAtLast(int aBufNum, long long aIndex);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _TRCTRACEBUFFER_CPP_
   TraceBuffer gTraceBuffer;
#else
   extern Buffer gBuffer;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace