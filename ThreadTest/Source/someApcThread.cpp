/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h>
#include "prnPrint.h"

#include "someShare.h"

#define  _SOMEAPCTHREAD_CPP_
#include "someApcThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

ApcThread::ApcThread()
{
   mTimerPeriod=1000;
   mTimerPeriod=0;
   mTerminateFlag = false;
   BaseClass::setThreadPriorityHigh();
}

//******************************************************************************

void ApcThread::threadTimerInitFunction()
{
   // Guard
   if (mTimerPeriod==0) return;

   // Call pointer
   Ris::Threads::TimerCall tTimerCall;
   tTimerCall.bind(this,&ApcThread::threadExecuteOnTimer);

   // Create timer
   mApcTimer.create(mTimerPeriod,tTimerCall);
}

//******************************************************************************

void ApcThread::threadExecuteOnTimer(int aTimeCount)
{
   // Guard
   if(mTerminateFlag) return;

   //Execute inheritor timer method
   executeOnTimer(aTimeCount);

}
//******************************************************************************
// Thread run function, base class overload.
// This provides the execution context for processing queued QCalls
// It waits for the call queue semaphore, extracts a call from
// the call queue, and executes the call.
//
// When a QCall is written to the call queue, this wakes up, reads it from the 
// queue, and executes it. It returns true if successful, it returns false if
// mTerminateFlag was true.

void ApcThread::threadRunFunction()
{
   // Wait forever on the alertable termination semaphore
   while (true)
   {
      mTerminateSem.get(INFINITE);
      if (mTerminateFlag) break;
   }
}

//******************************************************************************
// Thread exit function, base class overload.

void ApcThread::threadExitFunction()
{
   if (mTimerPeriod==0) return;
   mApcTimer.cancel();
}

//******************************************************************************
// shutdown

void ApcThread::shutdownThread()
{
   // Set termination flag
   mTerminateFlag = true;
   mTerminateSem.put();
   // Wait for thread terminate
   waitForThreadTerminate();
   Prn::print(Prn::QCallInit1, "ApcThread::shutdownThread");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This executes the qcall that was passed via APC by the following function.

static VOID CALLBACK Some_APCProc(_In_ ULONG_PTR dwParam)
{
   ApcThread* tThread = (ApcThread*)dwParam;
   tThread->executeOnApc();
}

//******************************************************************************
// This sends an APC to this thread, it contains a pointer to the above 
// function and a pointer to a qcall.

// This enqueues an APC to the threads APC queue

void ApcThread::enqueueApc()
{
   QueueUserAPC(
      Some_APCProc,
      *(HANDLE*)BaseClass::getHandlePtr(),
      (ULONG_PTR)this);
}

//******************************************************************************

void ApcThread::executeOnApc()
{
   gShare.mTimeMarker.doStop();
}

}//namespace
