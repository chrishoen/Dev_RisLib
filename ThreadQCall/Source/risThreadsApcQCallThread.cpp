/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h>
#include "prnPrint.h"

#include "ris_priorities.h"
#include "risThreadsApcQCallThread.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseApcQCallThread::BaseApcQCallThread()
{
   mTerminateFlag = false;
   mThreadPriority = get_default_qcall_thread_priority();
}

//******************************************************************************

void BaseApcQCallThread::threadTimerInitFunction()
{
}

//******************************************************************************

void BaseApcQCallThread::threadExecuteOnTimer(int aCurrentTimeCount)
{
   //Execute inheritor timer method
   executeOnTimer(aCurrentTimeCount);
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

void BaseApcQCallThread::threadRunFunction()
{
   // Wait forever on the alertable termination semaphore
   while (true)
   {
      mTerminateSem.get(INFINITE);
      if (mTerminateFlag) break;
   }
}

//******************************************************************************
// shutdown

void BaseApcQCallThread::shutdownThread()
{
   // Set termination flag
   mTerminateFlag = true;
   mTerminateSem.put();
   // Wait for thread terminate
   waitForThreadTerminate();
   Prn::print(Prn::QCallInit1, "BaseApcQCallThread::shutdownThread");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a c-function that is passed to the windows timer service to
// execute periodically. It is passed a pointer to a Timer object.
// It increments the current time count and calls the user timer call.

VOID CALLBACK QCall_APCProc(
   _In_ ULONG_PTR dwParam
   )
{
   BaseQCall* tQCall = (BaseQCall*)dwParam;
   tQCall->execute();
   delete tQCall;
}

//******************************************************************************

void BaseApcQCallThread::putQCallToThread(BaseQCall* aQCall)
{
// HANDLE*   hThreadPtr = (HANDLE*)(BaseThread::getHandlePtr());

   PAPCFUNC  pfnAPC  = QCall_APCProc;
   HANDLE    hThread = *(HANDLE*)BaseThread::getHandlePtr();
   ULONG_PTR dwData  = (ULONG_PTR)aQCall;

   QueueUserAPC(
      pfnAPC,
      hThread ,
      dwData);
}

}//namespace
}//namespace
