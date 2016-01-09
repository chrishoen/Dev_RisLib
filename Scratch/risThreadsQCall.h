#ifndef _RISTHREADSQCALL_H_
#define _RISTHREADSQCALL_H_

/*==============================================================================

QCall (Queued Function Call) 

A QCall is a function call that is invoked from a calling thread to a
called thread. The calling thread encapsulates a function call and queues
it to the called thread. The called thread dequeues the function call and
executes it in its own context.

This file contains class definitions for:
1) QCalls
2) A base class for threads that process QCalls 

class BaseQCall
class QCall0
class QCall1 <class X1>
class QCall2 <class X1,class X2>
class QCall3 <class X1,class X2,class X3>
class QCall4 <class X1,class X2,class X3,class X4>
class BaseQCallThread
class RecursiveExit

A QCall (Queued Function Call) is a thread interface mechanism that allows 
thread member functions to be called by other threads in a deferred manner
such that execution of the member functions is serialized and takes place
in the context of the called thread, not in the context of the calling thread.

A QCall is an object that contains data about the member function to execute
and argument data for the member function. A QCall encapsualtes a function
and its arguments.

Threads that can receive QCalls are based upon a single mCallQue to which QCalls
are sent by calling threads. When a QCall is sent to the mCallQue, the thread 
wakes up and processes it by executing the thread member function indicated
by the QCall.

This is similar to a message processing based thread that contains a single
message queue to which messages are sent, but it has the advantage of having
a simplified function call interface for each message.
 
QCalls are implemented with code based on CallPointers and templates.
CallPointers are templated constructs that implement C++ function pointers
that can point to class/object member functions. They act like c function
pointers, but can be used with class member functions.

QCalls contain three things:

1) An "invoke" CallPointer that is used to transfer a QCall to a specific
thread's QCallQue. It contains the address of a thread's putQCallToThread method.
2) An "execute" CallPointer that is used to execute the thread member
function (the procedure call). It contains the address of the member function.
3) The arguments to the thread member function (the arguments 
to the procedure call)

QCalls are organized around a QCall base class and class templates that manage
the different function signatures ("execute" CallPointers).

BaseQCall is the base class

QCall0 is a class template for a QCall with 0 arguments, no return
QCall1 is a class template for a QCall with 1 argument,  no return
QCall2 is a class template for a QCall with 2 arguments, no return
QCall3 is a class template for a QCall with 3 argument,  no return

The template syntax is based on that of the CallPointers.

Note that CallPointers that have returns cannot be used because there
is no way to get the return back to the caller.

QCall threads inherit from BaseQCallThread or BaseQCallThreadMultipleWait.

BaseQCallThread contains an mCallQue and a threadRunFunction that services
it. The mCallQue is a queue of QCall pointers that contains a semaphore that 
the thread waits at. When a QCall pointer is written to the queue, the thread
wakes up, gets the QCall from the queue and and calls the QCall "execute" 
CallPointer, passing in the QCall arguments.

BaseQCallThread does a WaitForSingleObject on the QCallQue semaphore. The
wait has a one second timeout that is used to poll a thread terminate flag
that is used to shutdown the thread.

BaseQCallThreadMultipleWait is similar to BaseQCallThread. It does a
WaitForMultipleObject, where one of the objects is the mCallQue semaphore.
Other objects are a timer and a termination request semaphore. Inheriting
threads can add other objects to to the multiple wait list.

QCalls are used in the following manner:

1) The called thread contains a member variable that is a QCall. Its 
   mInvokeCallPointer contains the address of the thread's putQCallToThread
   method. Its mExecuteCallPointer contains the address of the deferred 
   procedure member function.

2) The calling thread invokes the QCall by calling its "invoke" method, 
   passing in procedure arguments. The "invoke" method creates a new copy
   of the QCall on the heap, sets its arguments, and calls the QCall 
   mInvokeCallPointer. This writes the address of the QCall to the called 
   thread's mCallQue and signals the mCallQue semaphore.

3) The called thread's threadRunFunction is waiting on its mCallQue. 
   It wakes up, gets the address of the QCall from the mCallQue, and calls
   the QCall mExecuteCallPointer. This executes the thread member function
   associated with the QCall. Then it deletes the QCall from the heap.

For an example, the called thread could have a QCall and two methods such as:

   void invokeSomeFunction  (int aSessionIndex, Message* aRxMsg);
   void executeSomeFunction (int aSessionIndex, Message* aRxMsg);
   typedef Ris::Threads::QCall2<int,Message*> SomeFunctionQCall;
   SomeFunctionQCall mSomeFunctionQCall;

The calling thread invokes the QCall by:

  gCalledThread->invokeSomeFunction(1,mRxMsg);
                        or
  gCalledThread->mSomeFunctionQCall.invoke(1,mRxMsg);


The calling thread calls invokeSomeFunction in its context and the called
thread calls executeSomeFunction in its context.

Note that what happens here is that mSomeFunctionQCall contains a CallPointer
to executeSomeFunction. When it is invoked, it makes a copy of itself on the
heap (with correct arguments). The copy is passed to the mCallQue and is
executed by the thread run function and then deleted.
 
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risPortableTypes.h"
#include "risCallPointer.h"
#include "risContainers.h"
#include "risThreads.h"
#include "risThreadsTimer.h"

#pragma managed(push,off)

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// BaseQCallThread is a thread that waits on a queue of QCalls, which are
// objects that contain a pointer to a function and values for arguments that
// are to be passed to that function. This is similar to a thread that waits
// on a message queue, and wakes up and processes messages when they are sent
// to it's queue. Here the messages take the form of QCalls. 
//
// BaseQCallThread contains an mCallQue and a threadRunFunction that services
// it. The mCallQue is a queue of QCall pointers and it contains a semaphore that 
// the thread waits at. When a QCall pointer is written to the queue, the thread
// wakes up, gets the QCall from the queue and and calls the QCall "execute" 
// CallPointer, passing in the QCall arguments.
// 
// BaseQCallThread supplies a shutdownThread that invokes a mTerminateQCall that
// sets an mTerminateFlag. The threadRunFunction polls the mTerminateFlag
// and exits the thread if it is true.

class BaseQCall;
class QCall0;
class RecursiveExit;
enum {MaxRecursiveDepth=10};

class  BaseBaseQCallThread
{
public:
   virtual void putQCallToThread(BaseQCall* aQCall)=0;
};

class BaseQCallThread : public Ris::Threads::BaseThread,public BaseBaseQCallThread
{
public:
   BaseQCallThread();
  ~BaseQCallThread();

   //--------------------------------------------------------------
   // Thread base class overloads:

   // This sets up the thread timer
   virtual void threadTimerInitFunction(); 

   // This executes a loop that calls threadRunRecursive to process 
   // the call queue. The loop terminates on the mTerminateFlag.
   virtual void threadRunFunction(); 

   // The following two methods returns a status code
   enum RecursiveStatus
   {
      StatusCallExecuted     = 0,
      StatusTimerExecuted    = 1,
      StatusExitOccurred     = 2,
      StatusTimeoutOccurred  = 3,
      StatusAbort            = 4,
      StatusTerminate        = 5,
      StatusMaxDepthReached  = 6,
      StatusNull             = 7,
   };

   // This is called by threadRunFunction to process the call queue.
   // It waits for the call queue semaphore, extracts a call from
   // the call queue, and executes the call. The executing call can 
   // call threadWaitRecursive, which then calls threadRunRecursive. 
   // This is why it is recursive. It is indirectly recursive, such 
   // that it can call a function, which can call a function, which
   // can call it. It returns a RecursiveStatus code.
   int threadRunRecursive(RecursiveExit* aExit=0);

   // This can be called by an executing QCall to wait until another
   // QCall sets aExit or mTerminateFlag is true.
   // This can be used to have the execution of a first QCall wait
   // for the execution of a second QCall, without blocking the
   // queue. It returns a RecursiveStatus code and true if the code
   // is StatusExitOccurred
   bool threadWaitRecursive(RecursiveExit* aExit,int& aStatus,int aTimeout = -1);

   // This can be used by the second QCall to wake up the waiting
   // first QCall. aExit must be a variable that is visible to
   // the first and second QCalls.
   void threadNotifyRecursive(RecursiveExit* aExit);

   // This can be used by the second QCall to wake up the waiting
   // first QCall, for an abort. aExit must be a variable that is
   // visible to the first and second QCalls.
   void threadAbortRecursive(RecursiveExit* aExit);

   // This does a threadWaitRecursive on mSleepExit, to provide a 
   // sleep. It can be called by an executing QCall to sleep without
   // blocking the queue.
   int threadSleepRecursive(int aTimeout = -1);

   RecursiveExit* mSleepExit;

   // Empties the call queue
   void threadExitFunction(); 

   // Invokes the mTerminateQCall, which sets the mTerminateFlag.
   // Then it waits for the thread to terminate.
   void shutdownThread(); 

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // call queue:

   // Mutex protected waitable queue
   //   mCallQue    is a queue of quecalls
   //   mCentralSem is signaled for mCallQue or timer functions
   //   mLimitSem   is limits mCallQue from overflowing 
   //   mCallMutex  is mutex protection

   enum {CallQueSize=200};
   Ris::Containers::Queue<BaseQCall*,CallQueSize>  mCallQue;
   CountingSemaphore                               mCentralSem;
   CountingSemaphore                               mLimitSem;
   MutexSemaphore                                  mCallMutex;   

   //--------------------------------------------------------------
   // This is called by a QCall's invoke method to put itself to
   // the call queue.
   void putQCallToThread(BaseQCall* aQCall);

   //--------------------------------------------------------------
   // These set the queue mode.
   // If     blocking and the queue is full then a queue write is blocked.
   // If not blocking and the queue is full then a queue write is dropped.
   void setQueModeBlocking() {mBlockingQueMode=true;}
   void setQueModeDropping() {mBlockingQueMode=false;}
   bool mBlockingQueMode;

   //--------------------------------------------------------------
   // Terminate QCall:

   // executeTerminate sets the mTerminateFlag, which causes
   // threadRunFunction to exit. It is invoked by shutdownThread.
   void executeTerminate();
   typedef QCall0   TerminateQCall;
   TerminateQCall* mTerminateQCall;

   //Termination Flag
   bool   mTerminateFlag;

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // This is updated by threadRunRecursive to indicate the
   // current recursive depth
   int   mRecursiveDepth;

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Thread timer. It periodically executes threadExecuteOnTimer,
   // not in the execution context of this thread.
   Timer     mThreadTimer;
   TimerCall mThreadTimerCall;
   
   // This is executed by the timer. It updates timer variables
   // and signals the central semaphore to wake up the thread.
   // aCurrentTimeCount gives the number of timer events that
   // have occurred since thread launch.
   void threadExecuteOnTimer(int aCurrentTimeCount);

   // Inheritors provide an overload for this.
   // It is executed in the context of this thread, after the
   // central semaphore wakes up.
   // aCurrentTimeCount gives the number of timer events that
   // have occurred since thread launch.
   // It is called by threadRunFunction and is caused by
   // threadExecuteOnTimer.
   virtual void executeOnTimer(int aCurrentTimeCount){}

   //--------------------------------------------------------------
   // Timer state:

   // Set by threadExecuteOnTimer to indicate that a timer
   // update has occurred
   bool  mTimerExecuteFlag;

   // Timer period in milliseconds
   int   mTimerPeriod;
   // Timer modulo in milliseconds.
   // Inheritor executeOnTimer is called
   // once every mTimerPeriod*mTimerModulo milliseconds
   int   mTimerModulo;

   // These give the number of timer events that have
   // occurred since thread launch
   int   mCurrentTimeCount;
   int   mTimerCurrentTimeCount;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// RecursiveExit is a data structure that is shared between a function that
// calls threadWaitRecursive and a function that calls threadNotifyRecursive.
// threadWaitRecursive uses it to set an exit request and a timeout count.
// threadNotifyRecursive uses it to set an exit acknowledge.
// threadWaitRecursive blocks until an exit acknowledge is received or a
// timeout occurs. There is one record for each recursive depth.

class  RecursiveRecord
{
public:

   RecursiveRecord();
   void reset();

   // Exit request
   void setExitReq();
   bool isExitReq();
   bool mExitReqFlag;

   // Exit acknowledge
   void setExitAck();
   bool isExitAck();
   bool mExitAckFlag;

   // Exit abort
   void setExitAbort();
   bool isExitAbort();
   bool mExitAbortFlag;

   // If the current time count exceeds the timeout
   // time count, then a timeout has occurred
   int  mTimeoutTimeCount;
};

//******************************************************************************
// Contains an array of records, one for each recursive depth
// and has methods to control an exit or a timeout

class  RecursiveExit
{
public:

   RecursiveExit();
   void reset();

   //---------------------------------------------------------------------------
   // Array of records, for each recursive depth

   RecursiveRecord mRecord[MaxRecursiveDepth];

   //---------------------------------------------------------------------------
   // Exit request and acknowledge, for a given recursive depth

   void reset(int aDepth);
   void setExitReq(int aDepth);
   bool isExitReq(int aDepth);
   void setExitAck(int aDepth);
   bool isExitAck(int aDepth);
   void setExitAbort(int aDepth);
   bool isExitAbort(int aDepth);

   //---------------------------------------------------------------------------
   // Timeout methods

   // Sets the timeout time count to aCurrentTimeCount + aTimeoutInCounts
   void initializeTimeout(int aDepth,int aCurrentTimeCount,int aTimeoutInCounts);

   // Returns true if the current time count exceeds the timeout time count
   bool isTimeout(int aDepth,int aCurrentTimeCount);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall

//******************************************************************************
// Base class

class  BaseQCall
{
public:
   BaseQCall()
   {
      mSpecialFlag=false;
   }

   //---------------------------------------------------------------------------
   // Invoke, sends a QCall to a thread call queue:

   // Invoke CallPointer, it contains the address of the called thread's
   // putQCallToThread member function. It is called by inheritor class 
   // templates to send a QCall to a call queue.
   // void putQCallToThread(BaseQCall* aQCall);
   typedef Ris::CallPointer1<BaseQCall*> InvokeCallPointer;
   InvokeCallPointer mInvokeCallPointer;

   //---------------------------------------------------------------------------
   // Execute, it is called by the called thread to execute the deferred 
   // procedure:
   virtual void execute()=0;

   //---------------------------------------------------------------------------
   // Special flag
   bool mSpecialFlag;

};

//******************************************************************************
// QCall0 is a class for a QCall with 0 arguments

class  QCall0 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Invoke, sends a QCall to a thread call queue:

   // This is called by the calling thread.
   // It creates a new copy of the QCall, sets its arguments, and invokes it.
   void invoke()
   {
      // Create a new copy of this QCall.
      QCall0* tQCall = new QCall0(*this);
      // Invoke it.
      mInvokeCallPointer(tQCall);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the called thread to execute the deferred 
   // procedure:

   // Execute CallPointer, it contains the address of the called thread's
   // deferred procedure.
   typedef Ris::CallPointer0 ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the called thread
   void execute()
   {
      mExecuteCallPointer();
   }

   //---------------------------------------------------------------------------
   // Bind:

   template <class CallObject,class CallMethod>
   void bind(CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aCallObject,&BaseBaseQCallThread::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
// QCall1 is a class template for a QCall with 1 argument

template <class X1>
class  QCall1 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure arguments:

   X1 mX1;

   //---------------------------------------------------------------------------
   // Invoke, sends a QCall to a thread call queue:

   // This is called by the calling thread.
   // It creates a new copy of the QCall, sets its arguments, and invokes it.
   void invoke(X1 aX1)
   {
      // Create a new copy of this QCall.
      QCall1* tQCall = new QCall1(*this);
      // Set its arguments.
      tQCall->mX1=aX1;
      // Invoke it.
      mInvokeCallPointer(tQCall);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the called thread to execute the deferred 
   // procedure:

   // Execute CallPointer, it contains the address of the called thread's
   // deferred procedure.
   typedef Ris::CallPointer1<X1> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the called thread
   void execute()
   {
      mExecuteCallPointer(mX1);
   }

   //---------------------------------------------------------------------------
   // Bind:

   template <class CallObject,class CallMethod>
   void bind(CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aCallObject,&BaseBaseQCallThread::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
// QCall2 is a class template for a QCall with 2 arguments

template <class X1,class X2>
class  QCall2 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure arguments:

   X1 mX1;
   X2 mX2;

   //---------------------------------------------------------------------------
   // Invoke, sends a QCall to a thread call queue:

   // This is called by the calling thread.
   // It creates a new copy of the QCall, sets its arguments, and invokes it.
   void invoke(X1 aX1,X2 aX2)
   {
      // Create a new copy of this QCall.
      QCall2* tQCall = new QCall2(*this);
      // Set its arguments.
      tQCall->mX1=aX1;
      tQCall->mX2=aX2;
      // Invoke it.
      mInvokeCallPointer(tQCall);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the called thread to execute the deferred 
   // procedure:

   // Execute CallPointer, it contains the address of the called thread's
   // deferred procedure.
   typedef Ris::CallPointer2<X1,X2> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the called thread
   void execute()
   {
      mExecuteCallPointer(mX1,mX2);
   }

   //---------------------------------------------------------------------------
   // Bind:

   template <class CallObject,class CallMethod>
   void bind(CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aCallObject,&BaseBaseQCallThread::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
// QCall3 is a class template for a QCall with 3 arguments

template <class X1,class X2,class X3>
class  QCall3 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure arguments:

   X1 mX1;
   X2 mX2;
   X3 mX3;

   //---------------------------------------------------------------------------
   // Invoke, sends a QCall to a thread call queue:

   // This is called by the calling thread.
   // It creates a new copy of the QCall, sets its arguments, and invokes it.
   void invoke(X1 aX1,X2 aX2,X3 aX3)
   {
      // Create a new copy of this QCall.
      QCall3* tQCall = new QCall3(*this);
      // Set its arguments.
      tQCall->mX1=aX1;
      tQCall->mX2=aX2;
      tQCall->mX3=aX3;
      // Invoke it.
      mInvokeCallPointer(tQCall);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the called thread to execute the deferred 
   // procedure:

   // Execute CallPointer, it contains the address of the called thread's
   // deferred procedure.
   typedef Ris::CallPointer3<X1,X2,X3> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the called thread
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3);
   }

   //---------------------------------------------------------------------------
   // Bind:

   template <class CallObject,class CallMethod>
   void bind(CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aCallObject,&BaseBaseQCallThread::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
// QCall4 is a class template for a QCall with 4 arguments

template <class X1,class X2,class X3,class X4>
class  QCall4 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure arguments:

   X1 mX1;
   X2 mX2;
   X3 mX3;
   X4 mX4;

   //---------------------------------------------------------------------------
   // Invoke, sends a QCall to a thread call queue:

   // This is called by the calling thread.
   // It creates a new copy of the QCall, sets its arguments, and invokes it.
   void invoke(X1 aX1,X2 aX2,X3 aX3,X4 aX4)
   {
      // Create a new copy of this QCall.
      QCall4* tQCall = new QCall4(*this);
      // Set its arguments.
      tQCall->mX1=aX1;
      tQCall->mX2=aX2;
      tQCall->mX3=aX3;
      tQCall->mX4=aX4;
      // Invoke it.
      mInvokeCallPointer(tQCall);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the called thread to execute the deferred 
   // procedure:

   // Execute CallPointer, it contains the address of the called thread's
   // deferred procedure.
   typedef Ris::CallPointer4<X1,X2,X3,X4> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the called thread
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3,mX4);
   }

   //---------------------------------------------------------------------------
   // Bind:

   template <class CallObject,class CallMethod>
   void bind(CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aCallObject,&BaseBaseQCallThread::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************


}//namespace
}//namespace

#pragma managed(pop)

#endif

