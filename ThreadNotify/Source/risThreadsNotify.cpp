/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdarg.h>

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
   mAnyFlag = false;
   mTimeoutFlag = false;
   mAbortFlag = false;

   mEventSem.reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Clear all of the mask and latch bits and set a single mask bit.

void Notify::setMaskSingle(int aBitNum)
{
   // Reset all variables and reset the event semaphore.
   reset();

   // Set the mask bit.
   mMask[aBitNum] = true;

   // Set the any flag for the OR trap condition.
   mAnyFlag = true;

   // Enable notifications.
   mLock = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Clear all of the mask and latch bits and set a variable list of mask bits.
// Set the trap condition for OR.

void Notify::setMaskAny(int aNumArgs, ...)
{
   // Reset all variables and reset the event semaphore.
   reset();

   // Set the mask bit from variable arguments.
   va_list valist;
   va_start(valist, aNumArgs);
   for (int i = 0; i < aNumArgs; i++)
   {
      int tBitNum = va_arg(valist, int);
      mMask[tBitNum] = true;
   }
   va_end(valist);

   // Set the any flag for the OR trap condition.
   mAnyFlag = true;

   // Enable notifications.
   mLock = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Clear all of the mask and latch bits and set a variable list of mask bits.
// Set the trap condition for AND.

void Notify::setMaskAll(int aNumArgs, ...)
{
   // Reset all variables and reset the event semaphore.
   reset();

   // Set the mask bit from variable arguments.
   va_list valist;
   va_start(valist, aNumArgs);
   for (int i = 0; i < aNumArgs; i++)
   {
      int tBitNum = va_arg(valist, int);
      mMask[tBitNum] = true;
   }
   va_end(valist);

   // Set the any flag for the AND trap condition.
   mAnyFlag = false;

   // Enable notifications.
   mLock = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set a bit in the bit latch and conditionally signal the event
// semaphore.

void Notify::notify(int aBitNum)
{
   // Guard.
   if (mLock) return;

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
// Set the abort bit and signal the event semaphore.

void Notify::abort(int aBitNum)
{
   // Set the abort flag.
   mAbortFlag = false;

   // Signal the event.
   mEventSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a bit to be set. Return false if a timeout or abort occured.

bool Notify::wait(int aTimeout)
{
   // Wait for the event.
   if (mEventSem.get(aTimeout))
   {
      // Not timeout occured. If no abort occured then return true.
      return !mAbortFlag;
   }
   else
   {
      // A timeout occured.
      return false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a specified time. Ignore any bit notifications except an abort.
// Return false if an abort occured.

bool Notify::waitForTimer(int aTimeout)
{
   // Reset all variables.
   reset();
   // Wait for the semaphore to timeout.
   wait(aTimeout);
   // If no abort occured then return true.
   return !mAbortFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

