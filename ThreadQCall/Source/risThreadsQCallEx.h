#ifndef _RISTHREADSQCALLEX_H_
#define _RISTHREADSQCALLEX_H_

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
#include "risCallPointer.h"

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
// QCall

//******************************************************************************
// Base class

class  BaseQCall
{
public:
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

   // Invoke function call overload
   void operator()()
   {
      invoke();
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the called thread to execute the deferred 
   // procedure:

   // Execute CallPointer, it contains the address of the called thread's
   // deferred procedure.
   typedef Ris::CallPointer0<> ExecuteCallPointer;
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
      mInvokeCallPointer.bind  (aCallObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   template <class InvokeToObject,class CallObject,class CallMethod>
   void bind(InvokeToObject aInvokeToObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aInvokeToObject,&BaseQCallTargetEx::putQCallToThread);
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

   // Invoke function call overload
   void operator()(X1 aX1)
   {
      invoke(aX1);
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
      mInvokeCallPointer.bind  (aCallObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   template <class InvokeToObject,class CallObject,class CallMethod>
   void bind(InvokeToObject aInvokeToObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aInvokeToObject,&BaseQCallTargetEx::putQCallToThread);
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

   // Invoke function call overload
   void operator()(X1 aX1,X2 aX2)
   {
      invoke(aX1,aX2);
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
      mInvokeCallPointer.bind  (aCallObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   template <class InvokeToObject,class CallObject,class CallMethod>
   void bind(InvokeToObject aInvokeToObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aInvokeToObject,&BaseQCallTargetEx::putQCallToThread);
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

   // Invoke function call overload
   void operator()(X1 aX1,X2 aX2,X3 aX3)
   {
      invoke(aX1,aX2,aX3);
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
      mInvokeCallPointer.bind  (aCallObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   template <class InvokeToObject,class CallObject,class CallMethod>
   void bind(InvokeToObject aInvokeToObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aInvokeToObject,&BaseQCallTargetEx::putQCallToThread);
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

   // Invoke function call overload
   void operator()(X1 aX1,X2 aX2,X3 aX3,X4 aX4)
   {
      invoke(aX1,aX2,aX3,aX4);
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
      mInvokeCallPointer.bind  (aCallObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   template <class InvokeToObject,class CallObject,class CallMethod>
   void bind(InvokeToObject aInvokeToObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aInvokeToObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
// QCall5 is a class template for a QCall with 5 arguments

template <class X1,class X2,class X3,class X4,class X5>
class  QCall5 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure arguments:

   X1 mX1;
   X2 mX2;
   X3 mX3;
   X4 mX4;
   X5 mX5;

   //---------------------------------------------------------------------------
   // Invoke, sends a QCall to a thread call queue:

   // This is called by the calling thread.
   // It creates a new copy of the QCall, sets its arguments, and invokes it.
   void invoke(X1 aX1,X2 aX2,X3 aX3,X4 aX4,X5 aX5)
   {
      // Create a new copy of this QCall.
      QCall5* tQCall = new QCall5(*this);
      // Set its arguments.
      tQCall->mX1=aX1;
      tQCall->mX2=aX2;
      tQCall->mX3=aX3;
      tQCall->mX4=aX4;
      tQCall->mX5=aX5;
      // Invoke it.
      mInvokeCallPointer(tQCall);
   }

   // Invoke function call overload
   void operator()(X1 aX1,X2 aX2,X3 aX3,X4 aX4,X5 aX5)
   {
      invoke(aX1,aX2,aX3,aX4,aX5);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the called thread to execute the deferred 
   // procedure:

   // Execute CallPointer, it contains the address of the called thread's
   // deferred procedure.
   typedef Ris::CallPointer5<X1,X2,X3,X4,X5> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the called thread
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3,mX4,mX5);
   }

   //---------------------------------------------------------------------------
   // Bind:

   template <class CallObject,class CallMethod>
   void bind(CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aCallObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   template <class InvokeToObject,class CallObject,class CallMethod>
   void bind(InvokeToObject aInvokeToObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aInvokeToObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
// QCall6 is a class template for a QCall with 6 arguments

template <class X1,class X2,class X3,class X4,class X5,class X6>
class  QCall6 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure arguments:

   X1 mX1;
   X2 mX2;
   X3 mX3;
   X4 mX4;
   X5 mX5;
   X6 mX6;

   //---------------------------------------------------------------------------
   // Invoke, sends a QCall to a thread call queue:

   // This is called by the calling thread.
   // It creates a new copy of the QCall, sets its arguments, and invokes it.
   void invoke(X1 aX1,X2 aX2,X3 aX3,X4 aX4,X5 aX5,X6 aX6)
   {
      // Create a new copy of this QCall.
      QCall6* tQCall = new QCall6(*this);
      // Set its arguments.
      tQCall->mX1=aX1;
      tQCall->mX2=aX2;
      tQCall->mX3=aX3;
      tQCall->mX4=aX4;
      tQCall->mX5=aX5;
      tQCall->mX6=aX6;
      // Invoke it.
      mInvokeCallPointer(tQCall);
   }

   // Invoke function call overload
   void operator()(X1 aX1,X2 aX2,X3 aX3,X4 aX4,X5 aX5,X6 aX6)
   {
      invoke(aX1,aX2,aX3,aX4,aX5,aX6);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the called thread to execute the deferred 
   // procedure:

   // Execute CallPointer, it contains the address of the called thread's
   // deferred procedure.
   typedef Ris::CallPointer6<X1,X2,X3,X4,X5,X6> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the called thread
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3,mX4,mX5,mX6);
   }

   //---------------------------------------------------------------------------
   // Bind:

   template <class CallObject,class CallMethod>
   void bind(CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aCallObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   template <class InvokeToObject,class CallObject,class CallMethod>
   void bind(InvokeToObject aInvokeToObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aInvokeToObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
// QCall7 is a class template for a QCall with 7 arguments

template <class X1,class X2,class X3,class X4,class X5,class X6,class X7>
class  QCall7 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure arguments:

   X1 mX1;
   X2 mX2;
   X3 mX3;
   X4 mX4;
   X5 mX5;
   X6 mX6;
   X7 mX7;

   //---------------------------------------------------------------------------
   // Invoke, sends a QCall to a thread call queue:

   // This is called by the calling thread.
   // It creates a new copy of the QCall, sets its arguments, and invokes it.
   void invoke(X1 aX1,X2 aX2,X3 aX3,X4 aX4,X5 aX5,X6 aX6,X7 aX7)
   {
      // Create a new copy of this QCall.
      QCall7* tQCall = new QCall7(*this);
      // Set its arguments.
      tQCall->mX1=aX1;
      tQCall->mX2=aX2;
      tQCall->mX3=aX3;
      tQCall->mX4=aX4;
      tQCall->mX5=aX5;
      tQCall->mX6=aX6;
      tQCall->mX7=aX7;
      // Invoke it.
      mInvokeCallPointer(tQCall);
   }

   // Invoke function call overload
   void operator()(X1 aX1,X2 aX2,X3 aX3,X4 aX4,X5 aX5,X6 aX6,X7 aX7)
   {
      invoke(aX1,aX2,aX3,aX4,aX5,aX6,aX7);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the called thread to execute the deferred 
   // procedure:

   // Execute CallPointer, it contains the address of the called thread's
   // deferred procedure.
   typedef Ris::CallPointer7<X1,X2,X3,X4,X5,X6,X7> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the called thread
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3,mX4,mX5,mX6,mX7);
   }

   //---------------------------------------------------------------------------
   // Bind:

   template <class CallObject,class CallMethod>
   void bind(CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aCallObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   template <class InvokeToObject,class CallObject,class CallMethod>
   void bind(InvokeToObject aInvokeToObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aInvokeToObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************
// QCall8 is a class template for a QCall with 8 arguments

template <class X1,class X2,class X3,class X4,class X5,class X6,class X7,class X8>
class  QCall8 : public BaseQCall
{
public:
   //---------------------------------------------------------------------------
   // Queued procedure arguments:

   X1 mX1;
   X2 mX2;
   X3 mX3;
   X4 mX4;
   X5 mX5;
   X6 mX6;
   X7 mX7;
   X8 mX8;

   //---------------------------------------------------------------------------
   // Invoke, sends a QCall to a thread call queue:

   // This is called by the calling thread.
   // It creates a new copy of the QCall, sets its arguments, and invokes it.
   void invoke(X1 aX1,X2 aX2,X3 aX3,X4 aX4,X5 aX5,X6 aX6,X7 aX7,X8 aX8)
   {
      // Create a new copy of this QCall.
      QCall8* tQCall = new QCall8(*this);
      // Set its arguments.
      tQCall->mX1=aX1;
      tQCall->mX2=aX2;
      tQCall->mX3=aX3;
      tQCall->mX4=aX4;
      tQCall->mX5=aX5;
      tQCall->mX6=aX6;
      tQCall->mX7=aX7;
      tQCall->mX8=aX8;
      // Invoke it.
      mInvokeCallPointer(tQCall);
   }

   // Invoke function call overload
   void operator()(X1 aX1,X2 aX2,X3 aX3,X4 aX4,X5 aX5,X6 aX6,X7 aX7,X8 aX8)
   {
      invoke(aX1,aX2,aX3,aX4,aX5,aX6,aX7,aX8);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the called thread to execute the deferred 
   // procedure:

   // Execute CallPointer, it contains the address of the called thread's
   // deferred procedure.
   typedef Ris::CallPointer7<X1,X2,X3,X4,X5,X6,X7,X8> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the called thread
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3,mX4,mX5,mX6,mX7,mX8);
   }

   //---------------------------------------------------------------------------
   // Bind:

   template <class CallObject,class CallMethod>
   void bind(CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aCallObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   template <class InvokeToObject,class CallObject,class CallMethod>
   void bind(InvokeToObject aInvokeToObject,CallObject aCallObject,CallMethod aCallMethod)
   {
      mInvokeCallPointer.bind  (aInvokeToObject,&BaseQCallTargetEx::putQCallToThread);
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }
};

//******************************************************************************


}//namespace
}//namespace

#endif

