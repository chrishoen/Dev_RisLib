/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <windows.h> 

#include "prnPrint.h"

#include "risThreadsTimerEx.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a c-function that is passed to the windows timer service to
// execute periodically. It is passed a pointer to a Timer object.
// It increments the current time count and calls the user timer call.

VOID CALLBACK Timer_TimerProc(
   UINT uTimerID,
   UINT uMsg,
   DWORD_PTR dwUser,
   DWORD_PTR dw1,
   DWORD_PTR dw2)
{
   // Convert pointer
   ThreadTimerEx* tTimer = (ThreadTimerEx*)dwUser;

   // Increment current time
   tTimer->mCurrentTimeCount++;

   // Invoke user timer call
   tTimer->mTimerCall(tTimer->mCurrentTimeCount);
}


VOID CALLBACK WaitOrTimerCallback_Proc(
  __in  PVOID lpParameter,
  __in  BOOLEAN TimerOrWaitFired
)
{
   // Convert pointer
   ThreadTimerEx* tTimer = (ThreadTimerEx*)lpParameter;

   // Increment current time
   tTimer->mCurrentTimeCount++;

   // Invoke user timer call
   tTimer->mTimerCall(tTimer->mCurrentTimeCount);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// class definition for implementation specific

class ThreadTimerEx::Specific
{
public:
   Specific()
   {
      mTimerId=0;
   }
   int mTimerId;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

ThreadTimerEx::ThreadTimerEx()
{
   // Initialize members
   mTimerPeriod=1000;
   mCurrentTimeCount=0;

   // Create new specific implementation
   mSpecific = new Specific;
}

ThreadTimerEx::~ThreadTimerEx()
{
   cancel();
   delete mSpecific;
}

//******************************************************************************

void ThreadTimerEx::startTimer (TimerCall aTimerCall,int aTimerPeriod)
{
   mTimerCall    = aTimerCall;
   mTimerPeriod  = aTimerPeriod;

#if 0
   // Create a windows timer for periodic
   mSpecific->mTimerId = timeSetEvent(
      mTimerPeriod,
      0,
      Timer_TimerProc,
     (DWORD_PTR)this,
      TIME_PERIODIC);
#endif

   HANDLE tHandle = 0;
   ULONG  tFlags  = 0;

   tFlags = WT_EXECUTEINTIMERTHREAD;
// tFlags = WT_EXECUTEINPERSISTENTTHREAD;

// tHandle = CreateTimerQueue();
 

   CreateTimerQueueTimer(
      &tHandle,
      0,
      WaitOrTimerCallback_Proc,
      (void*)this,
      (DWORD)mTimerPeriod,
      (DWORD)mTimerPeriod,
      tFlags);
}

//******************************************************************************

void ThreadTimerEx::cancel()
{
   if (mSpecific->mTimerId != 0)
   {
      timeKillEvent(mSpecific->mTimerId);
   }
}

}//namespace
}//namespace
