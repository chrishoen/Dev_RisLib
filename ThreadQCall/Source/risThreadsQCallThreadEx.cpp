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
   mTerminateFlag = false;
   mCallQueSize=100;

   // Timer
   mTimerExecuteFlag = false;
   mTimerPeriod = 1000;
   mCurrentTimeCount = 0;
   mTimerCurrentTimeCount = 0;

   mThreadPriority = get_default_qcall_thread_priority();
}

BaseQCallThreadEx::~BaseQCallThreadEx()
{
}

//******************************************************************************

void BaseQCallThreadEx::threadResourceInitFunction()
{
   // Initialize the call queue
   mCallQueue.initialize(mCallQueSize,128);
}

//******************************************************************************

void BaseQCallThreadEx::threadTimerInitFunction()
{
   // Guard
   if (mTimerPeriod == 0) return;

   // Bind timer callback
   mThreadTimerCall.bind (this,&BaseQCallThreadEx::threadExecuteOnTimer);

   // Start timer
   mThreadTimer.startTimer(mThreadTimerCall,mTimerPeriod);
}

//******************************************************************************

void BaseQCallThreadEx::threadExecuteOnTimer(int aCurrentTimeCount)
{
   // Update timer variables
   mTimerCurrentTimeCount++;
   mTimerExecuteFlag=true;

   // Use central semaphore to wake up the thread
   mCentralSem.put();
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
   Prn::print(Prn::QCallRun1, "BaseQCallThreadEx::threadRunFunction");
   while (true)
   {
      //----------------------------------------------------------
      //----------------------------------------------------------
      //----------------------------------------------------------
      // Wait for the central thread semaphore

      mCentralSem.get();

      //----------------------------------------------------------
      //----------------------------------------------------------
      //----------------------------------------------------------
      // Test for terminate

      if(mTerminateFlag) return;

      //----------------------------------------------------------
      //----------------------------------------------------------
      //----------------------------------------------------------
      // Test for timer. 
      // When the timer service routine is called, it sets
      // TimerExecuteFlag true, increments mCurrentTimeCount, and
      // puts to mCentralSem. 

      if(mTimerExecuteFlag)
      {
         mTimerExecuteFlag=false;

         // Copy time count from timer.
         mCurrentTimeCount = mTimerCurrentTimeCount;

         // Execute inheritor timer function.
         executeOnTimer(mCurrentTimeCount);
      }

      //----------------------------------------------------------
      //----------------------------------------------------------
      //----------------------------------------------------------
      // Not timer, was from the thread call queue.

      else
      {
         //----------------------------------------------------------
         //----------------------------------------------------------
         //----------------------------------------------------------
         // Get QCall from queue

         int tIndex;
         BaseQCall* tQCall = (BaseQCall*)mCallQueue.startRead(&tIndex);

         //----------------------------------------------------------
         //----------------------------------------------------------
         //----------------------------------------------------------
         // Execute QCall

         // If there is a QCall available
         if (tQCall)
         {
            // Execute QCall
            tQCall->execute();
            // Release it
            mCallQueue.finishRead(tIndex);
         }
      }
   }
}

//******************************************************************************
// Thread init function, base class overload.

void BaseQCallThreadEx::threadResourceExitFunction()
{
   Prn::print(Prn::QCallInit1, "BaseQCallThreadEx::threadResourceExitFunction");

   // Finalize the call queue
   mCallQueue.finalize();
}

//******************************************************************************
// shutdown

void BaseQCallThreadEx::shutdownThread()
{
   Prn::print(Prn::QCallInit1, "BaseQCallThreadEx::shutdownThread");

   // Set termination flag
   mTerminateFlag=true;
   // Post to the call sem to wake up thread if blocked on it
   mCentralSem.put();
   // Wait for thread terminate
   waitForThreadTerminate();
}

//******************************************************************************

void BaseQCallThreadEx::postQCallAvailable()
{
   mCentralSem.put();
}

}//namespace
}//namespace
