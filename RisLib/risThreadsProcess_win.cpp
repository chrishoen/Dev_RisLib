//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 

#include "risThreadsProcess.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

static int mTimerPeriod = 50;

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

void enterProcessHigh()
{
   // Set process priority class.
   int status = SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

   // Set process timer resolution.
   timeBeginPeriod(mTimerPeriod);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void exitProcess()
{
   // Restore the  process timer resolution.
   timeEndPeriod(100);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void setProcessAffinityMask(unsigned aMask)
{
   // Convert.
   DWORD_PTR tMask = (DWORD_PTR)aMask;

   // Set the process affinity mask.
   SetProcessAffinityMask(GetCurrentProcess(), aMask);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void showCurrentThreadInfo()
{
   unsigned tPriorityClass = GetPriorityClass(GetCurrentProcess());
   int tThreadPriority = GetThreadPriority(GetCurrentThread());
   int tCurrentProcessorNumber = GetCurrentProcessorNumber();

   TS::print(1, "");
   TS::print(0, "ThreadInfo %-20s %1d %3d %-8s %1d %1d $  %3d",
      "main",
      tCurrentProcessorNumber,
      tThreadPriority,
      "running",
      TS::tls()->mPrintLevel.mOutLevel,
      TS::tls()->mPrintLevel.mLogLevel,
      tPriorityClass);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

#if 0
int tPriorityClass = GetPriorityClass(GetCurrentProcess());
printf("GetPriorityClass %08X\n", tPriorityClass);
#endif
