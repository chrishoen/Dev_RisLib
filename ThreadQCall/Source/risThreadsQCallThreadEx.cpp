/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

//#pragma warning(disable:4793)

#include "prnPrint.h"

#include "ris_priorities.h"
#include "risThreadsQCallThreadEx.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseQCallThreadEx::BaseQCallThreadEx()
{
   // Logic
   mTerminateFlag=false;

   // Timer
   mTimerPeriod = 1000;
   mTimerCompletionDownCounter = 0;
   mTimerCompletionCode = 0;
   mThreadTimerCreateFlag = true;

   mThreadPriority = get_default_qcall_thread_priority();
   mTimerThreadPriority = get_default_qcall_timer_thread_priority();

   mCallQueue.initialize(CallQueSize);

}

BaseQCallThreadEx::~BaseQCallThreadEx()
{
}

//******************************************************************************

void BaseQCallThreadEx::threadTimerInitFunction()
{
   // Guard
   if (mThreadTimerCreateFlag == false) return;

   // Bind timer callback
   mThreadTimerCall.bind (this,&BaseQCallThreadEx::threadExecuteOnTimer);

   // Start timer
// mThreadTimer.startTimer(mThreadTimerCall,mTimerPeriod,mTimerThreadPriority);
   mThreadTimer.startTimer(mThreadTimerCall,mTimerPeriod);
}

//******************************************************************************

void BaseQCallThreadEx::threadExecuteOnTimer(int aCurrentTimeCount)
{
   // Guard
   if(mTerminateFlag) return;

   // Lock thread execution
   lockExecution();

   //Execute inheritor timer method
   executeOnTimer(aCurrentTimeCount);

   // Post to the timer completion semaphore,
   // If the down counter counts down to zero
   if (mTimerCompletionDownCounter != 0)
   {
      if (--mTimerCompletionDownCounter == 0)
      {
         mTimerCompletionSem.put();
      }
   }

   // Unlock thread execution
   unlockExecution();
}

//******************************************************************************

int BaseQCallThreadEx::threadWaitForTimerCompletion(
      int aTimerCount) 
{
   // Guard
   if (aTimerCount==0) return 0;
   if (aTimerCount==0) return mTimerCompletionCode;

   // Initialize completion code
   mTimerCompletionCode = TimerCompletion_Timeout;
   // Reset timer completion
   mTimerCompletionSem.reset();
   // Set the down counter, -1 means infinite timeout
   mTimerCompletionDownCounter = aTimerCount != -1 ? aTimerCount : 0;
   
   // Unlock thread execution
   unlockExecution();
   // Wait for timer completion
   mTimerCompletionSem.get();
   // Lock thread execution
   lockExecution();

   // Return completion code
   return mTimerCompletionCode;
}

//******************************************************************************

void BaseQCallThreadEx::threadAbortTimerCompletion()
{
   // Set completion code
   mTimerCompletionCode = TimerCompletion_Aborted;
   // Clear down counter
   mTimerCompletionDownCounter = 0;

   // Post to timer completion
   // This wakes up the above wait
   mTimerCompletionSem.put();
}

//******************************************************************************

void BaseQCallThreadEx::threadForceTimerCompletion()
{
   // Set completion code
   mTimerCompletionCode = TimerCompletion_Forced;
   // Clear down counter
   mTimerCompletionDownCounter = 0;

   // Post to timer completion
   // This wakes up the above wait
   mTimerCompletionSem.put();
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

void BaseQCallThreadEx::threadRunFunction()
{
   // Loop to process the call queue.
   // Exit the loop on a thread terminate.
   while (true)
   {
      Prn::print(Prn::QCallRun1, "BaseQCallThreadEx::threadRunFunction");

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
   
      BaseQCall* tQCall = (BaseQCall*)mCallQueue.readPtr();

      //----------------------------------------------------------
      //----------------------------------------------------------
      //----------------------------------------------------------
      // Execute QCall
   
      // If there is a QCall pending
      if (tQCall)
      {
         // Lock thread execution
         lockExecution();
         // Execute QCall
         tQCall->execute();
         // Delete it
         delete tQCall;
         // Unlock thread execution
         unlockExecution();
      }
   }
}

//******************************************************************************
// Thread init function, base class overload.

void BaseQCallThreadEx::threadExitFunction()
{
   Prn::print(Prn::QCallInit1, "BaseQCallThreadEx::threadExitFunction");

   // Empty the call queue

   while (true)
   {
      BaseQCall* tQCall = (BaseQCall*)mCallQueue.readPtr();
      if (tQCall)
      {
         delete tQCall;
      }
      else
      {
         break;
      }
   }
}

//******************************************************************************
// shutdown

void BaseQCallThreadEx::shutdownThread()
{
   Prn::print(Prn::QCallInit1, "BaseQCallThreadEx::shutdownThread");

   // Set termination flag
   mTerminateFlag=true;
   // Post to the call sem to wake up thread if blocked on it
   mCallSem.put();
   // Abort timer completion if thread if blocked on it
   threadAbortTimerCompletion();
   // Wait for thread terminate
   waitForThreadTerminate();
}

//******************************************************************************

void BaseQCallThreadEx::putQCallToThread(BaseQCall* aQCall)
{
   // Put the QCall to the queue and signal the semaphore
   if (mCallQueue.writePtr(aQCall))
   {
      mCallSem.put();
   }
   else 
   {
      Prn::print(0,"ERROR CallQue FULL");
      delete aQCall;
   }
}

void BaseQCallThreadEx::lockExecution()
{
      // Lock execution for this thread
      mExecutionMutex.lock();
}

void BaseQCallThreadEx::unlockExecution()
{
      // Unlock execution for this thread
      mExecutionMutex.unlock();
}


}//namespace
}//namespace
