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
   static const int cMaxStringSize = 40;

   // Throw codes.
   static const int cAbortException   = 666;
   static const int cTimeoutException = 667;
   static const int cErrorException   = 668;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

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
   int mError;

   // Label string.
   char mLabel[cMaxStringSize];

   // If this true then notification operations are disabled.
   bool mLock;

   // If this true then a timeout occurred.
   bool mTimeoutFlag;

   // If this true then an error occurred.
   bool mErrorFlag;

   // If this true then an abort notification occurred.
   bool mAbortFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If this true then an exception is thrown on abort, timeout or error.
   bool mThrowEnable;

   // This indicates the source of an exception.
   int mThrowCode;

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
   // Methods. These are used to receive notifications.

   // Clear all of the mask and latch bits and set a single mask bit.
   void setMaskOne(int aBitNum);

   // Clear all of the mask and latch bits and set a variable list of mask
   // bits. Set the trap condition for OR.
   void setMaskAny(int aNumArgs, ...);

   // Clear all of the mask and latch bits and set a variable list of mask
   // bits. Set the trap condition for AND.
   void setMaskAll(int aNumArgs, ...);

   // Set the mask label.
   void setLabel(const char* aLabel);

   // Enable exceptions. If this is true then an abort, timeout, or error
   // will throw an exception when a notification occurrs.
   void enableExceptions(bool aThrowEnable);

   // Wait for a bit to be set. Return false if a timeout or abort occured.
   bool wait(int aTimeout);

   // Wait for a specified time. Ignore any bit notifications except an abort.
   // Return false if an abort occured.
   bool waitForTimer(int aTimeout);

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

   // Set the abort bit and signal the event semaphore.
   void abort(int aBitNum);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


