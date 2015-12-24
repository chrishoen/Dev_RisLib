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

   static const int cTimerPeriod = 10;

   void enterProcessHigh()
   {
      // Set process priority class
      int status = SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
      // Set process timer resolution to one millisecond
      timeBeginPeriod(cTimerPeriod);
   }

   //******************************************************************************

   void exitProcess()
   {
      timeEndPeriod(cTimerPeriod);
   }

}//namespace
}//namespace

#if 0
int tPriorityClass = GetPriorityClass(GetCurrentProcess());
printf("GetPriorityClass %08X\n", tPriorityClass);
#endif
