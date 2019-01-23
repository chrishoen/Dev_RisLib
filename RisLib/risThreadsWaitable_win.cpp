/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 

#include <atomic> 

#include "risThreadsThreads.h"

#include "risThreadsWaitable.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Class definition for implementation specific.

class Waitable::Specific
{
public:
   HANDLE mTimerHandle;
   HANDLE mSemaphoreHandle;
   std::atomic<unsigned> mSemaphoreCount;

   Specific()
   {
      mTimerHandle = 0;
      mSemaphoreHandle = 0;
      mSemaphoreCount.store(0);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Waitable::Waitable()
{
   // Initialize members.
   mTimerCount = 0;
   mWasTimerFlag = false;
   mWasSemaphoreFlag = false;
   mValidFlag = false;

   // Create new specific implementation.
   mSpecific = new Specific;
}

Waitable::~Waitable()
{
   finalize();
   delete mSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize the timer and the counting semaphore. Start the timer
// periodically, in milliseconds.

void Waitable::initialize (int aTimerPeriod)
{
   // Initialize variables.
   mTimerCount = 0;
   mWasTimerFlag = false;
   mWasSemaphoreFlag = false;

   // If using the timer.
   if (aTimerPeriod > 0)
   {
      // Create the timer.
      mSpecific->mTimerHandle = CreateWaitableTimer(NULL, FALSE, NULL);

      // Calculate the timer interval. (100ns increments)
      LARGE_INTEGER tDueTime;
      tDueTime.QuadPart = -1 * aTimerPeriod * 1000 * 10;

      // Set the timer interval.
      SetWaitableTimer(mSpecific->mTimerHandle, &tDueTime, aTimerPeriod, NULL, NULL, 0);
   }
   // If not using the timer.
   else
   {
      mSpecific->mTimerHandle = 0;
   }

   // Create the semaphore.
   mSpecific->mSemaphoreHandle = CreateSemaphore(NULL, 0, 1000000, NULL);

   // Set valid.
   mValidFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Waitable::finalize()
{
   // Guard.
   if (!mValidFlag) return;

   // Set invalid.
   mValidFlag = false;

   // Cancel the timer.
   if (mSpecific->mTimerHandle) CancelWaitableTimer(mSpecific->mTimerHandle);

   // Close the timer and semaphore.
   if (mSpecific->mTimerHandle) CloseHandle(mSpecific->mTimerHandle);
   if (mSpecific->mSemaphoreHandle) CloseHandle(mSpecific->mSemaphoreHandle);

   mSpecific->mTimerHandle = 0;
   mSpecific->mSemaphoreHandle= 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for the timer or counting semaphore.

void Waitable::waitForTimerOrSemaphore()
{
   // Reset the flags.
   mWasTimerFlag = false;
   mWasSemaphoreFlag = false;

   // Guard.
   if (!mValidFlag) return;

   TS::print(6, "Waitable waitForTimerOrSemaphore*******************************BEGIN");

   // If waiting for timer or semaphore.
   if (mSpecific->mTimerHandle)
   {
      // Wait for the timer or the semaphore.
      DWORD tRet = 0;
      HANDLE tObjects[2];
      tObjects[0] = mSpecific->mTimerHandle;
      tObjects[1] = mSpecific->mSemaphoreHandle;
      tRet = WaitForMultipleObjects(2, tObjects, FALSE, -1);
      TS::print(5, "Waitable WaitForMultipleObjects %d", tRet);

      // Test if the timer was signaled or both the timer and the semaphore
      // were signaled.
      if (tRet == WAIT_OBJECT_0)
      {
         // The timer was signaled and maybe the semaphore was signaled.
         TS::print(5, "Waitable timer or both");

         // Increment the timer count and set the flag.
         mTimerCount++;
         mWasTimerFlag = true;

         // Test the semaphore count.
         if (mSpecific->mSemaphoreCount > 0)
         {
            // The semaphore was signaled. Decrement the semaphore count by one.
            mSpecific->mSemaphoreCount.fetch_sub(1);
            // Set the flag.
            mWasSemaphoreFlag = true;
         }
      }

      // Test if only the semaphore was signaled.
      if (tRet == WAIT_OBJECT_0 + 1)
      {
         // The timer was not signaled and the semaphore was signaled.
         TS::print(5, "Waitable semaphore only");

         // Test the semaphore count.
         if (mSpecific->mSemaphoreCount > 0)
         {
            // The semaphore was signaled. Decrement the semaphore count by one.
            mSpecific->mSemaphoreCount.fetch_sub(1);
            // Set the flag.
            mWasSemaphoreFlag = true;
         }
      }
   }
   // If waiting for semaphore only.
   else
   {
      // Wait for the semaphore.
      DWORD tRet = 0;
      tRet = WaitForSingleObject(mSpecific->mSemaphoreHandle, -1);
      TS::print(5, "Waitable WaitForSingleObject %d", tRet);

      // Test if the semaphore was signaled.
      if (tRet == WAIT_OBJECT_0)
      {
         // The semaphore was signaled.
         TS::print(5, "Waitable semaphore only");

         // Test the semaphore count.
         if (mSpecific->mSemaphoreCount > 0)
         {
            // The semaphore was signaled. Decrement the semaphore count by one.
            mSpecific->mSemaphoreCount.fetch_sub(1);
            // Set the flag.
            mWasSemaphoreFlag = true;
         }
      }
   }

   TS::print(6, "Waitable waitForTimerOrSemaphore*******************************END");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return true if the previous wait unblocking was a result of the timer 
// or the counting semaphore.

bool Waitable::wasTimer() { return mWasTimerFlag; }
bool Waitable::wasSemaphore() { return mWasSemaphoreFlag; }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Post to the counting semaphore.

void Waitable::postSemaphore()
{
   // Guard.
   if (!mValidFlag) return;

   // Increment the semaphore count by one.
   mSpecific->mSemaphoreCount.fetch_add(1);

   // Write to the counting semaphore, increment by one.
   ReleaseSemaphore(mSpecific->mSemaphoreHandle, 1, NULL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

