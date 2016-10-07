#ifndef _RISLFBLOCKQUEUE_H_
#define _RISLFBLOCKQUEUE_H_
/*==============================================================================

Lock Free Block Queue.

This implements a queue of fixed size blocks, where the queue provides the
memory allocation for the blocks (memory for the blocks is contained within
the queue). The queue is thread safe. It uses a atomic interlocked compare and
exchanges to guard against concurrency contentions.

It implements the Michael and Scott algorithm with no backoff. It maintains
storage for the blocks by implementing a free list that uses the Trieber 
algorithm with no backoff.

This can be used for Multiple Writer Single Reader.

==============================================================================*/
#include <atomic>
#include "risLFIndex.h"
//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class LFBlockQueue
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Management

   // Constructor
   LFBlockQueue();
  ~LFBlockQueue();

   // Allocate memory for the queue and free list arrays and initialize the
   // queue logic variables. Allocate is the number of blocks to allocate
   // memory for. BlockSize is the block size in bytes.
   void initialize(int aAllocate,int aBlockSize);

   // Deallocate memory.
   void finalize();

   // Queue size
   int  size();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Queue and Free List Methods. These write or read values from the queue
   // and pop or push node indices from the free list.

   // These are used to enqueue a block. StartWrite pops a block from the free
   // list and returns a pointer to it. After writing to the block, FinishWrite
   // is called to enqueue it. The node index is passed between the two methods.

   void* startWrite  (int* aNodeIndex);
   void  finishWrite (int  aNodeIndex);

   // These are used to dequeue a block. StartRead dequeues a block from the 
   // queue and returns a pointer to it. After reading from the block,
   // FinishRead is called to push it onto the free list. The node index is 
   // passed between the two methods.

   void* startRead   (int* aNodeIndex);
   void  finishRead  (int  aNodeIndex);

   // These are called by the above write and read methods.
   bool  listPop     (int* aNode);
   bool  listPush    (int  aNode);

   // Return a pointer to a block, based on its block index.
   void* element(int aIndex);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Queue and Free List Members

   // Number of blocks to allocate
   int mListAllocate;
   // Number of bytes per block
   int mBlockSize;

   // Pointer to allocated block memory.
   // This is an array of blocks.
   void* mMemory;

   // Queue variables
   AtomicLFIndex*    mQueueNext;
   AtomicLFIndex     mQueueHead;
   AtomicLFIndex     mQueueTail;

   // Free List variables
   AtomicLFIndex*    mListNext;
   AtomicLFIndex     mListHead;
   std::atomic<int>  mListSize;

   // Invalid node
   static const int  cInvalid = 0x80000000;
};

//******************************************************************************

}//namespace
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Example

#if 0
   //---------------------------------------------------------------------------
   // Management

   // includes
   #include <new>
   #include "risLFBlockQueue.h"

   // Declare block queue
   Ris::LFBlockQueue mBlockQueue;
   // Initialize block queue
   mBlockQueue.initialize(100,sizeof(Class1A));

   //---------------------------------------------------------------------------
   // Enqueue a block

   // Example counter
   int tWriteCount;
   // This is passed between StartWrite and FinishWrite
   int tWriteIndex;

   // Try to start a write to allocate a block
   void* tBlock = mBlockQueue.startWrite(&tWriteIndex);
   // If the queue is not full
   if (tBlock)
   {
      // Create a new object with placement new on the allocated block.
      // Placement new must be used with any classes that use vtables.
      Class1A* tObject = new(tBlock) Class1A;
      // Access the new object
      tObject->mCode1 = tWriteCount;
      // Finish the write
      mBlockQueue.finishWrite(tWriteIndex);
   }

   //---------------------------------------------------------------------------
   // Dequeue a block 

   // Example counter
   int tReadCount;
   // This is passed between StartRead and FinishRead
   int tReadIndex;

   // Try to start a block read, returns a pointer to an object
   Class1A* tObject = (Class1A*)mBlockQueue.startRead(&tReadIndex);
   // If the queue is not empty
   if (tObject)
   {
      // Access the object
      tReadCount = tObject->mCode1;
      // Finish the read
      mBlockQueue.finishRead(tReadIndex);
   }
#endif

