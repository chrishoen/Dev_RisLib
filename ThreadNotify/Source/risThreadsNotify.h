#pragma once

/*==============================================================================
Thread notification object.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsSynch.h"
#include "risLogic.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// The class provides a mechanism that allows a thread to wait for 
// notifications from other threads.


class Notify
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constanst.

   static const int cMaxBits = 32;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Array of mask bits.
   bool mMask[cMaxBits];

   // Array of latch bits.
   bool mLatch[cMaxBits];

   // Array of returned status codes.
   int mStatus[cMaxBits];

   // Array of returned data pointers.
   void* mData[cMaxBits];

   // If this true then notification operations are disabled.
   bool mLock;

   // If this flag is true then the setting of any masked latch bit will
   // cause an event to be signaled. If this flag is false then the setting
   // of all masked latch bits will cause an event to be signaled.
   bool mAnyFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Binary semaphore event that the owning thread blocks on. It is signaled
   // by other threads to notify the owning thread of a notification event.
   BinarySemaphore mEventSem;

public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   Notify();
   void reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Clear all of the mask and latch bits and set a single mask bit.
   void setMaskAny(int aBitNum);

   // Set a bit in the bit latch and conditionally signal the event
   // semaphore.
   void notify(int aBitNum);

   // Wait for a bit to be set.
   void wait(int aTimeout);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


