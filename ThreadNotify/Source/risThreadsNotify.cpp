/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 

#include <atomic> 

#include "risThreadsThreads.h"

#include "risThreadsNotify.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Notify::Notify()
{
   reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Reset the bit mask.

void Notify::reset()
{
   mLock = true;
   for (int i = 0; i < cMaxBits; i++)
   {
      mMask[i]   = false;
      mLatch[i]  = false;
      mStatus[i] = 0;
      mData[i]   = 0;
   }
   mLock = false;
   mAnyFlag = false;
   mEventSem.reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Clear all of the mask and latch bits and set a single mask bit.

void Notify::setMaskAny(int aBitNum)
{
   // Reset all variables and reset the event semaphore.
   reset();
   // Set the mask bit.
   mMask[aBitNum] = true;
   // Set the any flag for the OR trap condition.
   mAnyFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set a bit in the bit latch and conditionally signal the event
// semaphore.

void Notify::notify(int aBitNum)
{
   // Set the latch bit.
   mLatch[aBitNum] = true;

   // Test the masked latch bits for OR and AND trap conditions.
   bool tAnyFound = false;
   bool tAllFound = true;
   for (int i = 0; i < cMaxBits; i++)
   {
      if (mMask[i])
      {
         if (mLatch[i]) tAnyFound = true;
         else           tAllFound = false;
      }
   }
   if (!tAnyFound) tAllFound = false;
      
   // Test if the OR trap condition is true and any latched bits were found
   // or if the AND trap condition is true and all latched birs were found.
   if ((mAnyFlag && tAnyFound) || (!mAnyFlag && tAllFound))
   {
      // Signal the event.
      mEventSem.put();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a bit to be set.

void Notify::wait(int aTimeout)
{
   // Wait for the event.
   mEventSem.get(aTimeout);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

