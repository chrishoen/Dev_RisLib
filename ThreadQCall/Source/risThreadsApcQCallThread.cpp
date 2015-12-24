/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

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
   // Logic
   mTerminateFlag=false;

   mThreadPriority = get_default_qcall_thread_priority();
}

BaseApcQCallThread::~BaseApcQCallThread()
{
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
   // Loop to process the call queue.
   // Exit the loop on a thread terminate.
   while (true)
   {
      Prn::print(Prn::QCallRun1, "BaseApcQCallThread::threadRunFunction");

      //----------------------------------------------------------
      //----------------------------------------------------------
      //----------------------------------------------------------
      // Wait for the QCall thread semaphore

      mCallSem.get();

      // Test for terminate
      if(mTerminateFlag) return;

      //----------------------------------------------------------
      //----------------------------------------------------------
	   //----------------------------------------------------------
      // Get QCall from queue
   
      BaseQCall* tQCall=0;

      // Lock the queue
      mCallMutex.lock();
   
      // Test for pending QCall
      if (mCallQue.isGet())
      {
         // Get QCall from queue
         mCallQue.get(tQCall);
      }
   
      // Unlock the queue
      mCallMutex.unlock();

      //----------------------------------------------------------
      //----------------------------------------------------------
      //----------------------------------------------------------
      // Execute QCall
   
      // If there is a QCall pending
      if (tQCall)
      {
         // Execute QCall
         tQCall->execute();
         // Delete it
         delete tQCall;
      }
   }
}

//******************************************************************************
// Thread init function, base class overload.

void BaseApcQCallThread::threadExitFunction()
{
   Prn::print(Prn::QCallInit1, "BaseApcQCallThread::threadExitFunction");

   // Empty the call queue
   while(mCallQue.isGet())
   {
      BaseQCall* tQCall;
      mCallQue.get(tQCall);
      delete tQCall;
   }
}

//******************************************************************************
// shutdown

void BaseApcQCallThread::shutdownThread()
{
   Prn::print(Prn::QCallInit1, "BaseApcQCallThread::shutdownThread");

   // Set termination flag
   mTerminateFlag=true;
   // Post to the call sem to wake up thread if blocked on it
   mCallSem.put();
   // Wait for thread terminate
   waitForThreadTerminate();
}

//******************************************************************************

void BaseApcQCallThread::putQCallToThread(BaseQCall* aQCall)
{
   mCallMutex.lock();
   // Put the QCall to the queue and signal the semaphore
   if (mCallQue.isPut())
   {
      mCallQue.put(aQCall);
      mCallSem.put();
   }
   else 
   {
      Prn::print(0,"ERROR CallQue FULL");
      delete aQCall;
   }
   mCallMutex.unlock();
}

}//namespace
}//namespace
