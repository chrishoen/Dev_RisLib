/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#pragma warning(disable:4793)

#include <windows.h> 

#include "prnPrint.h"
#include "risThreadsApcTimer.h"

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
   ApcTimer* tTimer = (ApcTimer*)dwUser;

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
   ApcTimer* tTimer = (ApcTimer*)lpParameter;

   // Increment current time
   tTimer->mCurrentTimeCount++;

   // Invoke user timer call
   tTimer->mTimerCall(tTimer->mCurrentTimeCount);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// class definition for implementation specific

class ApcTimer::Specific
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

ApcTimer::ApcTimer()
{
   // Initialize members
   mTimerPeriod=1000;
   mCurrentTimeCount=0;

   // Create new specific implementation
   mSpecific = new Specific;
}

ApcTimer::~ApcTimer()
{
   cancel();
   delete mSpecific;
}

//******************************************************************************

void ApcTimer::setPeriodic (TimerCall aTimerCall,int aTimerPeriod)
{
   mTimerCall    = aTimerCall;
   mTimerPeriod  = aTimerPeriod;

   HANDLE tHandle = 0;
   ULONG  tFlags  = 0;

   tFlags = WT_EXECUTEINTIMERTHREAD;
// tFlags = WT_EXECUTEINPERSISTENTTHREAD;

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

void ApcTimer::cancel()
{
   if (mSpecific->mTimerId != 0)
   {
      timeKillEvent(mSpecific->mTimerId);
   }
}

}//namespace
}//namespace

