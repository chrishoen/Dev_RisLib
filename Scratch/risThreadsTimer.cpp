/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#pragma warning(disable:4793)

#ifdef _AFXDLL
// MFC project
#define _WIN32_WINNT 0x0500
#include <afxwin.h>
#include <mmsystem.h>
#else
#define _WIN32_WINNT 0x0500
// NOT MFC project
#include <windows.h> 
#endif

#pragma warning(disable:4793)
#include "prnPrint.h"

#pragma unmanaged

#include "risTimeMarker.h"
#include "risThreadsTimer.h"

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

Ris::PeriodicTimeMarker rTimeMarker1;
Ris::PeriodicTimeMarker rTimeMarker2;
Ris::PeriodicTimeMarker rTimeMarker3;

VOID CALLBACK Timer_TimerProc(
   UINT uTimerID,
   UINT uMsg,
   DWORD_PTR dwUser,
   DWORD_PTR dw1,
   DWORD_PTR dw2)
{
   // Convert pointer
   Timer* tTimer = (Timer*)dwUser;

   // Increment current time
   tTimer->mCurrentTimeCount++;

   // Invoke user timer call
   tTimer->mTimerCall(tTimer->mCurrentTimeCount);
#if 0
   rTimeMarker1.doEnd();
   rTimeMarker2=rTimeMarker1;
   rTimeMarker1.doBegin();

   double tTestPeriodUsec = double(tTimer->mTimerPeriod*1000.0);
   double tTimerError =  tTestPeriodUsec - rTimeMarker2.elapsedTimeInMicroseconds();
   Prn::print(0, "Timer_TimerProc %d %8.2f",tTimer->mCurrentTimeCount,tTimerError);
#endif
}


VOID CALLBACK WaitOrTimerCallback_Proc(
  __in  PVOID lpParameter,
  __in  BOOLEAN TimerOrWaitFired
)
{
   // Convert pointer
   Timer* tTimer = (Timer*)lpParameter;

   // Increment current time
   tTimer->mCurrentTimeCount++;

   // Invoke user timer call
   tTimer->mTimerCall(tTimer->mCurrentTimeCount);
#if 0
   rTimeMarker1.doEnd();
   rTimeMarker2=rTimeMarker1;
   rTimeMarker1.doBegin();

   double tTestPeriodUsec = double(tTimer->mTimerPeriod*1000.0);
   double tTimerError =  tTestPeriodUsec - rTimeMarker2.elapsedTimeInMicroseconds();
   Prn::print(0, "Timer_TimerProc %d %8.2f",tTimer->mCurrentTimeCount,tTimerError);
#endif
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// class definition for implementation specific

class Timer::Specific
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

Timer::Timer()
{
   // Initialize members
   mTimerPeriod=1000;
   mCurrentTimeCount=0;

   // Create new specific implementation
   mSpecific = new Specific;
}

Timer::~Timer()
{
   cancel();
   delete mSpecific;
}

//******************************************************************************

void Timer::setPeriodic (TimerCall aTimerCall,int aTimerPeriod)
{
   mTimerCall    = aTimerCall;
   mTimerPeriod  = aTimerPeriod;

   // Set process windows timer resolution to one millisecond
   timeBeginPeriod(1);

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

void Timer::setOneShot (TimerCall aTimerCall,int aTimerPeriod)
{
}

//******************************************************************************

void Timer::cancel()
{
   if (mSpecific->mTimerId != 0)
   {
      timeKillEvent(mSpecific->mTimerId);
   }
}

}//namespace
}//namespace

