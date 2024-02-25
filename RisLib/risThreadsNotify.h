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
   // Constants.

   // Throw codes.
   static const int cAbortException   = 666;
   static const int cTimeoutException = 667;
   static const int cErrorException   = 668;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If this flag is true then a notify event occured.
   bool mEventFlag;

   // Error code. If not zero then an error occurred. This is set by
   // notifiers.
   int mErrorCode;

   // This indicates the cause of an exception.
   int mExceptionCode;

   // If this true then notification operations are disabled.
   bool mLock;

   // If this true then an abort notification occurred.
   bool mAbortFlag;

   // If this true then a timeout occurred.
   bool mTimeoutFlag;

   // If this true then an error occurred.
   bool mErrorFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Binary semaphore event that the owning thread blocks on. It is signaled
   // by other threads to notify the owning thread of a notification event.
// BinarySemaphore mSem;
   CountingSemaphore mSem;

public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   Notify();
   void reset();
   void clearFlags();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. These are used to initialize and wait for notifications.

   // Test for an exception condition. If an abort, timeout, or error has
   // occurred then throw the corresponding exception.
   void testException();

   // Test for exceptions. Wait for a bit to be set. If the wait times out
   // then set the timeout flag. Test for exceptions.
   void wait(int aTimeout);

   // Test for exceptions. Wait for a specified time. Ignore any bit
   // notifications except an abort. Test for exceptions.
   void waitForTimer(int aTimeout);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. These are used to send notifications.

   // Set a bit in the bit latch and conditionally signal the event semaphore.
   // This wakes up a thread that is waiting for a notification.
   void notify();

   // Set a bit in the bit latch and conditionally signal the event semaphore.
   // Also set the error flag and error code. This wakes up a thread that is
   // waiting for a notification.
   void notifyError(int aError);

   // Set the abort flag and signal the event semaphore. This will abort any 
   // of the above set, test, or wait calls.
   void abort();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


