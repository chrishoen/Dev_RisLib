/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <stdlib.h>

#include "risLFBlockQueue.h"

using namespace std;

namespace Ris
{
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************

   LFBlockQueue::LFBlockQueue()
   {
      // All null
      mMemory = 0;
      mQueueNext = 0;
      mListNext = 0;
      mListAllocate = 0;
   }

   LFBlockQueue::~LFBlockQueue()
   {
      finalize();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize

   void LFBlockQueue::initialize(int aAllocate,int aBlockSize)
   {
      finalize();

      mBlockSize     = aBlockSize;
      mListAllocate  = aAllocate + 1;

      mMemory = malloc(mListAllocate*mBlockSize);
      mQueueNext = new AtomicLFIndex[mListAllocate];
      mListNext = new AtomicLFIndex[mListAllocate];

      for (int i = 0; i < mListAllocate-1; i++)
      {
         mQueueNext[i].store(LFIndex(cInvalid,0));
         mListNext[i].store(LFIndex(i+1,0));
      }

      mQueueNext[mListAllocate-1].store(LFIndex(cInvalid,0));
      mListNext[mListAllocate-1].store(LFIndex(cInvalid,0));

      mListHead.store(LFIndex(0,0));
      mListSize = mListAllocate;

      int tIndex;
      listPop(&tIndex);
      mQueueHead.store(LFIndex(tIndex,0));
      mQueueTail = mQueueHead.load();

   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Finalize

   void LFBlockQueue::finalize()
   {
      if (mMemory)    free(mMemory);
      if (mQueueNext) free(mQueueNext);
      if (mListNext)  free(mListNext);
      mMemory    = 0;
      mQueueNext = 0;
      mListNext  = 0;
   }

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Return a pointer to a block, based on block array index

   void* LFBlockQueue::element(int aIndex)
   {
      return (void*)((char*)mMemory + mBlockSize*aIndex);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Size

   int LFBlockQueue::size()
   { 
      return mListAllocate - mListSize.load(memory_order_relaxed);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This attempts to write a value to the queue. If the queue is not full
   // then it succeeds. It attempts to pop a node from the free list. If
   // the free list is empty then the queue is full and it exits. The value
   // is to be written is stored in the new node. The new node is then attached
   // to the queue tail node and the tail index is updated.

   void* LFBlockQueue::startWrite(int* aNodeIndex)
   {
      // Try to allocate a node from the free list.
      // Exit if it is empty.
      int tNodeIndex;
      if (!listPop(&tNodeIndex)) return 0;

      // Return a pointer to the node block.
      *aNodeIndex = tNodeIndex;
      return element(tNodeIndex);
   }

   void LFBlockQueue::finishWrite(int aNodeIndex)
   {
      // Initialize the node.
      int tNodeIndex = aNodeIndex;
      mQueueNext[tNodeIndex].store(LFIndex(cInvalid, 0), memory_order_relaxed);

      // Attach the node to the queue tail.
      LFIndex tTail,tNext;

      int tLoopCount=0;
      while (true)
      {
         tTail = mQueueTail.load(memory_order_relaxed);
         tNext = mQueueNext[tTail.mIndex].load(memory_order_acquire);

         if (tTail == mQueueTail.load(memory_order_relaxed))
         {
            if (tNext.mIndex == cInvalid)
            {
               if (mQueueNext[tTail.mIndex].compare_exchange_strong(tNext, LFIndex(tNodeIndex, tNext.mCount+1),memory_order_release,memory_order_relaxed)) break;
            }
            else
            {
               mQueueTail.compare_exchange_weak(tTail, LFIndex(tNext.mIndex, tTail.mCount+1),memory_order_release,memory_order_relaxed);
            }
         }

         if (++tLoopCount==10000) throw 101;
      }

      mQueueTail.compare_exchange_strong(tTail, LFIndex(tNodeIndex, tTail.mCount+1),memory_order_release,memory_order_relaxed);
   }

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This attempts to read a value from the queue. If the queue is not empty
   // then it succeeds. The next node in the queue to be read is the one 
   // immedialtely after the head node. It extracts the read value from the read
   // node, pushes the previous head node back onto the free list and updates the
   // head index.

   void* LFBlockQueue::startRead(int* aNodeIndex)
   {
      void* tBlockPtr = 0;
      LFIndex tHead, tTail, tNext;

      int tLoopCount=0;
      while (true)
      {
         tHead = mQueueHead.load(memory_order_relaxed);
         tTail = mQueueTail.load(memory_order_acquire);
         tNext = mQueueNext[tHead.mIndex].load(memory_order_relaxed);

         if (tHead == mQueueHead.load(memory_order_acquire))
         {
            if (tHead.mIndex == tTail.mIndex)
            {
               if (tNext.mIndex == cInvalid) return 0;
               mQueueTail.compare_exchange_strong(tTail, LFIndex(tNext.mIndex, tTail.mCount+1),memory_order_release,memory_order_relaxed);
            }
            else
            {
               tBlockPtr = element(tNext.mIndex);
               if (mQueueHead.compare_exchange_strong(tHead, LFIndex(tNext.mIndex, tHead.mCount+1),memory_order_acquire,memory_order_relaxed))break;
            }
         }

         if (++tLoopCount==10000) throw 102;
      }

      *aNodeIndex = tHead.mIndex;

      // Done.
      return tBlockPtr;
   }

   void LFBlockQueue::finishRead(int aNodeIndex)
   {
      listPush(aNodeIndex);
   }

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This detaches the head node.

   bool LFBlockQueue::listPop(int* aNode)
   {
      // Store the head node in a temp.
      // This is the node that will be detached.
      LFIndex tHead = mListHead.load(memory_order_relaxed);

      int tLoopCount=0;
      while (true)
      {
         // Exit if the list is empty.
         if (tHead.mIndex == cInvalid) return false;

         // Set the head node to be the node that is after the head node.
         if (mListHead.compare_exchange_weak(tHead, LFIndex(mListNext[tHead.mIndex].load(memory_order_relaxed).mIndex,tHead.mCount+1),memory_order_acquire,memory_order_relaxed)) break;

         if (++tLoopCount==10000) throw 103;
      }

      // Return the detached original head node.
      *aNode = tHead.mIndex;

      // Done.
      mListSize.fetch_sub(1,memory_order_relaxed);
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Insert a node into the list before the list head node.

   bool LFBlockQueue::listPush(int aNode)
   {
      // Store the head node in a temp.
      LFIndex tHead = mListHead.load(memory_order_relaxed);

      int tLoopCount=0;
      while (true)
      {
         // Attach the head node to the pushed node.
         mListNext[aNode].store(tHead,memory_order_relaxed);

         // The pushed node is the new head node.
         if ((*mListHeadIndexPtr).compare_exchange_weak(tHead.mIndex, aNode,memory_order_release,memory_order_relaxed)) break;
         if (++tLoopCount == 10000) throw 103;
      }

      // Done.
      mListSize.fetch_add(1,memory_order_relaxed);
      return true;
   }
}//namespace