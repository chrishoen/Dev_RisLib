#pragma once

/*==============================================================================

Single Reader Writer Object Queue Class Template. 

It is single writer single reader thread safe.
It is uses no thread synchronization.

It is shared memory safe. This means no constructors, no pointers.

It is thread safe for separate single writer and single reader threads.

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
class SRSWObjectQueue
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

   // Initialize, there's no constructor. This is called by the process who
   // first creates the shared memory.
   void initialize()
   {
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

   Element* tryStartWrite()
   {
      // If the queue is full then return zero.
      int tSize = mPutIndex - mGetIndex;
      if (tSize < 0) tSize = cNumElements + tSize;
      if (tSize > cNumElements - 2) return 0;

      // Return a pointer to the element at the put index.
      return &mElement[mPutIndex];
   }

   void finishWrite()
   {
      // Advance the put index.
      int tPutIndex = mPutIndex;
      if (++tPutIndex == cNumElements) tPutIndex = 0;
      mPutIndex = tPutIndex;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This attempts to start an object read from the queue. If the queue is
   // not empty then it succeeds.
   // 
   // This tests if get operations are allowed. Gets are allowed if the 
   // current size is greater than zero.
   //
   // If the queue is empty, then it returns a null pointer. If the queue is
   // not empty, then it returns a pointer to the element to get. The
   // pointer is then used to read the element. After the read, the finish
   // read is called to advance the get index.
   //
   // The try read and finish read use temp variables, so that updating
   // in get index is atomic for a single reader thread.
 
   Element* tryStartRead()
   {
      // If the queue is empty then return zero.
      int tSize = mPutIndex - mGetIndex;
      if (tSize < 0) tSize = cNumElements + tSize;
      if (tSize == 0) return 0;

      // Return a pointer to the element at the get index.
      return &mElement[mGetIndex];
   }

   void finishRead()
   {
      // Local index.
      int tGetIndex = mGetIndex;
      // Advance the get index.
      if (++tGetIndex == cNumElements) tGetIndex = 0;
      mGetIndex = tGetIndex;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

