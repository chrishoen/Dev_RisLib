#ifndef _SOMECLASS3_H_
#define _SOMECLASS3_H_

/*==============================================================================
File: someClass3.h
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

class  BaseInterface3
{
public:

   virtual void interfaceCall3(int aX)=0;
};

template <class Parent>
class  Interface3 : public BaseInterface3
{
public:
   typedef void( Parent::*Call )(int);

   Interface3(
      Parent* tParent,
      Call    tCall)
   :
      mParent(tParent),
      mCall(tCall)
   {}

   
      
   void interfaceCall3(int aX)
   {
      (mParent->*mCall)(aX);
   }

private:
   Parent* mParent;
   Call    mCall;
};

#if 0
class  BaseInterface3
{
public:

   virtual void interfaceCall3(int aX)=0;
};

template <class Parent>
class  Interface3 : public BaseInterface3
{
public:
   typedef void( Parent::*Call )(int);

   Interface3(
      Parent& tParent,
      Call    tCall)
   :
      mParent(tParent),
      mCall(tCall)
   {}

   
      
   void interfaceCall3(int aX)
   {
      (mParent.*mCall)(aX);
   }

private:
   Parent& mParent;
   Call    mCall;
};
#endif
//******************************************************************************
//******************************************************************************
//******************************************************************************

class  Class3
{
public:

   Class3();

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Interface:

   Interface3<Class3>* mMyInterface3;
   void myCall(int aX);
};

//******************************************************************************
// Global instance

#ifdef _SOMECLASS3_CPP_
          Class3 gClass3;
#else
   extern Class3 gClass3;
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

