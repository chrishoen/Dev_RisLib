#pragma once
/*==============================================================================
Base thread classes
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

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
 
class BaseDeadlineThread
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Earliest deadline first variables.
   // runtime <= deadline <= period.
   int mThreadEDFRunTimeUs;
   int mThreadEDFDeadlineUs;
   int mThreadEDFPeriodUs;

   // Timer count.
   int mTimerCount;

   // Timer count.
   int mTimerPeriod;

   // If true then terminate the thread loop.
   bool mTerminateFlag;

   // Pimpl pattern. Used to hide details of the operating system specific
   // variables, like the thread handle, from the .h file so that this
   // include file can be complied by different compliers. The class is
   // defined in the .cpp file, where there is a different version for
   // different compilers.
   class BaseSpecific;
   BaseSpecific* mBaseSpecific;


   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   BaseDeadlineThread(); 
   virtual ~BaseDeadlineThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Launch the thread.
   virtual void launchThread(); 

   // Thread function.
   void threadFunction();

   // Shutdown the thread
   void shutdownThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Wait for the thread to terminate.
   void waitForThreadTerminate();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Set the thread processor number and priority.
   void setThreadPriority(Priority aPriority) {}

   // Return the thread processor number.
   int  getThreadProcessorNumber() { return -1; }

   // Show thread configuration info.
   void showThreadFullInfo() {}


   void showThreadInfo() {}
   void setThreadName(const char*) {}
   void setThreadPrintLevel(int) {}

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Inheritor overloads.

   // An overload of this is supplied by the inheritor.
   // It is called periodically by the threadRunFunction.
   virtual void executeOnTimer(int aTimerCount) = 0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


