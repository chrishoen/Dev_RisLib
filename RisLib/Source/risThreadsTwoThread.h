#ifndef _RISTHREADSTWOTHREAD_H_
#define _RISTHREADSTWOTHREAD_H_
/*==============================================================================
Realtime infastructure library Threads

This encapsulates a pair of qcall threads that can be used to by a controller to
send commands to a plant and wait for responses. It provides the following
classes:

1) TwoThreadLongThread.
   A long term thread that executes qcalls that take a long time to complete.
   They send commands to a plant and wait for responses.
   The waits can timeout or be aborted.

2) TwoThreadShortThread.
   A short term thread that executes qcalls that take a short time to complete.
   They receive the responses and then notify the long term thread, who is
   waiting for the responses.

3) A timer. This is a member of the short term thread.

4) BaseTwoThread.
   Controllers inherit from this to obtain the functionality of the above three.

Executions between the long term thread qcalls, the short term thread qcalls
and the short term thread timer are synchronized with a common mutex.

An example of the use of this is as follows:

1) A ControllerThread, which inherits from BaseTwoThread, has a long term
   qcall that sends commands to a PlantThread and waits for responses. The 
   PlantThread sends a response that is received by a short term qcall.
   The short term qcall notifies the long term qcall that is waiting and wakes 
   it up with the response.

2) Meanwhile, the short term thread timer execution occurs.

All of these executions are synchronized with the common mutex and this
provides a semi-serialized thread of execution.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"
#include "risThreadsQCallThread.h"
#include "risThreadsQCall.h"
#include "risThreadsCPSpecific.h"
#include "risLogic.h"

namespace Ris
{
namespace Threads
{

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

   virtual void configureThreads();
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
   CPSpecificQCallThread* mShortThread;
   BaseQCallThread*       mLongThread;

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

}//namespace
}//namespace
#endif

#if 0
/*==============================================================================
Copyright 2014 Christopher Hoen

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#endif

