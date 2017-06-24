/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h> 
#include <stdio.h>
#include <assert.h>

#include "my_functions.h"
#include "ris_priorities.h"
#include "prnPrint.h"
#include "risThreadsThreads.h"

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
// This is a c-function that is used to execute the thread function of 
// a base thread object. It is passed to CreateThread to indirectly call
// the thread function because CreateThread can't use member function
// addresses.

int BaseThread_Execute (BaseThread* someThread)
{
   someThread->threadFunction();
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseThread::BaseSpecific
{
public:
   HANDLE mHandle;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseThread::BaseThread() 
{
   mBaseSpecific = new BaseSpecific;
   mBaseSpecific->mHandle    = 0;
   mThreadPriority = get_default_thread_priority();
   mThreadAffinityMask    = 0;
   mThreadIdealProcessor  = -1;

   mThreadStackSize = 0;
   mThreadInitSemFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseThread::~BaseThread() 
{
   delete mBaseSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void* BaseThread::getHandlePtr()
{
   return (void*)&mBaseSpecific->mHandle;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::configureThread()
{
   mBaseSpecific->mHandle = 0;
   mThreadStackSize = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::setThreadPriorityHigh()
{
   mThreadPriority = get_default_high_thread_priority();
}

void BaseThread::setThreadPriorityLow()
{
   mThreadPriority = get_default_low_thread_priority();
}

void BaseThread::setThreadPriority(int aThreadPriority)
{
   mThreadPriority = aThreadPriority;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::launchThread()
{
   //***************************************************************************
   // Configure thread parameters, but do not make any OS calls

   configureThread();

   //***************************************************************************
   // Create thread, suspended

   DWORD tThreadFlags = CREATE_SUSPENDED;

   // Create the thread in a suspended state.
   // The following threadFunction() is executed in 
   // the context of the created thread.
   mBaseSpecific->mHandle=CreateThread(
        NULL, 
       (DWORD) mThreadStackSize,
       (LPTHREAD_START_ROUTINE)BaseThread_Execute,
       (LPVOID)this,
       (DWORD)tThreadFlags,
        NULL);
 
   //***************************************************************************
   // Set thread parameters

   if( SetThreadPriority(mBaseSpecific->mHandle,mThreadPriority) == 0 )
   {
      char tStr[100];
      sprintf(tStr, "Bad Thread Priority %d %d %d",mThreadPriority,*((int*)&mBaseSpecific->mHandle),(int)GetLastError());
      halt(tStr);
   }

   if (mThreadAffinityMask != 0)
   {
      if( SetThreadAffinityMask(mBaseSpecific->mHandle,mThreadAffinityMask) == -1 ) throw "Bad Thread Affinity";
   }
   if (mThreadIdealProcessor != -1)
   {
      if( SetThreadIdealProcessor(mBaseSpecific->mHandle,mThreadIdealProcessor) == -1 ) throw "Bad Ideal Processor";
   }

   //***************************************************************************
   // Resume thread, this starts the thread function within the context of
   // the new thread,using configured priority and affinity

   ResumeThread(mBaseSpecific->mHandle);

   //***************************************************************************
   // Wait for the thread init function to complete.

   if (mThreadInitSemFlag)
   {
      mThreadInitSem.get();
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the function that is executed in the context of the created thread.
// It calls a sequence of functions that are overloaded by inheritors.

void BaseThread::threadFunction()
{
   // Thread execution
   try
   {
      // This is used by inheritors to initialize resources. This should be
      // overloaded by thread base classes and not by thread user classes.
      threadResourceInitFunction();
      // Initialization section, overload provided by inheritors
      // It is intended that this will be overloaded by 
      // inheriting thread base classes and by inheriting user classes
      threadInitFunction();
      // It is intended that this will be overloaded by 
      // inheriting thread base classes that provide timers,
      // and not by inheriting user classes.
      // Note that the timer starts after the initialization section
      // has completed 
      threadTimerInitFunction();
      // Post to the thread init semaphore.
      if (mThreadInitSemFlag)
      {
         mThreadInitSem.put();
      }
      // Run section, overload provided by inheritors 
      threadRunFunction();
      // Exit section, overload provided by inheritors
      threadExitFunction();
      // This is used by inheritors to finalize resources. This should be
      // overloaded by thread base classes and not by thread user classes.
      threadResourceExitFunction();
   }
   catch (char* aStr)
   {
      // Exception section, overload provided by inheritors
      threadExceptionFunction(aStr);
   }
   catch (...)
   {
      // Exception section, overload provided by inheritors
      threadExceptionFunction("UNKNOWN");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::threadExceptionFunction(char* aStr)
{
   Prn::print(0,"BaseThread::threadExceptionFunction %s",aStr);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::threadSleep(int aTicks)
{
   Sleep(aTicks);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::forceTerminateThread()
{
   TerminateThread(mBaseSpecific->mHandle,-1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int BaseThread::getThreadPriority()
{
    return GetThreadPriority(mBaseSpecific->mHandle);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::waitForThreadTerminate()
{
   WaitForSingleObject(mBaseSpecific->mHandle,INFINITE);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseThreadWithTermFlag::BaseThreadWithTermFlag() 
{
   mTerminateFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThreadWithTermFlag::shutdownThread()
{
   mTerminateFlag = true;
   waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThreadWithTermSem::shutdownThread()
{
   mTerminateSem.put();
   waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int countsPerOneSecond()
{
   return 1000;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void threadSleep(int aTicks)
{
   Sleep(aTicks);
}

void halt(char* aPrintStr)
{
   printf("HALTING %s\n",aPrintStr);
   exit(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

