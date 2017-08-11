//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 

#include "prnPrint.h"

#include "risThreadsTimer.h"

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
   ThreadTimer* tTimer = (ThreadTimer*)dwUser;

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
   ThreadTimer* tTimer = (ThreadTimer*)lpParameter;

   // Increment current time
   tTimer->mCurrentTimeCount++;

   // Invoke user timer call
   tTimer->mTimerCall(tTimer->mCurrentTimeCount);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// class definition for implementation specific

class ThreadTimer::Specific
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

ThreadTimer::ThreadTimer()
{
   // Initialize members
   mTimerPeriod=1000;
   mCurrentTimeCount=0;

   // Create new specific implementation
   mSpecific = new Specific;
}

ThreadTimer::~ThreadTimer()
{
   cancel();
   delete mSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ThreadTimer::startTimer (TimerCall aTimerCall,int aTimerPeriod)
{
   mTimerCall    = aTimerCall;
   mTimerPeriod  = aTimerPeriod;

   // Create a windows timer for periodic
   mSpecific->mTimerId = timeSetEvent(
      mTimerPeriod,
      0,
      Timer_TimerProc,
     (DWORD_PTR)this,
      TIME_PERIODIC);

   HANDLE tHandle = 0;
   ULONG  tFlags  = 0;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void ThreadTimer::cancel()
{
   if (mSpecific->mTimerId != 0)
   {
      timeKillEvent(mSpecific->mTimerId);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

