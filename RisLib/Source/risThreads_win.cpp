/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#ifdef _AFXDLL
// MFC project
#include <afxwin.h>
#else
#define _WIN32_WINNT 0x0500
// NOT MFC project
#include <windows.h> 
#endif

#include "my_functions.h"

#include <stdio.h>
#include <assert.h>
#include "prnPrint.h"
#include "risThreads.h"

#pragma unmanaged

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BinarySemaphore::Implementation
{
public:
   HANDLE mHandle;
};

//******************************************************************************

BinarySemaphore::BinarySemaphore() 
{
   mImplementation = new Implementation;
   mImplementation->mHandle = CreateEvent(NULL,FALSE,FALSE,NULL);
   mStatusCode=0;
}

//******************************************************************************

BinarySemaphore::~BinarySemaphore() 
{
   CloseHandle(mImplementation->mHandle);
   delete mImplementation;
}

//******************************************************************************

void* BinarySemaphore::getHandlePtr()
{
   return (void*)&mImplementation->mHandle;
}

//******************************************************************************

void BinarySemaphore::reset()
{
   ResetEvent(mImplementation->mHandle);
}

//******************************************************************************

void BinarySemaphore::put()
{
   SetEvent(mImplementation->mHandle);
}

//******************************************************************************

bool BinarySemaphore::get(int timeout)
{
   return WaitForSingleObject(mImplementation->mHandle,timeout) != WAIT_TIMEOUT ;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class CountingSemaphore::Implementation
{
public:
   HANDLE mHandle;
};

//******************************************************************************

CountingSemaphore::CountingSemaphore() 
{
   mImplementation = new Implementation;
   mImplementation->mHandle = CreateSemaphore(NULL,0,1000000,NULL);
}

CountingSemaphore::CountingSemaphore(int aInitial) 
{
   mImplementation = new Implementation;
   mImplementation->mHandle = CreateSemaphore(NULL,aInitial,1000000,NULL);
}

//******************************************************************************

CountingSemaphore::~CountingSemaphore() 
{
   CloseHandle(mImplementation->mHandle);
   delete mImplementation;
}

//******************************************************************************

void* CountingSemaphore::getHandlePtr()
{
   return (void*)&mImplementation->mHandle;
}

//******************************************************************************

void CountingSemaphore::put()
{
   ReleaseSemaphore(mImplementation->mHandle,1,NULL);
}

//******************************************************************************

bool CountingSemaphore::get(int timeout)
{
   switch (WaitForSingleObjectEx(mImplementation->mHandle,timeout,TRUE))
   {
      case WAIT_OBJECT_0      : return true;  break;
      case WAIT_TIMEOUT       : return false; break;
      case WAIT_IO_COMPLETION : return false; break;
      default                 : return false; break;
   } 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class MutexSemaphore::Implementation
{
public:
   HANDLE mHandle;
};

//******************************************************************************

MutexSemaphore::MutexSemaphore() 
{
   mImplementation = new Implementation;
   mImplementation->mHandle = CreateMutex(NULL,FALSE,NULL);
}

//******************************************************************************

MutexSemaphore::~MutexSemaphore() 
{
   CloseHandle(mImplementation->mHandle);
   delete mImplementation;
}

//******************************************************************************

void* MutexSemaphore::getHandlePtr()
{
   return (void*)&mImplementation->mHandle;
}

//******************************************************************************

void MutexSemaphore::put()
{
   ReleaseMutex(mImplementation->mHandle);
}

//******************************************************************************

bool MutexSemaphore::get(int timeout)
{
   return WaitForSingleObject(mImplementation->mHandle,timeout) != WAIT_TIMEOUT ;
}

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

class BaseThread::BaseImplementation
{
public:
   HANDLE mHandle;
};

//******************************************************************************

BaseThread::BaseThread() 
{
   mBaseImplementation = new BaseImplementation;
   mBaseImplementation->mHandle    = 0;
   mThreadPriority  = 0;
   mThreadAffinityMask    = 0;
   mThreadIdealProcessor  = -1;

   mThreadStackSize = 0;
}

//******************************************************************************

BaseThread::~BaseThread() 
{
   delete mBaseImplementation;
}

//******************************************************************************

void* BaseThread::getHandlePtr()
{
   return (void*)&mBaseImplementation->mHandle;
}

//******************************************************************************

void BaseThread::configureThread()
{
   mBaseImplementation->mHandle    = 0;
// mThreadPriority  = 0;
   mThreadStackSize = 0;
}

void BaseThread::setThreadPriorityHigh()
{
   mThreadPriority  = 7;
}

void BaseThread::setThreadPriority(int aThreadPriority)
{
   mThreadPriority  = aThreadPriority;
}

//******************************************************************************

void BaseThread::launchThread()
{
   //---------------------------------------------------------------------------
   // Configure thread parameters, but do not make any OS calls

   configureThread();

   //---------------------------------------------------------------------------
   // Create thread, suspended

   DWORD tThreadFlags = CREATE_SUSPENDED;

   // Create the thread in a suspended state.
   // The following threadFunction() is executed in 
   // the context of the created thread.
   mBaseImplementation->mHandle=CreateThread(
        NULL, 
       (DWORD) mThreadStackSize,
       (LPTHREAD_START_ROUTINE)BaseThread_Execute,
       (LPVOID)this,
       (DWORD)tThreadFlags,
        NULL);
 
   //---------------------------------------------------------------------------
   // Set thread parameters

   if (mThreadPriority != 0)
   {
      if( SetThreadPriority(mBaseImplementation->mHandle,translateThreadPriority(mThreadPriority)) == 0 )
      {
         char tStr[100];
         sprintf(tStr, "Bad Thread Priority %d %d",*((int*)&mBaseImplementation->mHandle),(int)GetLastError());
         halt(tStr);
      }
   }
   if (mThreadAffinityMask != 0)
   {
      if( SetThreadAffinityMask(mBaseImplementation->mHandle,mThreadAffinityMask) == -1 ) throw "Bad Thread Affinity";
   }
   if (mThreadIdealProcessor != -1)
   {
      if( SetThreadIdealProcessor(mBaseImplementation->mHandle,mThreadIdealProcessor) == -1 ) throw "Bad Ideal Processor";
   }

   //---------------------------------------------------------------------------
   // Resume thread, this starts the thread function within the context of
   // the new thread,using configured priority and affinity

   ResumeThread(mBaseImplementation->mHandle);

   //---------------------------------------------------------------------------
   // Wait for semaphore that is signaled in the following
   // threadFunction after the thread init section has executed.

   mThreadInitSem.get();
}

//******************************************************************************
// This is the function that is executed in the context of the created thread.
// It calls a sequence of functions that are overloaded by inheritors.

void BaseThread::threadFunction()
{
   // Thread execution
   try
   {
      // Seed random numbers for this thread
      my_srand();
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
      // Put to the init sem that the launch method is
      // waiting for. The launch method doesn't return
      // until the above two functions have executed.
      mThreadInitSem.put();
      // Run section, overload provided by inheritors 
      threadRunFunction();
      // Exit section, overload provided by inheritors
      threadExitFunction();
   }
   catch (char* aStr)
   {
      // Put to the init sem that the launch method is
      // waiting for
      mThreadInitSem.put();
      // Exception section, overload provided by inheritors
      threadExceptionFunction(aStr);
   }
   catch (...)
   {
      // Put to the init sem that the launch method is
      // waiting for
      mThreadInitSem.put();
      // Exception section, overload provided by inheritors
      threadExceptionFunction("UNKNOWN");
   }
}

//******************************************************************************

void BaseThread::threadTimerInitFunction()
{
}

//******************************************************************************

void BaseThread::threadInitFunction()
{
}

//******************************************************************************

void BaseThread::threadRunFunction()
{
}

//******************************************************************************

void BaseThread::threadExitFunction()
{
}

//******************************************************************************

void BaseThread::threadExceptionFunction(char* aStr)
{
   Prn::print(0,0,"BaseThread::threadExceptionFunction %s",aStr);
}

//******************************************************************************

void BaseThread::threadSleep(int aTicks)
{
   Sleep(aTicks);
}

//******************************************************************************

void BaseThread::forceTerminateThread()
{
   TerminateThread(mBaseImplementation->mHandle,-1);
}

//******************************************************************************

int BaseThread::getThreadPriority()
{
    return GetThreadPriority(mBaseImplementation->mHandle);
}

//******************************************************************************

void BaseThread::waitForThreadTerminate()
{
   WaitForSingleObject(mBaseImplementation->mHandle,INFINITE);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseThreadWithTermFlag::BaseThreadWithTermFlag() 
{
   mTerminateFlag = false;
}

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

int translateThreadPriority(int aThreadPriority)
{
   switch(aThreadPriority)
   {
      case 7: return THREAD_PRIORITY_TIME_CRITICAL;
      case 6: return THREAD_PRIORITY_HIGHEST;
      case 5: return THREAD_PRIORITY_ABOVE_NORMAL;
      case 4: return THREAD_PRIORITY_NORMAL;
      case 3: return THREAD_PRIORITY_BELOW_NORMAL;
      case 2: return THREAD_PRIORITY_LOWEST;
      case 1: return THREAD_PRIORITY_IDLE;
   }
   return THREAD_PRIORITY_NORMAL;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void halt(char* aPrintStr)
{
   printf("HALTING %s\n",aPrintStr);
   while(1);
}

}//namespace
}//namespace

