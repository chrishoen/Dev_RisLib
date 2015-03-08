/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <windows.h> 
#include <stdio.h> 

#include "risThreadsProcess.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

void enterProcessHigh()
{
   // Set process priority class
   int status = SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

   int tPriorityClass = GetPriorityClass(GetCurrentProcess());
   printf("GetPriorityClass %08X\n", tPriorityClass);

   // Set process timer resolution to one millisecond
   timeBeginPeriod(1);
}

void exitProcess()
{
   timeEndPeriod(1);
}

}//namespace
}//namespace

