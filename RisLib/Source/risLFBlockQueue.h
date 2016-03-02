#ifndef _RISLFBLOCKQUEUE_H_
#define _RISLFBLOCKQUEUE_H_
/*==============================================================================

Lock Free Block Queue. 

This implements a fixed size queue. The queue is thread safe. It uses a atomic
interlocked compare and exchanges to guard against concurrency contentions.
It implements the Michael and Scott algorithm with no backoff.

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
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Methods

   // Constructor
   LFBlockQueue();
  ~LFBlockQueue();

   // Allocate memory for the queue and free list arrays and initialize the
   // queue logic variables. aAllocate is the number of pointers to allocate
   // memory for.
   void initialize(int aAllocate,int aBlockSize);

   // Deallocate memory.
   void finalize();

   // Queue size
   int  size();

   // Return a pointer to a block, based on its block index.
  
   void* element(int aIndex);


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Queue and Free List Members

   // Number of values allocated
   int mBlockSize;
   int mAllocate;
   int mQueueAllocate;
   int mListAllocate;

   // Pointer to allocated block memory
   void* mMemory;

   // Queue array and variables
   AtomicLFIndex*    mQueueNext;
   AtomicLFIndex     mQueueHead;
   AtomicLFIndex     mQueueTail;

   // Free List array and variables
   AtomicLFIndex*    mListNext;
   AtomicLFIndex     mListHead;
   std::atomic<int>  mListSize;
   
   std::atomic<int>* mListHeadIndexPtr = (std::atomic<int>*)&mListHead;

   static const int  cInvalid = 0x80000000;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Queue and Free List Methods. These write or read values from the queue
   // and pop or push node indices from the free list.

   void* startWrite  (int* aNodeIndex);
   void  finishWrite (int  aNodeIndex);

   void* startRead   (int* aNodeIndex);
   void  finishRead  (int  aNodeIndex);

   bool  listPop     (int* aNode);
   bool  listPush    (int  aNode);
};

//******************************************************************************

}//namespace
#endif

