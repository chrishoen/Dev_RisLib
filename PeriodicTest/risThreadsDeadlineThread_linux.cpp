/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <functional>
#include <sys/resource.h>

//******************************************************************************
//******************************************************************************
//******************************************************************************

#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <sys/syscall.h>
#include <pthread.h>

/* XXX use the proper syscall numbers */
#ifdef __x86_64__
#define __NR_sched_setattr		314
#define __NR_sched_getattr		315
#endif

#ifdef __i386__
#define __NR_sched_setattr		351
#define __NR_sched_getattr		352
#endif

#ifdef __arm__
#define __NR_sched_setattr		380
#define __NR_sched_getattr		381
#endif

static volatile int done;

struct sched_attr {
   __u32 size;

   __u32 sched_policy;
   __u64 sched_flags;

   /* SCHED_NORMAL, SCHED_BATCH */
   __s32 sched_nice;

   /* SCHED_FIFO, SCHED_RR */
   __u32 sched_priority;

   /* SCHED_DEADLINE (nsec) */
   __u64 sched_runtime;
   __u64 sched_deadline;
   __u64 sched_period;
};

int sched_setattr(pid_t pid,
   const struct sched_attr* attr,
   unsigned int flags)
{
   return syscall(__NR_sched_setattr, pid, attr, flags);
}

int sched_getattr(pid_t pid,
   struct sched_attr* attr,
   unsigned int size,
   unsigned int flags)
{
   return syscall(__NR_sched_getattr, pid, attr, size, flags);
}


#define gettid() syscall(__NR_gettid)

#define SCHED_DEADLINE	6

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "my_functions.h"
#include "prnPrint.h"
#include "risNanoConvert.h"

#include "risThreadsDeadlineThread.h"

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

static void* BaseDeadlineThread_Execute (void* argument)
{
   BaseDeadlineThread* someThread = (BaseDeadlineThread*)argument;
   someThread->threadFunction();
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseDeadlineThread::BaseSpecific
{
public:
   pthread_t mHandle;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseDeadlineThread::BaseDeadlineThread() 
{
   mBaseSpecific = new BaseSpecific;
   mBaseSpecific->mHandle = 0;
   mTimerCount;
   mTerminateFlag = false;
   mThreadEDFRunTimeUs = 0;
   mThreadEDFDeadlineUs = 0;
   mThreadEDFPeriodUs = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseDeadlineThread::~BaseDeadlineThread() 
{
   delete mBaseSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

static void chkerror(int aRet, const char* aLabel)
{
   if (aRet >= 0)return;
   printf("FAIL %s %d\n", aLabel,aRet);
   exit(1);
}

void BaseDeadlineThread::launchThread()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Create the thread. The following threadFunction will execute
   // in the context of the created thread.

   int ret;
   ret = pthread_create(
      &mBaseSpecific->mHandle,
      0,
      &BaseDeadlineThread_Execute,
      (void*)this);
   chkerror(ret, "pthread_create");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the function that is executed in the context of the created thread.
// It calls a sequence of functions that are overloaded by inheritors.

void BaseDeadlineThread::threadFunction()
{
   int tRet = 0;

   // Set the scheduler.
   struct sched_attr tSchedAttr;

   tSchedAttr.size = sizeof(tSchedAttr);
   tSchedAttr.sched_flags = 0;
   tSchedAttr.sched_nice = 0;
   tSchedAttr.sched_priority = 0;

   tSchedAttr.sched_policy = SCHED_DEADLINE;
   tSchedAttr.sched_runtime  = mThreadEDFRunTimeUs * 1000;
   tSchedAttr.sched_deadline = mThreadEDFDeadlineUs * 1000;
   tSchedAttr.sched_period   = mThreadEDFPeriodUs * 1000;

   tRet = sched_setattr(0, &tSchedAttr, 0);
   chkerror(tRet, "threadFunction sched_setscheduler");

   // Loop until thread terminate.
   while (!mTerminateFlag)
   {
      // Call the inheritor's timer handler.
      executeOnTimer(mTimerCount++);
      // Indicate that execution is finished.
      sched_yield();
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseDeadlineThread::waitForThreadTerminate()
{
   pthread_join(mBaseSpecific->mHandle,NULL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseDeadlineThread::shutdownThread()
{
   mTerminateFlag = true;
   pthread_join(mBaseSpecific->mHandle, NULL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
