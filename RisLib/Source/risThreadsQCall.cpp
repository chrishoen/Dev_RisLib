/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

//#pragma warning(disable:4793)

#include "prnPrint.h"

#include "risThreadsQCall.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseQCallThread::BaseQCallThread() :
   mLimitSem(CallQueSize-10)
{
   // Logic

   mTerminateFlag=false;
   mBlockingQueMode=true;

   // Timer
   mTimerPeriod = 1000;
   mTimerCompletionDownCounter = 0;
   mTimerCompletionCode = 0;
   mCreateThreadTimerFlag = true;

   // Synchronization
   mSynchronizedExecutionMutex=0;
}

BaseQCallThread::~BaseQCallThread()
{
}

//******************************************************************************

void BaseQCallThread::threadTimerInitFunction()
{
   // Guard
   if (mCreateThreadTimerFlag == false) return;

   Prn::print(Prn::ThreadInit,0, "BaseQCallThread::threadTimerInitFunction %d",mTimerPeriod);
 
   // Set timer for periodic
   mThreadTimerCall.bind (this,&BaseQCallThread::threadExecuteOnTimer);
   mThreadTimer.setPeriodic(mThreadTimerCall,mTimerPeriod);
}

//******************************************************************************

void BaseQCallThread::threadExecuteOnTimer(int aCurrentTimeCount)
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

int BaseQCallThread::threadWaitForTimerCompletion(
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

void BaseQCallThread::threadAbortTimerCompletion()
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

void BaseQCallThread::threadForceTimerCompletion()
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

void BaseQCallThread::threadRunFunction()
{
   // Loop to process the call queue.
   // Exit the loop on a thread terminate.
   while (true)
   {
      Prn::print(Prn::ThreadRun,Prn::Run3, "BaseQCallThread::threadRunFunction");

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
   
         // Put to the limit semaphore
         if (mBlockingQueMode)
         {
            mLimitSem.put();
         }
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

void BaseQCallThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit,0, "BaseQCallThread::threadExitFunction");

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

void BaseQCallThread::shutdownThread()
{
   Prn::print(Prn::ThreadInit,0, "BaseQCallThread::shutdownThread");

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

void BaseQCallThread::putQCallToThread(BaseQCall* aQCall)
{
   // Get from the limit semaphore, blocking if the queue is almost full
   if (mBlockingQueMode)
   {
      mLimitSem.get();
   }

   mCallMutex.lock();
   // Put the QCall to the queue and signal the semaphore
   if (mCallQue.isPut())
   {
      mCallQue.put(aQCall);
      mCallSem.put();
   }
   else 
   {
      Prn::print(0,0, "ERROR CallQue FULL");
      delete aQCall;
   }
   mCallMutex.unlock();
}

//******************************************************************************

void BaseQCallThread::synchronizeExecutionWith(BaseQCallThread* aOtherThread)
{
   mSynchronizedExecutionMutex = &aOtherThread->mExecutionMutex;
}

void BaseQCallThread::lockExecution()
{
   // if this thread is not synchronized to another thread
   if (mSynchronizedExecutionMutex==0)
   {
      // Lock execution for this thread
      mExecutionMutex.lock();
   }
   else
   {
      // Lock execution for the other thread
      mSynchronizedExecutionMutex->lock();
   }
}

void BaseQCallThread::unlockExecution()
{
   // if this thread is not synchronized to another thread
   if (mSynchronizedExecutionMutex==0)
   {
      // Unlock execution for this thread
      mExecutionMutex.unlock();
   }
   else
   {
      // Unlock execution for the other thread
      mSynchronizedExecutionMutex->unlock();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Compile tests
#if 0

QCall0                        gCompileTestQCall0;
QCall1<int>                   gCompileTestQCall1;
QCall2<int,int*>              gCompileTestQCall2;
QCall3<int,int*,int**>        gCompileTestQCall3;
QCall4<int,int*,int**,int***> gCompileTestQCall4;
#endif

}//namespace
}//namespace
