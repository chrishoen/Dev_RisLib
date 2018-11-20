#pragma once

/*==============================================================================
Thread waitable timer and events.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// The class provides a mechanism that allows a thread to wait on a timer
// and on a counting semaphore.


class Waitable
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Current time count, incremented on every timer event.
   // Counts the number of timer events that have occurred
   // since the timer was created.
   int  mTimerCount;

   // Return true if the previous wait unblocking was a result of the timer 
   // or the counting semaphore.
   bool mWasTimerFlag;
   bool mWasSemaphoreFlag;

   // Return true if the object is valid.
   bool mValidFlag;

protected:
   // Pimpl pattern. Used to hide details of the operating system specific
   // variables, like the timer handle, from the .h file so that this
   // include file can be complied by different compliers. The class is
   // defined in the .cpp file, where there is a different version for
   // different compilers.
   class Specific;
   Specific* mSpecific;

public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   Waitable();
  ~Waitable();

   // Initialize the timer and the counting semaphore. Start the timer
   // periodically, in milliseconds.
   void initialize(int aTimerPeriod);

   // Cancel the timer.
   void finalize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Post to the counting semaphore.
   void postSemaphore();

   // Wait for the timer or counting semaphore.
   void waitForTimerOrSemaphore();

   // Return true if the previous wait unblocking was a result of the timer 
   // or the counting semaphore.
   bool wasTimer();
   bool wasSemaphore();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


