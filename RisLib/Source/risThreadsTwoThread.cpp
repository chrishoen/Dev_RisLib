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
//******************************************************************************
//******************************************************************************
//******************************************************************************
BaseTwoThread::BaseTwoThread()
{
   // Create thread objects
   mShortThread = new CPSpecificQCallThread;
   mLongThread  = new BaseQCallThread;

   // Default exception codes
   mTimerCompletionAbortException   = 666;
   mTimerCompletionTimeoutException = 667;

   // Synchronize execution mutex.
   // This means that long qcalls, short qcalls, and the timer are serialized.
   mLongThread->synchronizeExecutionWith(mShortThread);
}

BaseTwoThread::~BaseTwoThread()
{
   delete mShortThread;
   delete mLongThread;
}

void BaseTwoThread::configureThreads()
{
   // Defaults
   mShortThread->configureThread();
   // Defaults
   mLongThread->configureThread();
   // Thread timer, none, there is only one timer for the thread pair 
   mLongThread->mThreadTimerCreateFlag = false;
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
// Defaults, don't do anything

void BaseTwoThread::executeOnTimer(int aTimerCount) {}
void BaseTwoThread::threadInitFunction(){}
void BaseTwoThread::threadExitFunction(){}
void BaseTwoThread::threadExceptionFunction(char* aStr){}

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
      case BaseQCallThread::TimerCompletion_Aborted :
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
      case BaseQCallThread::TimerCompletion_Aborted :
         // Throw exception for abort
         if (mTimerCompletionAbortException != 0)
         {
            throw mTimerCompletionAbortException;
         }
         break;
      case BaseQCallThread::TimerCompletion_Timeout :
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

