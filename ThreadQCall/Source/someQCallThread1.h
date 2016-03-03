#ifndef _SOMEQCALLTHREAD1_H_
#define _SOMEQCALLTHREAD1_H_

/*==============================================================================
File: someQCallThread1.h
Description:
QCall test thread class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risThreadsQCallThread.h"

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class  QCallThread1 : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   QCallThread1();

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------

   void executeOnTimer(int aTimeCount);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // QCall:

   Ris::Threads::QCall1<int>   mC101QCall;
   void executeC101 (int aX);

   Ris::Threads::QCall1<int>   mC102QCall;
   void executeC102 (int aX);

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // QCall:

   Ris::Threads::QCall0 mF100QCall;
   void executeF100 ();

   Ris::Threads::QCall1<int> mF101QCall;
   void executeF101 (int aX1);

   Ris::Threads::QCall2<int,double> mF102QCall;
   void executeF102 (int aX1,double aX2);

   Ris::Threads::QCall3<int,double,float> mF103QCall;
   void executeF103 (int aX1,double aX2,float aX3);

   Ris::Threads::QCall4<int,double,float,long long int> mF104QCall;
   void executeF104 (int aX1,double aX2,float aX3,long long int aX4);

   Ris::Threads::QCall5<int,double,float,long long int,double> mF105QCall;
   void executeF105 (int aX1,double aX2,float aX3,long long int aX4,double aX5);

};

//******************************************************************************
// Global instance
       
#ifdef _SOMEQCALLTHREAD1_CPP_
          QCallThread1* gQCallThread1;
#else
   extern QCallThread1* gQCallThread1;
#endif

//******************************************************************************
}//namespace


#endif

