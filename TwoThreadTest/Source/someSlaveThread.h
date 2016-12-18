#ifndef _SOMESLAVETHREAD_H_
#define _SOMESLAVETHREAD_H_

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsQCallThread.h"
#include "risThreadsTwoThread.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an example slave thread that receives commands from  a master 
// thread and sends responses.

class  SlaveThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   SlaveThread();

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

   // Receive command from master thread
   typedef Ris::Threads::QCall3<int,int,int> CommandQCall;
   CommandQCall   mCommandQCall;
   void executeCommand (int aId,int aParm1,int aParm2);

   // Receive work request from master thread
   typedef Ris::Threads::QCall2<int,Ris::Threads::TwoThreadNotify> WorkRequestQCall;
   WorkRequestQCall   mWorkRequestQCall;
   void executeWorkRequest (int aParm1,Ris::Threads::TwoThreadNotify aCompletionNotify);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Helpers:

   void wait(int aSeconds);
};

//******************************************************************************
// Global instance

#ifdef _SOMESLAVETHREAD_CPP_
          SlaveThread* gSlaveThread;
#else
   extern SlaveThread* gSlaveThread;
#endif

//******************************************************************************
}//namespace


#endif

