/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 

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
   HANDLE mEventHandle;

   Specific()
   {
      mTimerHandle = 0;
      mEventHandle = 0;
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
   mWasEventFlag = false;
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
// Start a timer call periodically, in milliseconds

void Waitable::initialize (int aTimerPeriod)
{
   // Initialize variables.
   mTimerCount = 0;
   mWasTimerFlag = false;
   mWasEventFlag = false;

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

   // Create the event.
   mSpecific->mEventHandle = CreateSemaphore(NULL, 0, 1000000, NULL);

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
   CancelWaitableTimer(mSpecific->mTimerHandle);

   // Close the timer and event.
   CloseHandle(mSpecific->mTimerHandle);
   CloseHandle(mSpecific->mEventHandle);

   mSpecific->mTimerHandle = 0;
   mSpecific->mEventHandle= 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for timer or event.

void Waitable::waitForTimerOrEvent()
{
   // Reset variables.
   mWasTimerFlag = false;
   mWasEventFlag = false;

   // Guard.
   if (!mValidFlag) return;

   TS::print(5, "Waitable waitForTimerOrEvent*******************************BEGIN");

   // Wait for the timer or the event.
   DWORD tRet = 0;
   HANDLE tObjects[2];
   tObjects[0] = mSpecific->mTimerHandle;
   tObjects[1] = mSpecific->mEventHandle;
   tRet = WaitForMultipleObjects(2, tObjects, FALSE, -1);
   TS::print(0, "Waitable WaitForMultipleObjects %d",tRet);

   // Test if the timer was signaled.
   tRet = WaitForSingleObject(mSpecific->mTimerHandle, 0);
   if (tRet == WAIT_OBJECT_0)
   {
      TS::print(0, "Waitable WaitForSingleObject Timer");
      // Set the flag.
      mTimerCount++;
      mWasTimerFlag = true;
   }

   // Test if the event was signaled.
   tRet = WaitForSingleObject(mSpecific->mEventHandle, 0);
   if (tRet == WAIT_OBJECT_0)
   {
      TS::print(0, "Waitable WaitForSingleObject Event");
      // Set the flag.
      mWasEventFlag = true;
   }

   TS::print(5, "Waitable waitForTimerOrEvent*******************************END");
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

bool Waitable::wasTimer() { return mWasTimerFlag; }
bool Waitable::wasEvent() { return mWasEventFlag; }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Post to the event. This will unblock any pending waits.

void Waitable::postEvent()
{
   // Guard.
   if (!mValidFlag) return;

   // Write to the event semaphore, increment by one.
   ReleaseSemaphore(mSpecific->mEventHandle, 1, NULL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

