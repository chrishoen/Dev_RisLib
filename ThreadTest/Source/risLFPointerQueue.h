#ifndef _RISLFPOINTERQUEUE_H_
#define _RISLFPOINTERQUEUE_H_
/*==============================================================================

Lock Free Pointer Queue. 

This implements a pointer queue. The queue is thread safe. It uses an atomic
interlocked compare and exchange to guard against concurrency contentions. 
It is based on a multiple writer, single reader model. A writer starts a write, 
obtaining a write index to the next queue element that is available to be can 
written to. If the queue is full then the write start fails. The writer then
writes to the corresponding queue element. After the write is complete the 
writer somehow signals the reader to read from the queue. The reader wakes up 
and starts a read, obtaining a read index to the next queue element that is 
available to be read from. If the queue is empty, then the read start fails 
(this should not happen because writer should not signal the reader). The 
reader then reads from the corresponding queue element and then calls finish 
read to update the queue state.

==============================================================================*/

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
   // Methods

   // Constructor
   LFPointerQueue();
  ~LFPointerQueue();

   // Allocate memory for the queue array and initialize the queue logic. 
   // variables. aAllocate is the number of pointers to allocate, the size of
   // the array.
   void initialize(int aAllocate);

   // Write a pointer to the queue. Return false if the queue is full.
   bool  writePtr(void* aPointer);

   // Read a pointer from of the queue. Return null if the queue is empty.
   void* readPtr();

   //---------------------------------------------------------------------------
   // Memory Members

   // Array of pointers, dynamically allocated by initialize.
   void** mArray;

   // Size of the array, number of pointers allocated.
   int mAllocate;

   //---------------------------------------------------------------------------
   // Queue Logic Members

   //---------------------------------------------------------------------------
   // These two variables are each 16 bits and they are packed into a 32 bit 
   // structure because the atomic compare exchange operation used works on
   // 32 bit integers. This limits the queue size to 64K elements. The only 
   // code that can safely change these variables is contained here. Any other
   // code should be read only.
   //
   // WriteIndex is used to circularly index into queue memory for write 
   // operations. ReadAvailable is used to indicate the number of reads that 
   // are available. They have the following ranges:
   //
   //      0 <= WriteIndex    <= Allocate-1
   //      0 <= ReadAvailable <= Allocate
   //
   //      IF ReadAvailable == 0        THEN the queue is empty
   //      IF ReadAvailable == Allocate THEN the queue is full
   //
   //  The ReadIndex is derived from WriteIndex and ReadAvailable.
   //
   //      ReadIndex = WriteIndex - ReadAvailable;
   //      IF ReadIndex < 0 THEN ReadIndex = ReadIndex + Allocate;
   //---------------------------------------------------------------------------

   typedef union
   {
       struct    
       { 
         unsigned short mWriteIndex;  
         unsigned short mReadAvailable;  
       } Parms;
       signed mPacked;
   } LFQueueParms;

   LFQueueParms mParms;

   //---------------------------------------------------------------------------
   // Queue Logic Methods

   // This is called to start a write operation. If the queue is not full then
   // it succeeds. It updates the variable pointed by the input pointer with the 
   // WriteIndex that is to be used to access queue memory for the write, it
   // increments ReadAvailable and returns true. If it fails because the queue is 
   // full then it returns false.
   bool tryStartWrite (int* aWriteIndex);

   // This is a place holder.
   void finishWrite();

   // This is called to start a read operation. If the queue is not empty then it 
   // succeeds, it  updates the variable pointed by the input pointer with the 
   // ReadIndex that is to be used to access queue memory for the read and returns 
   // true. If it fails because the queue is empty then it returns false.
   bool tryStartRead  (int* aReadIndex);

   // This is called to finish a read operation. It decrements ReadAvailable.
   void finishRead();

};

//******************************************************************************

}//namespace
#endif

