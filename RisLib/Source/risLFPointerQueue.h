#ifndef _CCLFPOINTERQUEUE_H_
#define _CCLFPOINTERQUEUE_H_
/*==============================================================================

Lock Free Pointer Queue. 

This implements a pointer queue. The queue is thread safe. It uses a atomic
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

class LFPointerQueue
{
public:
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Methods

   // Constructor
   LFPointerQueue();
  ~LFPointerQueue();

   // Allocate memory for the queue and free list arrays and initialize the
   // queue logic variables. aAllocate is the number of pointers to allocate
   // memory for.
   void initialize(int aAllocate);

   // Deallocate memory.
   void finalize();

   // Queue size
   int  size();

   // Write a pointer to the queue. Return false if the queue is full.
   bool  writePtr(void* aPointer);

   // Read a pointer from of the queue. Return null if the queue is empty.
   void* readPtr();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Queue and Free List Members

   // Number of values allocated
   int mAllocate;
   int mQueueAllocate;
   int mListAllocate;

   // Array of values
   void** mValue;

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

   bool tryWrite (void*  aValue);
   bool tryRead  (void** aValue);

   bool listPop  (int*   aNode);
   bool listPush (int    aNode);
};

//******************************************************************************

}//namespace
#endif

