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

   VOID CALLBACK TimerAPCProc(
      LPVOID lpArg,               // Data value
      DWORD dwTimerLowValue,      // Timer low value
      DWORD dwTimerHighValue )    // Timer high value

   {
      // Formal parameters not used in this example.
      UNREFERENCED_PARAMETER(dwTimerLowValue);
      UNREFERENCED_PARAMETER(dwTimerHighValue);

      // Convert pointer
      ApcTimer* tTimer = (ApcTimer*)lpArg;

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
         mHandle=0;
      }
      HANDLE mHandle;
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
      delete mSpecific;
   }

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************

   void ApcTimer::create (int aTimerPeriod, TimerCall aTimerCall)
   {
      //---------------------------------------------------------------------------
      // Variables

      mTimerCall    = aTimerCall;
      mTimerPeriod  = aTimerPeriod;

      mSpecific->mHandle = NULL;

      LARGE_INTEGER liDueTime;
      liDueTime.QuadPart = -100000000LL;

      BOOL  bSuccess;

      //---------------------------------------------------------------------------
      // Create an unnamed waitable timer.

      mSpecific->mHandle = CreateWaitableTimer(NULL, FALSE, NULL);

      if (mSpecific->mHandle == NULL)
      {
         printf("CreateWaitableTimer failed (%d)\n", GetLastError());
         return;
      }

      // Set the timer to execute the apc procedure periodically, with the
      // timer call as an argument 

      bSuccess = SetWaitableTimer(
         mSpecific->mHandle,   // Handle to the timer object
         &liDueTime,           // When timer will become signaled
         mTimerPeriod,         // Periodic timer interval
         TimerAPCProc,         // Completion routine
         &mTimerCall,          // Argument to the completion routine
         FALSE );              // Do not restore a suspended system

      if (!bSuccess)
         {
         printf("SetWaitableTimer failed (%d)\n", GetLastError());
         return;
      }
   }

//******************************************************************************

void ApcTimer::cancel()
{
   if (mSpecific->mHandle != 0)
   {
      CancelWaitableTimer(mSpecific->mHandle);
      CloseHandle(mSpecific->mHandle);
      mSpecific->mHandle = 0;
   }
}

}//namespace
}//namespace

