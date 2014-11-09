#ifndef _SOMECONTROLLERTHREAD_H_
#define _SOMECONTROLLERTHREAD_H_

/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsTwoThread.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an example controller thread that sends commands to a plant thread
// and waits for responses.

class  ControllerThread : public Ris::Threads::BaseTwoThread
{
public:
   typedef Ris::Threads::BaseTwoThread BaseClass;

   ControllerThread();

   void configureThreads();
   void threadInitFunction();
   void threadExitFunction();

   void executeOnTimer(int aTimerCount);


   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // QCall:

   // Timer tests
   Ris::Threads::QCall1<int>   mTest1QCall;
   void executeTest1 (int aX=0);

   Ris::Threads::QCall1<int>   mTest2QCall;
   void executeTest2 (int aX=0);

   // Send command to plant thread
   Ris::Threads::QCall3<int,int,int>   mSendQCall;
   void executeSend (int aId,int aParm1,int aParm2);

   // Receive response from plant thread
   Ris::Threads::QCall1<int>   mResponseQCall;
   void executeResponse (int aId=0);

   // Sequence of interactions with plant thread
   Ris::Threads::QCall4<int,int,int,int>   mSequenceQCall;
   void executeSequence (int aId,int aIterations,int aCommandTimeout,int aResponseDelay);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Helpers:
   bool mTPFlag;
};

//******************************************************************************
// Global instance

#ifdef _SOMECONTROLLERTHREAD_CPP_
          ControllerThread* gControllerThread;
#else
   extern ControllerThread* gControllerThread;
#endif

//******************************************************************************
}//namespace


#endif

