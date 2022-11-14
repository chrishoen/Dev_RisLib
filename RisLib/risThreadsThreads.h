#pragma once
/*==============================================================================
Base thread classes
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsSynch.h"
#include "risThreadsPriorities.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an abstract virtual base class for threads.
// It encapsulates the win32 CreateThread call.
// Derived classes can use this to inherit a basic form for classes
// that encapsulate threads by providing member function overloads.
 
class BaseThread
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 128;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Thread run states.
   static const int cThreadRunState_Launching  = 1;
   static const int cThreadRunState_InitR      = 2;
   static const int cThreadRunState_InitF      = 3;
   static const int cThreadRunState_Running    = 4;
   static const int cThreadRunState_ExitR      = 5;
   static const int cThreadRunState_ExitF      = 6;
   static const int cThreadRunState_Terminated = 7;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Thread name.
   char mThreadName[cMaxStringSize + 2];

   // Thread run state.
   int mThreadRunState;

   // Configuration variables.
   // Some of these are passed to the CreateThread in launch
   // and some are used by threadFunction.
   int mThreadStackSize;
   int mThreadPriority;
   int mThreadSingleProcessor;

   // Init semaphore.
   // It is posted to after the end of threadInitFunction.
   // launchThread waits for it.
   BinarySemaphore  mThreadInitSem;

   // Exit semaphore.
   // It is posted to after the end of threadExitFunction.
   // waitForThreadTerminate waits for it.
   BinarySemaphore  mThreadExitSem;

   // The processor that was current at the start of the thread
   // run function.
   int mThreadRunProcessor;

   // The processor that was current at the last call to get
   // the current processor.
   int mThreadCurrentProcessor;

   // Pimpl pattern. Used to hide details of the operating system specific
   // variables, like the thread handle, from the .h file so that this
   // include file can be complied by different compliers. The class is
   // defined in the .cpp file, where there is a different version for
   // different compilers.
   class BaseSpecific;
   BaseSpecific* mBaseSpecific;

   //Termination Flag
   bool mTerminateFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Set the thread services thread name in the thread local storage.
   void setThreadName(const char* aThreadName);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   BaseThread(); 
   virtual ~BaseThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Launch the thread.
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
   void threadFunction();

   // Thread function initialization and run sections.
   // These are called by threadFunction, see above.

   // This is used by inheritors to initialize resources. This should be
   // overloaded by thread base classes and not by thread user classes.
   virtual void threadResourceInitFunction(){}

   // Initialization section, overload provided by inheritors
   // It is intended that this will be overloaded by 
   // inheriting thread base classes and by inheriting user classes
   virtual void threadInitFunction(){}

   // This should be used by inheritors to do the actual work of the thread
   virtual void threadRunFunction(){}

   // This should be used by inheritors when the thread exits.It is used
   // for any thread cleanup
   virtual void threadExitFunction(){}

   // This is used by inheritors to finalize resources. This should be
   // overloaded by thread base classes and not by thread user classes.
   virtual void threadResourceExitFunction(){}

   // This is called if there is an exception thrown in the above threadFunction.
   // Exceptions should throw a char*.
   virtual void threadExceptionFunction(const char* aStr);

   // This virtual is overloaded by inheritors to shutdown the thread.
   virtual void shutdownThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Sleep for some milliseconds.
   void threadSleep(int aTicks); 

   // Set the thread processor number and priority.
   void setThreadPriority(Priority aPriority);
   void setThreadPriority(int aPriority);
   void setThreadPriorityLow();
   void setThreadPriorityNormal();
   void setThreadPriorityHigh();

   // Wait for the thread to terminate.
   void waitForThreadTerminate();

   // Terminate the thread forcefully.
   void forceTerminateThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Return the thread priority.
   int  getThreadPriority();

   // Return the thread processor number.
   int  getThreadProcessorNumber();

   // Show thread configuration info.
   void showThreadFullInfo();

   // Show thread state info.
   virtual void showThreadInfo();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Helpers.
   const char* asStringThreadRunState();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Helpers.

// Return the number of operation system ticks per second.
int countsPerOneSecond();

// Sleep.
void threadSleep(int aTicks);

// Halt the program.
void halt(const char* aPrintStr=0);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


