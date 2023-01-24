/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include <signal.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

#include "risThreadsSynch.h"
#include "risThreadsProcess.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

static int mTimerPeriod = 10;

void setProcessTimerResolution(int aTimerPeriod)
{
   mTimerPeriod = aTimerPeriod;
}

int  getProcessTimerResolution()
{
   return mTimerPeriod;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

static int latency_target_fd = -1;
static int latency_target_value = 0;

static void set_latency_target(void)
{
   struct stat s;
	int err;

	err = stat("/dev/cpu_dma_latency", &s);
	if (err == -1) {
		//printf("WARN: stat /dev/cpu_dma_latency failed\n");
		return;
	}

	errno = 0;
	latency_target_fd = open("/dev/cpu_dma_latency", O_RDWR);
	if (latency_target_fd == -1) {
		//printf("WARN: open /dev/cpu_dma_latency\n");
		return;
	}

	errno = 0;
	err = write(latency_target_fd, &latency_target_value, 4);
	if (err < 1) {
		//printf("# error setting cpu_dma_latency to %d\n", latency_target_value);
		close(latency_target_fd);
		return;
	}
	//printf("# /dev/cpu_dma_latency set to %dus\n", latency_target_value);
}

void enterProcessHigh()
{
   set_latency_target();

   int tRet = 0;
   int tMaxPriority = sched_get_priority_max(SCHED_FIFO);
   int tMinPriority = sched_get_priority_min(SCHED_FIFO);

   struct rlimit tRLimit;
   tRLimit.rlim_cur = tMinPriority;
   tRLimit.rlim_max = tMaxPriority;
   tRet = setrlimit(RLIMIT_RTPRIO, &tRLimit);
   if (tRet)
   {
      printf("setrlimit ERROR  %d\n", errno);
   }
}

void exitProcess()
{
   close(latency_target_fd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void setProcessAffinityMask(unsigned aMask)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void showCurrentThreadInfo()
{
   int tCurrentProcessorNumber = sched_getcpu();;

   sched_param param;
   int policy;
   pthread_getschedparam(pthread_self(), &policy, &param);
   int tThreadPriority = param.sched_priority;

   printf("ThreadInfo %-20s %2d %3d %-8s\n",
      "main",
      tCurrentProcessorNumber,
      tThreadPriority,
      "running");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

Ris::Threads::BinarySemaphore rTerminateSem;

void sig_handler(int signum)
{
   // Post to the termination semaphore.
   printf("sig_handler %d\n", signum);
   rTerminateSem.put();
}

void postTermination()
{
   rTerminateSem.put();
}

void waitForTermination()
{
   // Register signal handler.
   signal(SIGINT, sig_handler);
   signal(SIGTERM, sig_handler);
   signal(SIGKILL, sig_handler);

   // Wait for the termination semaphore.
   printf("WAIT FOR TERMINATE\n");
   rTerminateSem.get();
   printf("TERMINATING\n");
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

