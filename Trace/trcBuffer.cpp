//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"

#define  _TRCBUFFER_CPP_
#include "trcBuffer.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Trc
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Buffer::Buffer()
{
   reset();
}

void Buffer::reset()
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
char* Buffer::elementAtFirst(int aBufNum, long long aIndex)
{
   aIndex = aIndex < cNumElements ? aIndex : cNumElements - 1;
   return mBufferFirst[aBufNum][aIndex];
}

// Return a pointer to an element in a last buffer, based on an index
// modulo the number of elements.
char* Buffer::elementAtLast(int aBufNum, long long aIndex)
{
   aIndex %= cNumElements;
   return mBufferLast[aBufNum][aIndex];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Start a trace on a buffer pair. Reset the write index and enable writes.

void Buffer::doStart(int aBufNum)
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

void Buffer::doStop(int aBufNum)
{
   mMutex[aBufNum].lock();
   mWriteEnableFlag[aBufNum] = false;
   mMutex[aBufNum].unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// If enabled, write a string to a buffer pair at the write index and
// advance the write index. For the first buffer of the pair this only 
// write the first N strings. For the last buffer of the pair this writes
// circulary modulo N.

void Buffer::doWrite(int aBufNum, const char* aString)
{

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace