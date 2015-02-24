#ifndef _RISTHREADSTIMER_H_
#define _RISTHREADSTIMER_H_

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risPortableTypes.h"
#include "risCallPointer.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// The Timer class provides a timer that can be used to call a function
// periodically. It is passed a function pointer, a timer period, and a
// thread priority fro the timer execution.

// Call pointer for function to be called periodically
typedef CallPointer1<int> TimerCall;

class ThreadTimer
{
public:
   ThreadTimer();
  ~ThreadTimer();

   // Executes a timer call periodically, in milliseconds,
   // with a thread priority (zero means use default)
   void startTimer(
      TimerCall aTimerCall,
      int       aTimerPeriod,
      int       aThreadPriority);

   void cancel();

   //---------------------------------------------------------------------------
   // Members

   // Pointer to function to execute periodically
   TimerCall  mTimerCall;

   // Period, in milliseconds
   int        mTimerPeriod;

   // Timer thread priority
   int        mThreadPriority;

   // Current time count, incremented on every timer event.
   // Counts the number of timer events that have occurred
   // since the timer was created.
   int        mCurrentTimeCount;

protected:
   // Specific implementation of member timer
   class Specific;
   Specific* mSpecific;
};

}//namespace
}//namespace

#endif
