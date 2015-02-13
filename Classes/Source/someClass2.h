#ifndef _SOMECLASS2_H_
#define _SOMECLASS2_H_

/*==============================================================================
File: someClass2.h
Description:
QCall test Class class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risCallPointer.h"
#include "Callback.h"

namespace Some
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

class  Class2
{
public:

   Class2();

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // CallPointer:

   typedef Ris::CallPointer1<int>   MyCallPointer;
   MyCallPointer mMyCallPointer;

   void myCall(int aX);

   void configure(MyCallPointer tMyCallPointer);

   typedef Utils::CallbackP1<Class2,int> MyCallback;
   MyCallback  mMyCallback;
};

//******************************************************************************
// Global instance

#ifdef _SOMECLASS2_CPP_
          Class2 gClass2;
#else
   extern Class2 gClass2;
#endif

//******************************************************************************
}//namespace

/*==============================================================================
Copyright (C) 2014 Chris Hoen, Hamburg NY.
Legal Notice:  It is forbidden to do anything against me because of this.
Legal Warning: Use this at your own risk, it's software.
Legal Rights:  Other than that, do whatever you want with it.
==============================================================================*/


#endif

