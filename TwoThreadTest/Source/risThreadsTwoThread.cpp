/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdarg.h>
#include "prnPrint.h"

#include "risThreadsTwoThread.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

TwoThreadShortThread::TwoThreadShortThread()
{
   // Timer
   mTimerPeriod = 1000;
   mTimerCompletionDownCounter = 0;
   mTimerCompletionCode = 0;
}

TwoThreadShortThread::~TwoThreadShortThread()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TwoThreadShortThread::threadExecuteOnTimer(int aCurrentTimeCount)
{
   //---------------------------------------------------------------------------
   // Post to the timer completion semaphore,
   // If the down counter counts down to zero
   if (mTimerCompletionDownCounter != 0)
   {
      if (--mTimerCompletionDownCounter == 0)
      {
         mTimerCompletionSem.put();
      }
   }

   //---------------------------------------------------------------------------
   // Update timer variables
   mTimerCurrentTimeCount++;
   mTimerExecuteFlag=true;

   // Use central semaphore to wake up the thread
   mCentralSem.put();
}

//******************************************************************************

int TwoThreadShortThread::threadWaitForTimerCompletion(
      int aTimerCount) 
{
   // Guard
   if (aTimerCount==0) return 0;
   if (aTimerCount==0) return mTimerCompletionCode;

   // Initialize completion code
   mTimerCompletionCode = TimerCompletion_Timeout;
   // Reset timer completion
   mTimerCompletionSem.reset();
   // Set the down counter, -1 means infinite timeout
   mTimerCompletionDownCounter = aTimerCount != -1 ? aTimerCount : 0;
   
   // Wait for timer completion
   mTimerCompletionSem.get();

   // Return completion code
   return mTimerCompletionCode;
}

//******************************************************************************

void TwoThreadShortThread::threadAbortTimerCompletion()
{
   // Set completion code
   mTimerCompletionCode = TimerCompletion_Aborted;
   // Clear down counter
   mTimerCompletionDownCounter = 0;

   // Post to timer completion
   // This wakes up the above wait
   mTimerCompletionSem.put();
}

//******************************************************************************

void TwoThreadShortThread::threadForceTimerCompletion()
{
   // Set completion code
   mTimerCompletionCode = TimerCompletion_Forced;
   // Clear down counter
   mTimerCompletionDownCounter = 0;

   // Post to timer completion
   // This wakes up the above wait
   mTimerCompletionSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
BaseTwoThread::BaseTwoThread()
{
   // Create thread objects
   mShortThread = new TwoThreadShortThread;
   mLongThread  = new TwoThreadLongThread;

   mShortThread->mThreadPriority = get_default_short_thread_priority();
   mLongThread->mThreadPriority = get_default_long_thread_priority();


   // Default exception codes
   mTimerCompletionAbortException   = 666;
   mTimerCompletionTimeoutException = 667;
}

BaseTwoThread::~BaseTwoThread()
{
   delete mShortThread;
   delete mLongThread;
}

//******************************************************************************
// Launch

void BaseTwoThread::launchThreads()
{
   mLongThread->launchThread();
   mShortThread->launchThread();
}

//******************************************************************************
// Shutdown

void BaseTwoThread::shutdownThreads() 
{
   // Abort any pending waits
   mShortThread->threadAbortTimerCompletion();
   // Shutdown the threads
   mShortThread->shutdownThread();
   mLongThread->shutdownThread();
}

//******************************************************************************
// Abort by aborting a timer completion.

void BaseTwoThread::abortWaitForNotify()
{
   mShortThread->threadAbortTimerCompletion();
}

//******************************************************************************
// Wait for timer.
// This is abortable.

int BaseTwoThread::waitForTimer(int aTimerCount)
{
   // Wait for timer completion
   int tTimerCompletionCode = mShortThread->threadWaitForTimerCompletion(aTimerCount);

   // Process timer completion
   switch (tTimerCompletionCode)
   {
      case TwoThreadShortThread::TimerCompletion_Aborted :
         // Throw exception for abort
         if (mTimerCompletionAbortException != 0)
         {
            throw mTimerCompletionAbortException;
         }
         break;
   }

   // Return timer completion code
   return tTimerCompletionCode;
}

//******************************************************************************
// Wait for a single notification.
// This is abortable and has a timeout.

int BaseTwoThread::waitForNotify(int aTimeout)
{
   // Wait for timer completion
   int tTimerCompletionCode = mShortThread->threadWaitForTimerCompletion(aTimeout);

   // Process timer completion
   switch (tTimerCompletionCode)
   {
      case TwoThreadShortThread::TimerCompletion_Aborted :
         // Throw exception for abort
         if (mTimerCompletionAbortException != 0)
         {
            throw mTimerCompletionAbortException;
         }
         break;
      case TwoThreadShortThread::TimerCompletion_Timeout :
         // Throw exception for abort
         if (mTimerCompletionTimeoutException != 0)
         {
            throw mTimerCompletionTimeoutException;
         }
         break;
   }

   // Return timer completion code
   return tTimerCompletionCode;
}

//******************************************************************************
// Single notification by forcing a timer completion.

void BaseTwoThread::notify()
{
   // Notify by forcing a timer completion
   mShortThread->threadForceTimerCompletion();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a multiple notification.
// This is abortable and has a timeout.
// Uses the notification latch.

void BaseTwoThread::waitForNotifyAny(int aTimeout)
{
   // Set flag to wait for any notification
   mWaitingForNotifyAny = true;
   // Wait for notication
   waitForNotify(aTimeout);
}

//******************************************************************************
// Wait for a multiple notification.
// This is abortable and has a timeout.
// Uses the notification latch.

void BaseTwoThread::waitForNotifyAll(int aTimeout)
{
   // Set flag to wait for all notifications
   mWaitingForNotifyAny = false;
   // Wait for notication
   waitForNotify(aTimeout);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a multiple notification.
// This is abortable and has a timeout.
// Uses the notification latch.

void BaseTwoThread::waitForNotifyAny(int aTimeout, int aNumArgs, ...)
{
   // Reset the notification latch
   mNotifyLatch.reset();

   // Set the notification latch mask from variable arguments
   va_list valist;
   va_start(valist,aNumArgs);
   for (int i=0;i<aNumArgs;i++)
   {
      int tIndex = va_arg(valist,int);
      mNotifyLatch.setMask(tIndex,true);
   }
   va_end(valist);

   // Wait for notification
   waitForNotifyAny(aTimeout);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a multiple notification.
// This is abortable and has a timeout
// Uses the notification latch.

void BaseTwoThread::waitForNotifyAll(int aTimeout, int aNumArgs, ...)
{
   // Reset the notification latch
   mNotifyLatch.reset();

   // Set the notification latch mask from variable arguments
   va_list valist;
   va_start(valist,aNumArgs);
   for (int i=0;i<aNumArgs;i++)
   {
      int tIndex = va_arg(valist,int);
      mNotifyLatch.setMask(tIndex,true);
   }
   va_end(valist);

   // Wait for notification
   waitForNotifyAll(aTimeout);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Multiple notification by forcing a timer completion.
// Uses the notification latch.

void BaseTwoThread::notify(int aIndex)
{
   // Set notification latch element
   mNotifyLatch.setLatch(aIndex,true);

   // Temp
   bool tNotify=false;

   // If waiting for any
   if (mWaitingForNotifyAny)
   {
      // If any notification in the latch
      if (mNotifyLatch.isAny()) tNotify=true;
   }
   // If waiting for any
   else
   {
      // If all notifications are in the latch
      if (mNotifyLatch.isAll()) tNotify=true;
   }

   // Notify
   if (tNotify)
   {
      mShortThread->threadForceTimerCompletion();
   }
}
}//namespace
}//namespace
