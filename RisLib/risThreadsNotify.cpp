/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"

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
   mErrorCode = 0;
   mEventFlag = false;
   mAbortFlag = false;
   mTimeoutFlag = false;
   mErrorFlag = 0;
   mSem.reset();
}

void Notify::clearFlags()
{
   mEventFlag = false;
   mAbortFlag = false;
   mTimeoutFlag = false;
   mErrorFlag = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the event flag and signal the event semaphore.

void Notify::notify()
{
   // Set the flag.
   mEventFlag = true;
   // Signal the event.
   mSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the error flag and signal the event semaphore.

void Notify::notifyError(int aError)
{
   // Store the error code.
   mErrorCode = aError;
   // Set the flag.
   mErrorFlag = true;
   // Signal the event.
   mSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the abort bit and signal the event semaphore.

void Notify::abort()
{
   // Set the flag.
   mAbortFlag = true;

   // Signal the event.
   mSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test for an exception condition. If an abort, timeout, or error
// has occurred then throw the corresponding exception.

void Notify::testException()
{
   // Test for an abort condition.
   if (mAbortFlag)
   {
      clearFlags();
      mExceptionCode = cAbortException;
      throw cAbortException;
   }

   // Test for a timeout condition.
   if (mTimeoutFlag)
   {
      clearFlags();
      mExceptionCode = cTimeoutException;
      throw cTimeoutException;
   }

   // Test for an error condition.
   if (mErrorFlag)
   {
      clearFlags();
      mExceptionCode = cErrorException;
      throw cErrorException;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a bit to be set. Test for exception conditions.

void Notify::wait(int aTimeout)
{
   // Test for exception conditions.
   testException();

   // Wait for the event. Set the timeout flag if the wait times out.
   if (!mSem.get(aTimeout))
   {
      // A timeout occurred.
      mTimeoutFlag = true;
   }

   // Test for exception conditions.
   testException();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a specified time. Ignore any bit notifications except an abort.
// Test for exception conditions.

void Notify::waitForTimer(int aTimeout)
{
   // Test for exception conditions.
   testException();

   // Reset all variables.
   reset();

   // Wait for the event. Only an abort should signal the event. 
   mSem.get(aTimeout);

   // Test for exception conditions.
   testException();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

