#ifndef _SOMEAPCTHREAD_H_
#define _SOMEAPCTHREAD_H_

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risPortableTypes.h"
#include "risCallPointer.h"
#include "risContainers.h"
#include "risThreads.h"

#include "risThreads.h"
#include "risThreadsApcTimer.h"
#include "risThreadsSynch.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class ApcThread : public Ris::Threads::BaseThread
{
public:
   typedef Ris::Threads::BaseThread BaseClass;

   ApcThread();

   //--------------------------------------------------------------
   //Base class overloads.

   void threadInitFunction();
   void threadRunFunction();
   void threadExitFunction();
   void shutdownThread();

   //Termination 
   Ris::Threads::AlertableSemaphore   mTerminateSem;
   bool                               mTerminateFlag;

   //--------------------------------------------------------------
   // This enqueues an APC to the threads APC queue

   void enqueueApc();

   //--------------------------------------------------------------
   // This executes on the above APC

   void executeOnApc();

   //--------------------------------------------------------------
   // Timer

   Ris::Threads::ApcTimer mApcTimer;
   int                    mTimerPeriod;

   // This is called periodically by the apc timer
   // Inheritors provide an overload for this.
   void executeOnTimer(int aCurrentTimeCount);
};

//******************************************************************************
// Global instance

#ifdef _SOMEAPCTHREAD_CPP_
          ApcThread* gApcThread;
#else
   extern ApcThread* gApcThread;
#endif

//******************************************************************************

}//namespace

#endif

