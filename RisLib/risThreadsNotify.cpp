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
   resetVars();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Reset the bit mask.

void Notify::resetVars()
{
   mEnableExceptions = true;
   mNotifyFlag = false;
   mAbortFlag = false;
   mTimeoutFlag = false;
}

// Restart.
void Notify::restart(bool aEnableExceptions)
{
   resetVars();
   mEnableExceptions = aEnableExceptions;
   mSem.reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the event flag and signal the event semaphore.

void Notify::notify()
{
   // Set the flag.
   mNotifyFlag = true;

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
// If exceptions are enabled then throw an abort if the abort flag is set.
// Else return the abort flag.

bool Notify::testForAbort()
{
   if (mEnableExceptions)
   {
      if (mAbortFlag)
      {
         throw cAbortException;
      }
      else
      {
         return false;
      }
   }
   else
   {
      return mAbortFlag;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// If there is an abort pending then return false. Wait for a notify. 
// Return true if there was not an abort or timeout.

bool Notify::wait(int aTimeout)
{
   // Test for an abort.
   if (testForAbort()) return false;

   // Wait for the event. Set the timeout flag if the wait times out.
   if (!mSem.get(aTimeout))
   {
      // A timeout occurred.
      mTimeoutFlag = true;
      return false;
   }

   // Return true if there was not an abort or timeout.
   return !testForAbort();
}

bool Notify::waitForever()
{
   return wait(-1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

