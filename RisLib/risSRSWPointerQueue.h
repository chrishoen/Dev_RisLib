#pragma once

/*==============================================================================

Single Reader Writer Pointer Queue Class Template. 

It is single writer single reader thread safe.
It uses no thread synchronization.

This implements a pointer queue. 

It is thread safe for separate single writer and single reader threads.

Ris::SRSWPointerQueue<void*,100> mPointerQueue;

==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

template <class Element,int Size>
class SRSWPointerQueue
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Number of elements allocated is size + 1. There is an extra element
   // allocated. This allows a put operation to occur concurrently to
   // a get operation, because the indices aren't updated until after the
   // operations are finished.
   static const int cNumElements = Size + 1;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Element access indices.
   int mPutIndex;
   int mGetIndex;

   // Array of elements.
   Element mElement[cNumElements];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SRSWPointerQueue()
   {
      reset();
   }

   void reset()
   {
      // Initialize variables.
      mPutIndex = 0;
      mGetIndex = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This is the current size of the queue. It is the difference between the 
   // mPutIndex and the mGetIndex.

   int size()
   {
      int tSize = mPutIndex - mGetIndex;
      if (tSize < 0) tSize = cNumElements + tSize;
      return tSize;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This attempts to start an object write to the queue. If the queue is not
   // full then it succeeds.
   // 
   // This tests if put operations are allowed. Puts are allowed if the 
   // current size is less than or equal to NumElements - 2. If the size is
   // equal to NumElements - 2 then the next put operation would put the size
   // to cNumElements - 1, which is the max number of elements. This is the
   // same as "is not full".
   //
   // If the queue is full, then it returns a null pointer. If the queue is
   // not full, then it returns a pointer to the element to put to. The
   // pointer is then used to write the element. After the write, the finish
   // write is called to advance the put index.
   // 
   // The try write and finish write use temp variables, so that updating
   // in put index is atomic for a single writer thread.

   bool tryWrite (Element aElement)
   {
      // Test if the queue is full.
      int tSize = mPutIndex - mGetIndex;
      if (tSize < 0) tSize = cNumElements + tSize;
      if (tSize > cNumElements - 2) return false;

      // Local put index.
      int tPutIndex = mPutIndex;
      // Copy the source element into the element at the queue put index.
      mElement[tPutIndex] = aElement;
      // Advance the put index.
      if(++tPutIndex == cNumElements) tPutIndex = 0;
      mPutIndex = tPutIndex;
      // Done.
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This attempts to read a value to the queue. If the queue is not empty
   // then it succeeds.
   //
   // This gets an element from the queue and advances the get index. It does a 
   // copy from the queue array element at the get index into a destination
   // element. It uses a temp index variable so that writing to the index is
   // atomic. Note that the destination element must be of element size.
   // It returns the destination element.
  
   Element tryRead()
   {
      // If the queue is empty then return zero.
      int tSize = mPutIndex - mGetIndex;
      if (tSize < 0) tSize = cNumElements + tSize;
      if (tSize == 0) return 0;

      // Local index.
      int tGetIndex = mGetIndex;
      // Copy the queue array element at the get index into a temp.
      Element tPointer = mElement[tGetIndex];
      // Advance the get index.
      if(++tGetIndex == cNumElements) tGetIndex = 0;
      mGetIndex = tGetIndex;

      // Return the temp.
      return tPointer;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This flushes the queue with reads. It deletes the pointers. It returns
   // the number of elements flushed.

   int flushRead()
   {
      // Flush the queue, delete the elements.
      int tCount = 0;
      while (Element tPointer = tryRead())
      {
         delete tPointer;
         tCount++;
      }

      // Return the count.
      return tCount;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

