#ifndef _RISTHREADSTWOTHREAD_H_
#define _RISTHREADSTWOTHREAD_H_

/*==============================================================================

 
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <atomic>
#include "risLogic.h"
#include "risThreadsQCallThread.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread that executes short term qcalls. That is, execution of
// thread qcalls is expected to take a short amount of time.

class TwoThreadShortThread : public Ris::Threads::BaseQCallThread
{
public:
   TwoThreadShortThread();
  ~TwoThreadShortThread();

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Thread base class overloads, these call the call pointers listed below, 
   // if they have been bound. The timer function also manages timer completion
   // notification logic.

   void threadInitFunction(); 
   void threadExitFunction(); 
   void threadExceptionFunction(char* aStr);
   void executeOnTimer(int aTimerCount);

   //---------------------------------------------------------------------------
   // Associated thread call pointers, these are called by the
   // above thread base overloads, if they are bound. They are
   // bound to functions by the instantiator before the thread
   // is launched. Any that are not bound result in a no op
   // for the thread run function

   CallPointer0<>       mThreadInitCallPointer;
   CallPointer0<>       mThreadExitCallPointer;
   CallPointer1<char*>  mThreadExceptionCallPointer;
   CallPointer1<int>    mThreadExecuteOnTimerCallPointer;

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Timer completion notification.
   // The above timer execution method posts to this semaphore,
   // if the down counter is not zero and then decrements to zero.
   BinarySemaphore  mTimerCompletionSem;
   std::atomic<int> mTimerCompletionDownCounter;

   // The following method returns a status code
   enum
   {
      TimerCompletion_None     = 0,
      TimerCompletion_Timeout  = 1,
      TimerCompletion_Aborted  = 2,
      TimerCompletion_Forced   = 3,
   };

   // This waits for timer completions
   int threadWaitForTimerCompletion(
      int aTimerCount); 

   // Timer completion code, set by above method
   int mTimerCompletionCode;

   // This aborts waits for timer completions
   void threadAbortTimerCompletion();

   // This forces a timer completion
   void threadForceTimerCompletion();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a thread that executes long term qcalls. That is, execution of
// thread qcalls is expected to take a long amount of time.

class TwoThreadLongThread : public Ris::Threads::BaseQCallThread
{
public:
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Two thread, has a short term thread and a long term thread.

class BaseTwoThread
{
public:

   //--------------------------------------------------------------
   // Constructor

   BaseTwoThread();
   virtual ~BaseTwoThread();

   //--------------------------------------------------------------
   // Thread methods

   void launchThreads(); 
   void shutdownThreads();

   //--------------------------------------------------------------
   // Thread methods, overloaded by inheritor
   // These execute in the context of the short thread

   virtual void threadInitFunction(){}
   virtual void threadExitFunction(){}
   virtual void threadExceptionFunction(char* aStr){}
   virtual void executeOnTimer(int aTimerCount){}

   //--------------------------------------------------------------
   // Thread members

   // Threads 
   TwoThreadShortThread*  mShortThread;
   TwoThreadLongThread*   mLongThread;

   //--------------------------------------------------------------
   // Exception members

   // If this is nonzero, then it is used to throw an
   // exception when a timer completion abort is caused.
   int mTimerCompletionAbortException;

   // If this is nonzero, then it is used to throw an
   // exception when a timer completion timeout happens.
   int mTimerCompletionTimeoutException;

   //--------------------------------------------------------------
   // Wait for timer

   // Long thread wait for timer.
   // This waits for a timer completion.
   // If the wait is aborted then it can throw an exception.
   // It returns a TimerCompletion code.
   int waitForTimer(int aTimeout = -1);

   //--------------------------------------------------------------
   // Long thread wait for a notification from short thread.
   // This waits for a timer completion.
   // If the wait times out  then it can throw an exception.
   // If the wait is aborted then it can throw an exception.
   // It returns a TimerCompletion code.
   int waitForNotify(int aTimeout = -1);

   //--------------------------------------------------------------
   // Multiple notifications

   // Long thread wait for a notification from short thread.
   // One notification in the list.
   void waitForNotify(int aTimeout, int aIndex);

   // Long thread wait for a notification from short thread.
   // Any notifications in the list.
   void waitForNotifyAny(int aTimeout, int aNumArgs, ...);

   // Long thread wait for a notification from short thread.
   // All notifications in the list.
   void waitForNotifyAll(int aTimeout, int aNumArgs, ...);

   // Notification latch
   Ris::Logic::AndOrLatch mNotifyLatch;

   void resetNotify(){ mNotifyLatch.reset(); }

   // This is used by the above waits.
   // If true  then waiting for any notification.
   // If false then waiting for all notifications.
   bool mWaitingForNotifyAny;

   //--------------------------------------------------------------
   // Short thread notifies long thread

   // Short thread notifies long thread with a latch index
   // This forces a timer completion, waking up the above waits.
   void notify(int aIndex);

   // Abort long thread from waiting for notification from short thread.
   // This aborts the wait for a timer completion.
   void abortWaitForNotify();

};

//******************************************************************************
}//namespace
}//namespace

#endif

