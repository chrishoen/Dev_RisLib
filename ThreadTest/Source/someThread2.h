#ifndef _SOMETHREAD2_H_
#define _SOMETHREAD2_H_

/*==============================================================================
File: someThread2.h
Description:
QCall test thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsQCall.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class  Thread2 : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   Thread2();

   void threadInitFunction();

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------

   void executeOnTimer(int aCurrentTimeCount);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // QCall, 101:

   Ris::Threads::QCall1<int>   mC101QCall;
   void executeC101 (int aX=0);

   Ris::Threads::QCall1<int>   mC102QCall;
   void executeC102 (int aX=0);

   Ris::Threads::QCall1<int>   mTest1QCall;
   void executeTest1 (int aX=0);
};

//******************************************************************************
// Global instance

#ifdef _SOMETHREAD2_CPP_
          Thread2* gThread2;
#else
   extern Thread2* gThread2;
#endif

//******************************************************************************
}//namespace


#endif

