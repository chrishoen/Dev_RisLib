#ifndef _SOMETHREAD1_H_
#define _SOMETHREAD1_H_

/*==============================================================================
File: someThread1.h
Description:
QCall test thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsQCall.h"
#include "risTimerTester.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class  Thread1 : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   Thread1();

   void configureThread();

   void executeOnTimer(int aTimerCount);


   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // QCall:

   Ris::Threads::QCall1<int>   mF101QCall;
   void executeF101 (int aX=0);

   Ris::Threads::QCall1<int>   mF102QCall;
   void executeF102 (int aX=0);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // QCalls:

   Ris::Threads::QCall1<int>   mTest1QCall;
   void executeTest1 (int aX=0);

   Ris::Threads::QCall1<int>   mTest2QCall;
   void executeTest2 (int aX=0);

   Ris::Threads::QCall1<int>   mTest3QCall;
   void executeTest3 (int aX=0);

   Ris::Threads::QCall2<int,int>   mTest4QCall;
   void executeTest4 (int aNumOfIterations,int aNumOfExecutions);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Members:

   Ris::TimerTester mTimerTester;

   bool mTPFlag;
};

//******************************************************************************
// Global instance

#ifdef _SOMETHREAD1_CPP_
          Thread1* gThread1=0;
#else
   extern Thread1* gThread1;
#endif

//******************************************************************************
}//namespace

#endif

#if 0
/*==============================================================================
Copyright 2013 Christopher Hoen

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#endif

