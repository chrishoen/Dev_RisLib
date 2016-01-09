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
// periodically. It is passed a function pointer and a timer period.


// Call pointer for function to be called periodically
typedef CallPointer1<int> TimerCall;

class  Timer
{
public:
   Timer();
  ~Timer();

   // Executes a timer call periodically, in milliseconds
   void setPeriodic (TimerCall aTimerCall,int aTimerPeriod);

   // Executes a timer call once, in milliseconds
   void setOneShot  (TimerCall aTimerCall,int aTimerDuration);

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

