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
 
class BasePeriodicThread2
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Configuration variables.
   // Some of these are passed to the CreateThread in launch
   // and some are used by threadFunction.
   int    mThreadStackSize;
   int    mThreadPriority;
   int    mThreadSingleProcessor;

   // The processor that was current at the start of the thread
   // run function.
   int mThreadRunProcessor;

   // Thread period milliseconds.
   int mTimerPeriod;

   // Timer count.
   int mTimerCount;

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
   BasePeriodicThread2(); 
   virtual ~BasePeriodicThread2();

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

   // Set the thread processor number and priority.
   void setThreadPriority(Priority aPriority);

   // Wait for the thread to terminate.
   void waitForThreadTerminate();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Return the thread processor number.
   int  getThreadProcessorNumber();

   // Show thread configuration info.
   void showThreadFullInfo();


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


