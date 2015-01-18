#ifndef _SOMEPLANTTHREAD_H_
#define _SOMEPLANTTHREAD_H_

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsQCallThread.h"
#include "risThreadsQCall.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an example plant thread that receives commands from  a controller 
// thread and sends responses.

class  PlantThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   PlantThread();

   void configureThreads();
   void threadInitFunction();

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------

   void executeOnTimer(int aCurrentTimeCount);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // QCalls:

   // Receive command from controller thread
   Ris::Threads::QCall3<int,int,int>   mCommandQCall;
   void executeCommand (int aId,int aParm1,int aParm2);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Helpers:

   void wait(int aSeconds);
};

//******************************************************************************
// Global instance

#ifdef _SOMEPLANTTHREAD_CPP_
          PlantThread* gPlantThread;
#else
   extern PlantThread* gPlantThread;
#endif

//******************************************************************************
}//namespace


#endif

