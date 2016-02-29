#ifndef _RISTHREADSTWOTHREAD_H_
#define _RISTHREADSTWOTHREAD_H_

/*==============================================================================

 
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risLogic.h"
#include "risThreadsQCallThread.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class TwoThreadShortThread : public Ris::Threads::BaseQCallThread
{
public:
   TwoThreadShortThread();
  ~TwoThreadShortThread();

   // This is executed by the timer. It updates timer variables
   // and signals the central semaphore to wake up the thread.
   // aCurrentTimeCount gives the number of timer events that
   // have occurred since thread launch.
   void threadExecuteOnTimer(int aCurrentTimeCount);

   // This is posted to by the above call at the 
   // completion of timer execution, if the down counter
   // is not zero and then decrements to zero;

   // Timer completion notification.
   // The above threadExecuteOnTimer call posts to this semaphore,
   // if the down counter is not zero and then decrements to zero.
   BinarySemaphore mTimerCompletionSem;
   int             mTimerCompletionDownCounter;

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

class TwoThreadLongThread : public Ris::Threads::BaseQCallThread
{
public:
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Two thread, has a short term thread and a long term thread

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
   void executeOnTimer(int aTimerCount);

   // Thread methods, overloaded by inheritor
   virtual void threadInitFunction();
   virtual void threadExitFunction();
   virtual void threadExceptionFunction(char* aStr);

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
   // Single notifications

   // Long thread wait for a notification from short thread.
   // This waits for a timer completion.
   // If the wait times out  then it can throw an exception.
   // If the wait is aborted then it can throw an exception.
   // It returns a TimerCompletion code.
   int waitForNotify(int aTimeout = -1);

   // Abort long thread from waiting for notification from short thread.
   // This aborts the wait for a timer completion.
   void abortWaitForNotify();

   // Short thread notifies long thread.
   // This forces a timer completion, waking up the above waits.
   void notify();

   //--------------------------------------------------------------
   // Multiple notifications

   // Notification latch
   Ris::Logic::AndOrLatch mNotifyLatch;

   // Long thread wait for a notification from short thread.
   // Any notifications in the list.
   void waitForNotifyAny(int aTimeout = -1);

   // Long thread wait for a notification from short thread.
   // All notifications in the list.
   void waitForNotifyAll(int aTimeout = -1);

   // Long thread wait for a notification from short thread.
   // Any notifications in the list.
   void waitForNotifyAny(int aTimeout, int aNumArgs, ...);

   // Long thread wait for a notification from short thread.
   // All notifications in the list.
   void waitForNotifyAll(int aTimeout, int aNumArgs, ...);

   // This is used by the above waits.
   // If true  then waiting for any notification.
   // If false then waiting for all notifications.
   bool mWaitingForNotifyAny;

   // Short thread notifies long thread with a latch index
   // This forces a timer completion, waking up the above waits.
   void notify(int aIndex);
};
//******************************************************************************
}//namespace
}//namespace

#endif
