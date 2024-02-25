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


   // If true then exceptions are enabled.
   int mEnableExceptions;

   // If this flag is true then a notify event occured.
   bool mNotifyFlag;

   // If this true then an abort notification occurred.
   bool mAbortFlag;

   // If this true then a timeout occurred.
   bool mTimeoutFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Binary semaphore event that the owning thread blocks on. It is signaled
   // by other threads to notify the owning thread of a notification event.
   BinarySemaphore mSem;

public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   Notify();
   void resetVars();

   // Restart.
   void restart(bool aEnableExceptions);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. These are used to initialize and wait for notifications.

   // If exceptions are enabled then throw an abort if the abort flag is set.
   // Else return the abort flag.
   bool testForAbort();

   // If there is an abort pending then return false. Wait for a notify. 
   // Return true if there was not an abort or timeout.
   bool wait(int aTimeout);
   bool waitForever();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. These are used to send notifications.

   // Set the notify flag and signal the event semaphore.
   // This wakes up a thread that is waiting for a notification.
   void notify();

   // Set the abort flag and signal the event semaphore.
   void abort();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


