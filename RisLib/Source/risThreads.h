#ifndef _RISTHREADS_H_
#define _RISTHREADS_H_
/*==============================================================================

Realtime infastructure library Threads

This file contains classes that encapsulate standard rtos multithreading
synchronization constructs. It supplies events, semaphores, and threads.
The purpose of the classes is to wrap the rtos api thread synchronization
calls.

class BinarySemaphore
class CountingSemaphore
class MutexSemaphore
class WaitableTimer
class BaseThread
class BaseThreadWithTermFlag 
class BaseThreadWithTermSem
class BaseTimerThread
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "ris_priorities.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
// This encapsulates a binary semaphore.

class BinarySemaphore
{
public:

   BinarySemaphore();            //creates the semaphore
   virtual ~BinarySemaphore();   //deletes the semaphore
   void reset();                 //resets the semaphore

   void put ();                  //put to the semaphore
   bool get (int timeout= -1);   //get from the semaphore, block until timeout,
                                 //return true if no timeout

   int mStatusCode;              //user status code

   void* getHandlePtr();

protected:
   class Implementation;
   Implementation* mImplementation;
};

//******************************************************************************
// This encapsulates a counting semaphore.

class CountingSemaphore
{
public:

   CountingSemaphore();             //creates the semaphore
   CountingSemaphore(int aInitial); //creates the semaphore
   virtual ~CountingSemaphore();    //deletes the semaphore

   void put ();                     //put to the semaphore
   bool get (int timeout= -1);      //get from the semaphore, block until timeout,
                                    //return true if no timeout

   void* getHandlePtr();

protected:
   class Implementation;
   Implementation* mImplementation;
};

//******************************************************************************
// This encapsulates a mutex semaphore.

class MutexSemaphore
{
public:

   MutexSemaphore();             //creates the semaphore
   virtual ~MutexSemaphore();    //deletes the semaphore

   void put ();                  //put to the semaphore
   bool get (int timeout= -1);   //get from the semaphore, block until timeout,
                                 //return true if no timeout
   // synonyms
   void unlock  ()                {put();}
   bool lock    (int timeout= -1) {return get(timeout);}

   void give    ()                {put();}
   bool take    (int timeout= -1) {return get(timeout);}

   void release ()                {put();}
   bool acquire (int timeout= -1) {return get(timeout);}

   void* getHandlePtr();

protected:
   class Implementation;
   Implementation* mImplementation;
};

//******************************************************************************
// This is an abstract virtual base class for threads.
// It encapsulates the win32 CreateThread call.
// Derived classes can use this to inherit a basic form for classes
// that encapsulate threads by providing member function overloads.
 
class BaseThread
{
public:

   // Resets the handle to indicate a null thread.
   // Sets the configuration variables to default values.

   BaseThread(); 
   virtual ~BaseThread();

   // Sets the configuration variables to default values.
   // This can be overloaded to set them to specific values.
   // This is called by launch before the thread is created.
   // Note that launch uses the configuration variables when it
   // does the CreateThread.

   virtual void configureThread(); 

   // This launches the thread.
   // It calls win32 CreateThread and passes in the configuration variables
   // and the address of the c-function BaseThread_Execute and a this pointer.
   // The CreateThread call creates a new win32 thread with the configuration
   // and and indirectly (via BaseThread_Execute) executes the threadFunction
   // member function as the thread entry point.
   //
   // When it is called, it causes the following to happen:
   //   1) A new thread is created, mHandle is the handle.
   //   2) the new thread has parameters as specified in the configuration
   //      variables (stack size, flags, NOT thread priority)
   //   3) Member function threadFunction immediately executes in the
   //       context of the new thread.
   //   4) After the CreateThread, this call waits on the initialization
   //      semaphore. It does not return until the threadFunction signals it.
   //
   // Note:
   // Creation of the thread does not set the thread priority before 
   // threadFunction is called (this is because that's the way win32
   // CreateThread does it). The first thing that threadFunction does is set
   // the priority.

   virtual void launchThread(); 

   // Thread function.
   // This is called (indirectly) by the CreateThread and it executes in the
   // context of the new thread. The thread automatically terminates when 
   // this function returns.
   // 
   // It does the following:
   //   1) Sets the thread priority as specified in the configuration
   //   2) Calls the threadInitFunction initialization section
   //   3) Calls the threadTimerInitFunction initialization section
   //   4) Signals the initialization semaphore. The call to launched
   //      that created this thread waits on this semaphore before it
   //      returns.
   //   5) Calls the threadRunFunction run section
   //
   // The thread function is partitioned into separate initialization
   // and run sections to provide for a serialized thread initialization
   // sequence. This means that multiple threads can be launched in sequence
   // and it is guarenteed that a threads' init section will not
   // execute until the previously launched threads' init section completes.

   // This is the function that is run in the context of the new thread.
   // It calls the following four subfunctions. The thread launch doesn't 
   // return until the first two have completed.

   virtual void threadFunction(); 

   // Thread function initialization and run sections.
   // These are called by threadFunction, see above.

   // This is an initialization section. It's purpose is to provide a thread
   // initialization such that the launching thread waits for it to complete
   virtual void threadInitFunction(); 
   // This is used by inheritors to initialize thread timers. This should be
   // overloaded by thread base classes and not by thread user classes.
   // The launching thread also waits for this to complete.
   virtual void threadTimerInitFunction(); 
   // This should be used by inheritors to do the actual work of the thread
   virtual void threadRunFunction(); 
   // This should be used by inheritors when the thread exits.It is used
   // for any thread cleanup
   virtual void threadExitFunction(); 

   // This is called if there is an exception thrown in the above threadFunction.
   // Exceptions should throw a char*.
   virtual void threadExceptionFunction(char* aStr);

   // This virtual is overloaded by inheritors to shutdown the thread
   virtual void shutdownThread() {}

   // Here are some standard thread calls

   void threadSleep(int ticks); 
   void forceTerminateThread();
   int  getThreadPriority();

   // Call this in the configureThread
   void setThreadPriorityHigh();
   void setThreadPriorityLow();
   void setThreadPriority(int aThreadPriority);

   //This waits forever for the threads' handle.
   //It returns when the thread terminates.
   //It can be used to wait for a thread to terminate.

   void waitForThreadTerminate();

   //Configuration variables.
   //Some of these are passed to the CreateThread in launch
   //and some are used by threadFunction.

   int    mThreadStackSize;
   int    mThreadPriority;
   int    mThreadAffinityMask;
   int    mThreadIdealProcessor;
   int    mThreadUseInitSem;

   //Exit semaphore.
   //It is posted to after the end of threadExitFunction.
   //waitForThreadTerminate waits for it.

   BinarySemaphore  mThreadExitSem;

public:
   void* getHandlePtr();

protected:
   class BaseImplementation;
   BaseImplementation* mBaseImplementation;
};

//******************************************************************************
// This is an extension of virtual base class for general purpose threads.
// It provides a polled termination mechanism where an inheritor thread run
// function should poll a termination flag.

class BaseThreadWithTermFlag : public BaseThread
{
public:

   BaseThreadWithTermFlag(); 

   virtual ~BaseThreadWithTermFlag(){} 

   //This sets the terminate flag and waits on the handle for the thread
   //to terminate. The inheritor thread run function should polls this
   //flag and terminate if it is true.
   virtual void shutdownThread();

protected:
   //Termination Flag
   bool   mTerminateFlag;
};

//******************************************************************************
// This is an extension of virtual base class for general purpose threads.
// It provides a termination mechanism where an inheritor thread run
// function pends on a termination semaphore.
//
// This can be used for threads whose purpose is to create and hold other
// threads (a platform thread). An inheritor thread run function can create 
// other threads and then pend on the termination semaphore. When it is
// signalled, the thread wakes up, terminates its child threads and terminates
// itself.

class BaseThreadWithTermSem : public BaseThread
{
public:

   virtual ~BaseThreadWithTermSem(){} 

   //This posts to the termination semaphore.
   //The inheritor thread run function should pend on the semaphore
   //and terminate if it is signaled.
   virtual void shutdownThread();

protected:
   // Termination semaphore
   BinarySemaphore   mTerminateSem;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
int countsPerOneSecond();

void halt(char* aPrintStr=0);

}//namespace
}//namespace
#endif


