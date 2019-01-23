/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 
#include <assert.h>

#include "tsThreadServices.h"

#include "my_functions.h"
#include "risThreadsPriorities.h"
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
   mBaseSpecific->mHandle = 0;
   mThreadRunState = 0;
   mThreadPriority = gPriorities.mNormal.mPriority;
   mThreadSingleProcessor = -1;
   mThreadStackSize = 0;
   mThreadRunProcessor = -1;


   // Create this now in the thread context of the thread creator.
   // It will be copied to the thread local storage variable at the
   // start of the thread run function.
   mThreadLocal = new TS::ThreadLocal;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseThread::~BaseThread() 
{
   delete mBaseSpecific;
   delete mThreadLocal;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the thread services thread name in the thread local storage.

void BaseThread::setThreadName(const char* aThreadName)
{
   mThreadLocal->setThreadName(aThreadName);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the thread services print level in the thread local storage.

void BaseThread::setThreadPrintLevel(TS::PrintLevel aPrintLevel)
{
   mThreadLocal->mPrintLevel = aPrintLevel;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::setThreadPriority(Priority aPriority)
{
   mThreadSingleProcessor = aPriority.mProcessor;
   mThreadPriority = aPriority.mPriority;
}

void BaseThread::setThreadPriorityLow()
{
   mThreadSingleProcessor = gPriorities.mLow.mProcessor;
   mThreadPriority = gPriorities.mLow.mPriority;
}

void BaseThread::setThreadPriorityNormal()
{
   mThreadSingleProcessor = gPriorities.mNormal.mProcessor;
   mThreadPriority = gPriorities.mNormal.mPriority;
}

void BaseThread::setThreadPriorityHigh()
{
   mThreadSingleProcessor = gPriorities.mHigh.mProcessor;
   mThreadPriority = gPriorities.mHigh.mPriority;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::launchThread()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   TS::print(1, "");
   TS::print(1, "launchThread %s", mThreadLocal->mThreadName);

   // Set the run state.
   mThreadRunState = cThreadRunState_Launching;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Create thread, suspended.

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
   //***************************************************************************
   //***************************************************************************
   // Set thread parameters.

   if( SetThreadPriority(mBaseSpecific->mHandle,mThreadPriority) == 0 )
   {
      char tStr[100];
      sprintf(tStr, "Bad Thread Priority %d %d %d",mThreadPriority,*((int*)&mBaseSpecific->mHandle),(int)GetLastError());
      halt(tStr);
   }

   if (mThreadSingleProcessor != -1)
   {
      int tAffinityMask = 1 << mThreadSingleProcessor;
      if( SetThreadAffinityMask(mBaseSpecific->mHandle,tAffinityMask) == -1 ) throw "Bad Thread Affinity";
      if( SetThreadIdealProcessor(mBaseSpecific->mHandle,mThreadSingleProcessor) == -1 ) throw "Bad Ideal Processor";
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Resume thread, this starts the thread function within the context of
   // the new thread,using configured priority and affinity.

   ResumeThread(mBaseSpecific->mHandle);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Wait for the thread init function to complete.

   mThreadInitSem.get();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the function that is executed in the context of the created thread.
// It calls a sequence of functions that are overloaded by inheritors.

void BaseThread::threadFunction()
{
   // Set the thread local storage pointer to the address of the
   // thread local storage object.
   TS::setThreadLocal(mThreadLocal);
   TS::print(1, "threadFunction BEGIN");

   // Set the processor that was current at the start of the thread
   // run function.
   mThreadRunProcessor = getThreadProcessorNumber();

   // Thread execution
   try
   {
      // Seed thread random number.
      my_srand();
      // This is used by inheritors to initialize resources. This should be
      // overloaded by thread base classes and not by thread user classes.
      mThreadRunState = cThreadRunState_InitR;
      threadResourceInitFunction();
      // Initialization section, overload provided by inheritors
      // It is intended that this will be overloaded by 
      // inheriting thread base classes and by inheriting user classes
      TS::print(1, "threadInitFunction");
      mThreadRunState = cThreadRunState_InitF;
      threadInitFunction();
      // Post to the thread init semaphore.
      mThreadInitSem.put();
      // Run section, overload provided by inheritors 
      TS::print(1, "threadRunFunction");
      mThreadRunState = cThreadRunState_Running;
      threadRunFunction();
      // Exit section, overload provided by inheritors
      TS::print(1, "threadExitFunction");
      mThreadRunState = cThreadRunState_ExitF;
      threadExitFunction();
      // This is used by inheritors to finalize resources. This should be
      // overloaded by thread base classes and not by thread user classes.
      mThreadRunState = cThreadRunState_ExitR;
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

   // Set the run state.
   mThreadRunState = cThreadRunState_Terminated;

   TS::print(1, "threadFunction END");
   // Zero the thread local storage pointer.
   TS::setThreadLocal(0);
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

void BaseThread::waitForThreadTerminate()
{
   TS::print(1, "waitForThreadTerminate BEGIN %s",mThreadLocal->mThreadName);
   WaitForSingleObject(mBaseSpecific->mHandle,INFINITE);
   TS::print(1, "waitForThreadTerminate END   %s", mThreadLocal->mThreadName);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::shutdownThreadPrologue()
{
   TS::print(1, "");
   TS::print(1, "shutdownThread %s", mThreadLocal->mThreadName);
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
   shutdownThreadPrologue();
   mTerminateFlag = true;
   waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThreadWithTermSem::shutdownThread()
{
   shutdownThreadPrologue();
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

int BaseThread::getThreadPriority()
{
   return GetThreadPriority(mBaseSpecific->mHandle);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int BaseThread::getThreadProcessorNumber()
{
   return GetCurrentProcessorNumber();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::showThreadFullInfo()
{
   printf("ThreadInfo>>>>>>>>>>>>>>>>>>>>>>>>>>BEGIN %s\n", mThreadLocal->mThreadName);

   unsigned tPriorityClass = GetPriorityClass(GetCurrentProcess());
   int tThreadPriority = GetThreadPriority(mBaseSpecific->mHandle);
   int tCurrentProcessorNumber = GetCurrentProcessorNumber();

   unsigned long long tProcessAffinityMask = 0;
   unsigned long long tSystemAffinityMask = 0;

   GetProcessAffinityMask(
      GetCurrentProcess(),
      &tProcessAffinityMask,
      &tSystemAffinityMask);

   printf("PriorityClass           %8d\n", tPriorityClass);
   printf("ThreadPriority          %8d\n", tThreadPriority);
   printf("SystemAffinityMask      %8X\n", (unsigned)tSystemAffinityMask);
   printf("ProcessAffinityMask     %8X\n", (unsigned)tProcessAffinityMask);
   printf("SingleProcessor         %8d\n", mThreadSingleProcessor);
   printf("CurrentProcessorNumber  %8d\n", tCurrentProcessorNumber);

   printf("ThreadInfo<<<<<<<<<<<<<<<<<<<<<<<<<<END\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::showThreadInfo()
{
   int tThreadPriority = getThreadPriority();

   TS::print(0, "ThreadInfo %-20s %1d %3d %-8s %1d %1d",
      mThreadLocal->mThreadName,
      mThreadRunProcessor,
      tThreadPriority,
      asStringThreadRunState(),
      mThreadLocal->mPrintLevel.mOutLevel,
      mThreadLocal->mPrintLevel.mLogLevel);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* BaseThread::asStringThreadRunState()
{
   switch (mThreadRunState)
   {
   case cThreadRunState_Launching:  return  "launching";
   case cThreadRunState_InitR:      return  "initR";
   case cThreadRunState_InitF:      return  "initF";
   case cThreadRunState_Running:    return  "running";
   case cThreadRunState_ExitF:      return  "exitF";
   case cThreadRunState_ExitR:      return  "exitR";
   case cThreadRunState_Terminated: return  "termed";
   default: return "UNKNOWN";
   }
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

