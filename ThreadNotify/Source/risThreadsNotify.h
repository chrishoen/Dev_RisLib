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

   // Maximum number of bits.
   static const int cMaxBits = 32;

   // Maximum string size.
   static const int cMaxStringSize = 100;

   // Throw codes.
   static const int cAbortException   = 666;
   static const int cTimeoutException = 667;
   static const int cErrorException   = 668;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Label string.
   char mLabel[cMaxStringSize];

   // Array of mask bits.
   bool mMask[cMaxBits];

   // Array of latch bits.
   bool mLatch[cMaxBits];

   // If this flag is true then the setting of any masked latch bit will
   // cause an event to be signaled. If this flag is false then the setting
   // of all masked latch bits will cause an event to be signaled.
   bool mAnyFlag;

   // Error code. If not zero then an error occurred. This is set by
   // notifiers.
   int mErrorCode;

   // This indicates the cause of an exception.
   int mExceptionCode;

   // Exception description string.
   char mException[cMaxStringSize];

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
   BinarySemaphore mEventSem;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Binary semaphore event that the owning thread blocks on if a suspend 
   // has been requested. A resume request posts to this semaphore.
   BinarySemaphore mResumeSem;

   // If this true then a suspend has been requested.
   bool mSuspendFlag;

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
   // Methods. These are used to receive notifications.

   // Clear all of the mask and latch bits and set a single mask bit.
   void setMaskOne(int aBitNum);
   void setMaskOne(const char* aLabel,int aBitNum);

   // Clear all of the mask and latch bits and set a variable list of mask
   // bits. Set the trap condition for OR.
   void setMaskAny(int aNumArgs, ...);
   void setMaskAny(const char* aLabel,int aNumArgs, ...);

   // Clear all of the mask and latch bits and set a variable list of mask
   // bits. Set the trap condition for AND.
   void setMaskAll(int aNumArgs, ...);
   void setMaskAll(const char* aLabel,int aNumArgs, ...);

   // Test for an exception condition. If an abort, timeout, or error
   // has occurred then throw the corresponding exception.
   void testException();

   // Test the suspend request flag . If a suspend has been requested then
   // block on the resume semaphore until it is signalled by a resume.
   // Return true if a suspend and resume occurred.
   bool testSuspend();

   // Wait for a bit to be set. Test for exceptions.
   void wait(int aTimeout);

   // Wait for a specified time. Ignore any bit notifications except an abort.
   // Test for exceptions.
   void waitForTimer(int aTimeout);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. These are used to send notifications.

   // Set a bit in the bit latch and conditionally signal the event
   // semaphore.
   void notify(int aBitNum);

   // Set a bit in the bit latch and conditionally signal the event
   // semaphore. Also set the error code.
   void notifyError(int aBitNum,int aError);

   // Set the abort flag and signal the event semaphore.
   void abort();

   // Set the suspend flag and reset the resume semaphore.
   void suspend();

   // Clear the suspend flag and signal the resume semaphore.
   void resume();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


