#ifndef _RISTHREADSAPCTIMER_H_
#define _RISTHREADSAPCTIMER_H_

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risCallPointer.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// The Timer class provides a timer that can be used to call a function
// periodically. It is passed a function pointer and a timer period. This uses
// waitable timers and asynchronous procedure calls. These only work with
// thread run functions that are alertable (SleepEx).

// Call pointer for function to be called periodically
typedef CallPointer1<int> TimerCall;

class ApcTimer
{
public:
   ApcTimer();
  ~ApcTimer();

   // Creates a timer call that is called periodically. It is passed a period
   // and a call pointer that contains the address of a method to call.

   void create (int aTimerPeriod, TimerCall aTimerCall);

   void cancel();

   //---------------------------------------------------------------------------
   // members

   // Pointer to function to execute periodically
   TimerCall  mTimerCall;

   // Period, in milliseconds
   int        mTimerPeriod;

   // Current time count, incremented on every timer event.
   // Counts the number of timer events that have occurred
   // since the timer was created.
   int        mCurrentTimeCount;

protected:
   // Platform specific implementation
   class Specific;
   Specific* mSpecific;
};

}//namespace
}//namespace

#endif

