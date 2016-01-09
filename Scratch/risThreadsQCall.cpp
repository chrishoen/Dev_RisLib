/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#pragma warning(disable:4793)

#pragma warning(disable:4793)
#include "prnPrint.h"

#pragma unmanaged

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
   mRecursiveDepth=0;

   mTerminateFlag=false;
   mBlockingQueMode=true;

   mTerminateQCall = new TerminateQCall;

   mSleepExit = new RecursiveExit;

   // Timer
   mTimerExecuteFlag=false;
   mTimerPeriod = 1000;
   mTimerModulo = 1;
   mCurrentTimeCount=1;
   mTimerCurrentTimeCount=1;

   // Initialize QCalls
   mTerminateQCall->bind (this,&BaseQCallThread::executeTerminate);
   mThreadTimerCall.bind (this,&BaseQCallThread::threadExecuteOnTimer);
}

BaseQCallThread::~BaseQCallThread()
{
   delete mSleepExit;
}

//******************************************************************************

void BaseQCallThread::threadTimerInitFunction()
{
   Prn::print(Prn::QCallThreadInit,0, "BaseQCallThread::threadTimerInitFunction");

   // Set timer for periodic
   mThreadTimer.setPeriodic(mThreadTimerCall,mTimerPeriod);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// This provides the execution context for processing queued QCalls
// It loops to call threadRunRecursive to process the queue.

void BaseQCallThread::threadRunFunction()
{
   Prn::print(Prn::QCallThreadInit,0, "BaseQCallThread::threadRunFunction");

   // Loop to process the call queue
   bool tGoing=true;
   while(tGoing)
   {
      // Call recursive function to get QCall from queue, execute it, 
      // and delete it. Break from the loop for thread termination.

      int tStatus=threadRunRecursive();

      switch(tStatus)
      {
         case StatusNull :             tGoing=true;  break;
         case StatusCallExecuted :     tGoing=true;  break;
         case StatusTimerExecuted :    tGoing=true;  break;
         case StatusExitOccurred :     tGoing=true;  break;
         case StatusTimeoutOccurred :  tGoing=true;  break;
         case StatusAbort :            tGoing=true;  break;
         case StatusTerminate :        tGoing=false; break;
         case StatusMaxDepthReached :  tGoing=false; break;
      }
   }
}

//******************************************************************************
// This is called by threadRunFunction to process the call queue.
// It waits for the call queue semaphore, extracts a call from
// the call queue, and executes the call. The executing call 
// can call threadWaitRecursive, which then calls
// threadRunRecursive. This is why it is recursive. This function
// returns false if mTerminateFlag was true.
//
// When a QCall is written to the call queue, this wakes up, reads it from the 
// queue, and executes it. It returns true if successful, it returns false if
// mTerminateFlag was true. This function is recursive because it can be called
// by functions that are executed from the call queue. The functions can call
// this recursively when waiting for another queue call to execute. If this
// returns false then the calling function should terminate the thread.

int BaseQCallThread::threadRunRecursive(RecursiveExit* aExit)
{
   // Increment recursive depth
   if (++mRecursiveDepth>MaxRecursiveDepth)
   {
      // If max recursive depth reached, then halt program
      mRecursiveDepth = MaxRecursiveDepth;
      Ris::Threads::halt("BaseQCallThread MaxRecursiveDepth Reached");
      // Doesn't actally return, because of halt
      return StatusMaxDepthReached;
   }

   int tStatus=StatusNull;

   //----------------------------------------------------------
   // Wait for the central thread semaphore, it is signaled
   // by the timer and the call queue.

   mCentralSem.get();

   // Test for terminate
   if(mTerminateFlag)
   {
      --mRecursiveDepth;
      return StatusTerminate;
   }

   // Test for timer. 
   // When the timer service routine is called, it sets
   // TimerExecuteFlag true, increments mCurrentTimeCount, and
   // puts to mCentralSem. 
   if(mTimerExecuteFlag)
   {
      mTimerExecuteFlag=false;

      // Copy time count from timer
      mCurrentTimeCount = mTimerCurrentTimeCount;

      // Execute inheritor timer function, in the context of this thread.
      // It is called once every mTimerPeriod*mTimerModulo ms.
      bool tExecuteOnTimer=true;
      if (mTimerModulo>1) if (mCurrentTimeCount%mTimerModulo!=0) tExecuteOnTimer=false;

      if (tExecuteOnTimer)
      {
         executeOnTimer(mCurrentTimeCount);
      }

      tStatus = StatusTimerExecuted;
   }
   // Not timer, was from the thread call queue.
   else
   {
	   //----------------------------------------------------------
      // Get QCall from queue
   
      BaseQCall* tQCall=0;
   
      mCallMutex.lock();
   
      if (mCallQue.isGet())
      {
         // Get from queue
         mCallQue.get(tQCall);
   
         // Put to the limit semaphore
         if (mBlockingQueMode)
         {
            mLimitSem.put();
         }
      }
   
      mCallMutex.unlock();
      //----------------------------------------------------------
      // Execute QCall and then delete. This is where the thread's
      // main action takes place.
   
      if (tQCall)
      {
         tQCall->execute();
         delete tQCall;
      }

      tStatus = StatusCallExecuted;
   }

   //----------------------------------------------------------
   // Done, either a timer function or a QCall function was executed

   // Test for terminate
   if(mTerminateFlag)
   {
      tStatus = StatusTerminate;
   }
   else if (aExit)
   {
      // Test for exit
      if (aExit->isExitAck(mRecursiveDepth-1))
      {
         if (!aExit->isExitAbort(mRecursiveDepth-1)) tStatus = StatusExitOccurred;
         else                                        tStatus = StatusAbort; 
         aExit->reset(mRecursiveDepth-1);
      }
      // Test for timeout
      else if (aExit->isTimeout(mRecursiveDepth-1,mCurrentTimeCount))
      {
         tStatus = StatusTimeoutOccurred;
      }
   }

   // Status is okay, either StatusCallExecuted or StatusCallTimeout
   --mRecursiveDepth;
   return tStatus;
}

//******************************************************************************
// This can be called by an executing QCall to wait until another
// QCall sets aExit to acknowledge or mTerminateFlag is true.
// This can be used to have the execution of a first QCall wait
// for the execution of a second QCall, without blocking the
// queue.

bool BaseQCallThread::threadWaitRecursive(RecursiveExit* aExit,int& aStatus,int aTimeoutInMs)
{
   aStatus=StatusNull;

   // Guard
   if (mTimerPeriod==0) return false;

   //----------------------------------------------
   // Calculate timeout

   int tTimeoutInCounts = 0;

   if (aTimeoutInMs < 1)
   {
      tTimeoutInCounts = 0; 
   }
   else
   {
      if (aTimeoutInMs > mTimerPeriod)
      {
         tTimeoutInCounts = aTimeoutInMs/mTimerPeriod;
      }
      else
      {
         tTimeoutInCounts = 1;
      }
   }

   //----------------------------------------------
   // Set exit request and timeout variables

   aExit->setExitReq(mRecursiveDepth);
   aExit->initializeTimeout(mRecursiveDepth,mCurrentTimeCount,tTimeoutInCounts);
   
   //----------------------------------------------
   // Loop to call the thread recursive run function

   bool tGoing=true;
   while(tGoing)
   {
      // Call recursive function to get QCall from queue, execute it, 
      // and delete it.

      aStatus = threadRunRecursive(aExit);

      // Exit the loop, based on status
      switch(aStatus)
      {
         case StatusNull :             tGoing=true;   break;
         case StatusCallExecuted :     tGoing=true;   break;
         case StatusTimerExecuted :    tGoing=true;   break;
         case StatusExitOccurred :     tGoing=false;  break;
         case StatusTimeoutOccurred :  tGoing=false;  break;
         case StatusTerminate :        tGoing=false;  break;
         case StatusAbort :            tGoing=false;  break;
         case StatusMaxDepthReached :  tGoing=false;  break;
      }
   }

   return aStatus == StatusExitOccurred;
}

//******************************************************************************
// This can be called by an executing QCall to sleep

int BaseQCallThread::threadSleepRecursive(int aTimeoutInMs)
{
   int tStatus=0;
   return threadWaitRecursive(mSleepExit,tStatus,aTimeoutInMs);
}

//******************************************************************************
// This can be used by the second QCall to wake up the waiting
// first QCall. aExit must be a variable that is visible to
// the first and second QCalls.

void BaseQCallThread::threadNotifyRecursive(RecursiveExit* aExit)
{
   aExit->setExitAck(mRecursiveDepth-1);
}

//******************************************************************************
// This can be used by the second QCall to wake up the waiting
// first QCall, for an abort. aExit must be a variable that is visible to
// the first and second QCalls.

void BaseQCallThread::threadAbortRecursive(RecursiveExit* aExit)
{
   aExit->setExitAbort(mRecursiveDepth-1);
}

//******************************************************************************
// Thread init function, base class overload.

void BaseQCallThread::threadExitFunction()
{
   Prn::print(Prn::QCallThreadInit,0, "BaseQCallThread::threadExitFunction");

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
   Prn::print(Prn::QCallThreadInit,0, "BaseQCallThread::shutdownThread");

   mTerminateFlag=true;
   mTerminateQCall->invoke();
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
      mCentralSem.put();
   }
   else 
   {
      Prn::print(0,0, "ERROR CallQue FULL");
      delete aQCall;
   }
   mCallMutex.unlock();
}

//******************************************************************************

void BaseQCallThread::executeTerminate()
{
   mTerminateFlag=true;
}

//******************************************************************************

void BaseQCallThread::threadExecuteOnTimer(int aCurrentTimeCount)
{
   // Update timer variables
   mTimerCurrentTimeCount++;
   mTimerExecuteFlag=true;

   // Use central semaphore to wake up the thread
   mCentralSem.put();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

RecursiveRecord::RecursiveRecord()
{
   reset();
}

void RecursiveRecord::reset()
{
   mExitReqFlag      = false;
   mExitAckFlag      = false;
   mExitAbortFlag    = false;
   mTimeoutTimeCount = 0;
}

void RecursiveRecord::setExitReq()
{
   mExitReqFlag=true;
}

bool RecursiveRecord::isExitReq()
{
   return mExitReqFlag;
}

void RecursiveRecord::setExitAck()
{
   if (mExitReqFlag)
   {
      mExitReqFlag=false;
      mExitAckFlag=true;
   }
}

bool RecursiveRecord::isExitAck()
{
   return mExitAckFlag;
}

void RecursiveRecord::setExitAbort()
{
   if (mExitReqFlag)
   {
      mExitReqFlag=false;
      mExitAckFlag=true;
      mExitAbortFlag=true;
   }
}

bool RecursiveRecord::isExitAbort()
{
   return mExitAbortFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

RecursiveExit::RecursiveExit()
{
   reset();
}

void RecursiveExit::reset()
{
   for(int i=0;i<MaxRecursiveDepth;i++)
   {
      mRecord[i].reset();         
   }
}

//---------------------------------------------------------------------------
// Exit methods

void RecursiveExit::reset(int aDepth)
{
   mRecord[aDepth].reset();
}

void RecursiveExit::setExitReq(int aDepth)
{
   mRecord[aDepth].setExitReq();
}

bool RecursiveExit::isExitReq(int aDepth)
{
   return mRecord[aDepth].isExitReq();
}
void RecursiveExit::setExitAck(int aDepth)
{
   mRecord[aDepth].setExitAck();
}
bool RecursiveExit::isExitAck(int aDepth)
{
   return mRecord[aDepth].isExitAck();
}
void RecursiveExit::setExitAbort(int aDepth)
{
   mRecord[aDepth].setExitAbort();
}
bool RecursiveExit::isExitAbort(int aDepth)
{
   return mRecord[aDepth].isExitAbort();
}

//---------------------------------------------------------------------------
// Timeout methods

void RecursiveExit::initializeTimeout(int aDepth,int aCurrentTimeCount,int aTimeoutInCounts)
{
   if (aTimeoutInCounts < 1)
   {
      mRecord[aDepth].mTimeoutTimeCount = -1;
   }
   else
   {
      mRecord[aDepth].mTimeoutTimeCount = aCurrentTimeCount + aTimeoutInCounts;
   }
}

bool RecursiveExit::isTimeout(int aDepth,int aCurrentTimeCount)
{
   if (mRecord[aDepth].mTimeoutTimeCount == -1)
   {
      return false;
   }

   return aCurrentTimeCount >= mRecord[aDepth].mTimeoutTimeCount;
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

