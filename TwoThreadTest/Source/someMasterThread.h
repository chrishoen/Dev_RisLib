#ifndef _SOMEMASTERTHREAD_H_
#define _SOMEMASTERTHREAD_H_

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
// This is an example master thread that sends commands to a slave thread
// and waits for responses.

class  MasterThread : public Ris::Threads::BaseTwoThread
{
public:
   typedef Ris::Threads::BaseTwoThread BaseClass;

   MasterThread();

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

   // Send command to slave thread
   Ris::Threads::QCall3<int,int,int>   mSendCommandQCall;
   void executeSendCommand (int aId,int aParm1,int aParm2);

   // Send work request to slave thread
   Ris::Threads::QCall1<int>   mSendWorkRequestQCall;
   void executeSendWorkRequest (int aParm1);

   // Receive response from slave thread
   Ris::Threads::QCall1<int>   mResponseQCall;
   void executeResponse (int aId=0);

   // Sequence of interactions with slave thread
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

#ifdef _SOMEMASTERTHREAD_CPP_
          MasterThread* gMasterThread;
#else
   extern MasterThread* gMasterThread;
#endif

//******************************************************************************
}//namespace


#endif

