#ifndef _RISCALLPOINTER_H_
#define _RISCALLPOINTER_H_
/*==============================================================================

Call pointers, they are similar to function pointers, but can be used for 
C++ class  methods.

This code is based on Rich Hickey's 1994 Functors. Everything has been renamed,
out coding standards have been applied, and bind methods have been added.

==============================================================================*/

/*==============================================================================
This is a set of templates that can be used to implement CallPointers,
which are function objects which hold addresses of functions and 
can be called like c-style function pointers. CallPointers act much
in the same way as c-style function pointers, but they can be used
for c-style functions and c++ class  member functions.
 

To use:
 
To instantiate a CallPointer, specify a template that corresponds
to the signature of the function that it will point to.
There are 10 CallPointer templates from which to choose:

CallPointer0
CallPointer1<P1>
CallPointer2<P1,P2>
CallPointer3<P1,P2,P3>
CallPointer4<P1,P2,P3,P4>
CallPointer0wRet<R0>
CallPointer1wRet<P1,R0>
CallPointer2wRet<P1,P2,R0>
CallPointer3wRet<P1,P2,P3,R0>
CallPointer4wRet<P1,P2,P3,P4,R0>

These are parameterized by their args and return value if any. Each has
a default ctor and an operator() with the corresponding signature.

They can be treated and used just like ptr-to-functions.

You can create a CallPointer from:

an object and a pointer-to-member-function.

Each CallPointer has a bind method that can be used to set it to the
address of a member function.
 
myCallPointer.bind(ptr-to-object,ptr-to-member-function)

The CallPointer system is 100% type safe. 

/////////////////////// BEGIN Example 1 //////////////////////////

#include <stdio.h>
#include "risCallPointer.h"

   // define a class  with member function and instantiate it

   class  CThing
   {
   public:
      void mfunction1 (int n1) {printf("CThing::mfunction1 %d\n",n1);}
   };

   CThing  gCThing1;

   // instantiate a CallPointer

   Ris::CallPointer1<int> gCallPointer1;

void amain()
{
   // Bind CallPointer with object + member function and call it

   gCallPointer1.bind(&gCThing1,&CThing::mfunction1);
   gCallPointer1(102);
}

/////////////////////// END of example 1 //////////////////////////

Fragments:

   Ris::CallPointer1<int> call21;
   call21.bind(&gCThing1, &CThing::mfunction1);
   call21.bind(this, &CThing::mfunction1);
   call21.bind(cfunction1);

   Ris::CallPointer1<int> call31(&gCThing1, &CThing::mfunction1);
   Ris::CallPointer1<int> call32(this, &CThing::mfunction1);
   Ris::CallPointer1<int> call33(cfunction1);

==============================================================================*/



//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <string.h>
#include <stddef.h>

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class  BaseCallPtr
{
public:
   bool isValid() {return mObject!=0;}
   void setInvalid() {mObject=0;}

   typedef void (BaseCallPtr::*VoidMethod)();
   enum {MethodSize = 16};

   void *getObject() {return mObject;}
   char *getMethod() {return mMethod;}

protected:

   void* mObject;
   char  mMethod[MethodSize];

   BaseCallPtr()
   :  mObject(0){}

   BaseCallPtr(void* aObject,void* aMethod,size_t aMethodSize)
   {
      mObject = (void*)aObject;
      memset(mMethod,0,MethodSize);
      memcpy(mMethod,aMethod,aMethodSize);
   }
};


//******************************************************************************
//******************************************************************************
//******************************************************************************
// no arg - no return -- CallPtr0

class  CallPtr0 : public BaseCallPtr
{
public:
   CallPtr0(){}

   void operator()()
   {
      mThunk(this);
   }

protected:
   typedef void (*Thunk)(BaseCallPtr*);

   CallPtr0(Thunk aThunk,void* aObject,void* aMethod,size_t aMethodSize)
   :  BaseCallPtr(aObject,aMethod,aMethodSize),mThunk(aThunk){}

private:
   Thunk mThunk;
};

//-----------------------------------------------------------------------------
// translate CallPtr0

template
   <class  Object,
    class  Method>

class  MethodTranslator0 : public CallPtr0
{
public:
   MethodTranslator0(Object* aObject,Method& aMethod)
   :  CallPtr0(thunk,aObject,&aMethod,sizeof(Method)){}

   static void thunk(BaseCallPtr* aCallPtr)
   {
      Object* tObject = (Object*)aCallPtr->getObject();
      Method& tMethod = (*(Method*)(void *)(aCallPtr->getMethod()));
      (tObject->*tMethod)();
   }
};

//-----------------------------------------------------------------------------
// set CallPtr0

template
  <class  ObjectA,
   class  ObjectB>

inline void setCallPtr
  (CallPtr0& aCallPtr,
   ObjectA* aObjectA,
   void (ObjectB::* & aMethodB)())
{
   typedef void (ObjectB::*MethodB)();
   aCallPtr = MethodTranslator0<ObjectA,MethodB>(aObjectA,aMethodB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//no arg - with return -- CallPtr0wRet

template
  <class  R0>

class  CallPtr0wRet : public BaseCallPtr
{
public:
   CallPtr0wRet(){}

   R0 operator()()
   {
      return (mThunk(this));
   }

protected:
   typedef R0 (*Thunk)(BaseCallPtr*);

   CallPtr0wRet(Thunk aThunk,void* aObject,void* aMethod,size_t aMethodSize)
   :  BaseCallPtr(aObject,aMethod,aMethodSize),mThunk(aThunk){}

private:
   Thunk mThunk;
};

//-----------------------------------------------------------------------------
// translate CallPtr0wRet

template
  <class  R0,
   class  Object,
   class  Method>

class  MethodTranslator0wRet : public CallPtr0wRet<R0>
{
public:
   MethodTranslator0wRet(Object* aObject,Method& aMethod)
   :  CallPtr0wRet<R0>(thunk,aObject,&aMethod,sizeof(Method)){}

   static R0 thunk(BaseCallPtr* aCallPtr)
   {
      Object* tObject = (Object *)aCallPtr->getObject();
      Method& tMethod = (*(Method*)(void *)(aCallPtr->getMethod()));
      return ((tObject->*tMethod)());
   }
};

//-----------------------------------------------------------------------------
// set CallPtr0wRet

template
  <class  R0,
   class  ObjectA,
   class  ObjectB>

void setCallPtr
  (CallPtr0wRet<R0>& aCallPtr,
   ObjectA* aObjectA,
   R0 (ObjectB::* & aMethodB)())
{
   typedef R0 (ObjectB::*MethodB)();
   aCallPtr = MethodTranslator0wRet<R0,ObjectA,MethodB>(aObjectA,aMethodB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// one arg - no return -- CallPtr1

template <class  P1>

class  CallPtr1 : public BaseCallPtr
{
public:
   CallPtr1(){}

   void operator()(P1 p1)
   {
      mThunk(this,p1);
   }

protected:
   typedef void (*Thunk)(BaseCallPtr*,P1);

   CallPtr1(Thunk aThunk,void* aObject,void* aMethod,size_t aMethodSize)
   :  BaseCallPtr(aObject,aMethod,aMethodSize),mThunk(aThunk){}

private:
   Thunk mThunk;
};

//-----------------------------------------------------------------------------
// translate CallPtr1

template
  <class  P1,
   class  Object, 
   class  Method>

class  MethodTranslator1 : public CallPtr1<P1>
{
public:
   MethodTranslator1(Object* aObject,Method& aMethod)
   :  CallPtr1<P1>(thunk,aObject,&aMethod,sizeof(Method)){}

   static void thunk(BaseCallPtr* aCallPtr,P1 p1)
   {
      Object* tObject = (Object *)aCallPtr->getObject();
      Method& tMethod = (*(Method*)(void *)(aCallPtr->getMethod()));
      (tObject->*tMethod)(p1);
   }
};

//-----------------------------------------------------------------------------
// set CallPtr1

template
   <class  P1,
    class  ObjectA,
    class  ObjectB>

void setCallPtr
  (CallPtr1<P1>& aCallPtr,
   ObjectA* aObjectA,
   void (ObjectB::* & aMethodB)(P1))
{
   typedef void (ObjectB::*MethodB)(P1);
   aCallPtr = MethodTranslator1<P1,ObjectA,MethodB>(aObjectA,aMethodB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// one arg - with return -- CallPtr1wRet

template
   <class  P1,class  R0>

class  CallPtr1wRet : public BaseCallPtr
{
public:
   CallPtr1wRet(){}
   R0 operator()(P1 p1)
   {
      return (mThunk(this,p1));
   }

protected:
   typedef R0 (*Thunk)(BaseCallPtr*,P1);

   CallPtr1wRet(Thunk aThunk,void* aObject,void* aMethod,size_t aMethodSize)
   :  BaseCallPtr(aObject,aMethod,aMethodSize),mThunk(aThunk){}

private:
   Thunk mThunk;
};

//-----------------------------------------------------------------------------
// translate CallPtr1wRet

template
  <class  P1,class  R0,
   class  Object, 
   class  Method>

class  MethodTranslator1wRet : public CallPtr1wRet<P1,R0>
{
public:
   MethodTranslator1wRet(Object* aObject,Method& aMethod)
   :  CallPtr1wRet<P1,R0>(thunk,aObject,&aMethod,sizeof(Method)){}

   static R0 thunk(BaseCallPtr* aCallPtr,P1 p1)
   {
      Object* tObject = (Object *)aCallPtr->getObject();
      Method& tMethod =  (*(Method*)(void *)(aCallPtr->getMethod()));
      return ((tObject->*tMethod)(p1));
   }
};


//-----------------------------------------------------------------------------
// set CallPtr1wRet

template
  <class  P1,class  R0, 
   class  ObjectA,
   class  ObjectB>

void setCallPtr
  (CallPtr1wRet<P1,R0>& aCallPtr,
   ObjectA* aObjectA,
   R0 (ObjectB::* & aMethodB)(P1))
{
   typedef R0 (ObjectB::*MethodB)(P1);
   aCallPtr = MethodTranslator1wRet<P1,R0,ObjectA,MethodB>(aObjectA,aMethodB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// two args - no return -- CallPtr2

template
  <class  P1,class  P2>

class  CallPtr2 : public BaseCallPtr
{
public:
   CallPtr2(){}

   void operator()(P1 p1,P2 p2)
   {
      mThunk(this,p1,p2);
   }

protected:
   typedef void (*Thunk)(BaseCallPtr*,P1,P2);

   CallPtr2(Thunk aThunk,void* aObject,void* aMethod,size_t aMethodSize)
   :  BaseCallPtr(aObject,aMethod,aMethodSize),mThunk(aThunk){}

private:
   Thunk mThunk;
};

//-----------------------------------------------------------------------------
// translate CallPtr2

template
   <class  P1,class  P2,
    class  Object, 
    class  Method>

class  MethodTranslator2 : public CallPtr2<P1,P2>
{
public:
   MethodTranslator2(Object* aObject,Method& aMethod)
   :  CallPtr2<P1,P2>(thunk,aObject,&aMethod,sizeof(Method)){}

   static void thunk(BaseCallPtr* aCallPtr,P1 p1,P2 p2)
   {
      Object* tObject = (Object *)aCallPtr->getObject();
      Method& tMethod = (*(Method*)(void *)(aCallPtr->getMethod()));
      (tObject->*tMethod)(p1,p2);
   }
};

//-----------------------------------------------------------------------------
// set CallPtr2

template
  <class  P1,class  P2,
   class  ObjectA,
   class  ObjectB>

void setCallPtr
  (CallPtr2<P1,P2>& aCallPtr,
   ObjectA* aObjectA,
   void (ObjectB::* & aMethodB)(P1,P2))
{
   typedef void (ObjectB::*MethodB)(P1,P2);
   aCallPtr = MethodTranslator2<P1,P2,ObjectA,MethodB>(aObjectA,aMethodB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// two args - with return -- CallPtr2wRet

template
  <class  P1,class  P2,class  R0>

class  CallPtr2wRet : public BaseCallPtr
{
public:
   CallPtr2wRet(){}

   R0 operator()(P1 p1,P2 p2)
   {
      return (mThunk(this,p1,p2));
   }

protected:
   typedef R0 (*Thunk)(BaseCallPtr*,P1,P2);

   CallPtr2wRet(Thunk aThunk,void* aObject,void* aMethod,size_t aMethodSize)
   :  BaseCallPtr(aObject,aMethod,aMethodSize),mThunk(aThunk){}

private:
   Thunk mThunk;
};

//-----------------------------------------------------------------------------
// translate CallPtr2wRet

template 
  <class  P1,class  P2,class  R0,
   class  Object, 
   class  Method>

class  MethodTranslator2wRet : public CallPtr2wRet<P1,P2,R0>
{
public:
   MethodTranslator2wRet(Object* aObject,Method& aMethod)
   :  CallPtr2wRet<P1,P2,R0>(thunk,aObject,&aMethod,sizeof(Method)){}

   static R0 thunk(BaseCallPtr* aCallPtr,P1 p1,P2 p2)
   {
      Object* tObject = (Object *)aCallPtr->getObject();
      Method& tMethod = (*(Method*)(void *)(aCallPtr->getMethod()));
      return ((tObject->*tMethod)(p1,p2));
   }
};

//-----------------------------------------------------------------------------
// set CallPtr2wRet

template 
  <class  P1,class  P2,class  R0,
   class  ObjectA,
   class  ObjectB>

void setCallPtr
  (CallPtr2wRet<P1,P2,R0>& aCallPtr,
   ObjectA* aObjectA,
   R0 (ObjectB::* & aMethodB)(P1,P2))
{
   typedef R0 (ObjectB::*MethodB)(P1,P2);
   aCallPtr = MethodTranslator2wRet<P1,P2,R0,ObjectA,MethodB>(aObjectA,aMethodB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// three args - no return -- CallPtr3

template
  <class  P1,class  P2,class  P3>

class  CallPtr3 : public BaseCallPtr
{
public:
   CallPtr3(){}

   void operator()(P1 p1,P2 p2,P3 p3)
   {
      mThunk(this,p1,p2,p3);
   }

protected:
   typedef void (*Thunk)(BaseCallPtr*,P1,P2,P3);

   CallPtr3(Thunk aThunk,void* aObject,void* aMethod,size_t aMethodSize)
   :  BaseCallPtr(aObject,aMethod,aMethodSize),mThunk(aThunk){}

private:
   Thunk mThunk;
};

//-----------------------------------------------------------------------------
// translate CallPtr3

template
  <class  P1,class  P2,class  P3,
   class  Object, 
   class  Method>

class  MethodTranslator3 : public CallPtr3<P1,P2,P3>
{
public:
   MethodTranslator3(Object* aObject,Method& aMethod)
   :  CallPtr3<P1,P2,P3>(thunk,aObject,&aMethod,sizeof(Method)){}

   static void thunk(BaseCallPtr* aCallPtr,P1 p1,P2 p2,P3 p3)
   {
      Object* tObject = (Object *)aCallPtr->getObject();
      Method& tMethod = (*(Method*)(void *)(aCallPtr->getMethod()));
      (tObject->*tMethod)(p1,p2,p3);
   }
};

//-----------------------------------------------------------------------------
// set CallPtr3

template
  <class  P1,class  P2,class  P3,
   class  ObjectA,
   class  ObjectB>

void setCallPtr
  (CallPtr3<P1,P2,P3>& aCallPtr,
   ObjectA* aObjectA,
   void (ObjectB::* & aMethodB)(P1,P2,P3))
{
   typedef void (ObjectB::*MethodB)(P1,P2,P3);
   aCallPtr = MethodTranslator3<P1,P2,P3,ObjectA,MethodB>(aObjectA,aMethodB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// three args - with return -- CallPtr3wRet

template
  <class  P1,class  P2,class  P3,class  R0>

class  CallPtr3wRet : public BaseCallPtr
{
public:
   CallPtr3wRet(){}
   R0 operator()(P1 p1,P2 p2,P3 p3)
   {
      return (mThunk(this,p1,p2,p3));
   }

protected:
   typedef R0 (*Thunk)(BaseCallPtr*,P1,P2,P3);

   CallPtr3wRet(Thunk aThunk,void* aObject,void* aMethod,size_t aMethodSize)
   :  BaseCallPtr(aObject,aMethod,aMethodSize),mThunk(aThunk){}

private:
   Thunk mThunk;
};

//-----------------------------------------------------------------------------
// translate CallPtr3wRet

template
  <class  P1,class  P2,class  P3,class  R0,
   class  Object, 
   class  Method>

class  MethodTranslator3wRet : public CallPtr3wRet<P1,P2,P3,R0>
{
public:
   MethodTranslator3wRet(Object* aObject,Method& aMethod)
   :  CallPtr3wRet<P1,P2,P3,R0>(thunk,aObject,&aMethod,sizeof(Method)){}

   static R0 thunk(BaseCallPtr* aCallPtr,P1 p1,P2 p2,P3 p3)
   {
      Object* tObject = (Object *)aCallPtr->getObject();
      Method& tMethod = (*(Method*)(void *)(aCallPtr->getMethod()));
      return ((tObject->*tMethod)(p1,p2,p3));
   }
};

//-----------------------------------------------------------------------------
// set CallPtr3wRet

template
  <class  P1,class  P2,class  P3,class  R0,
   class  ObjectA,
   class  ObjectB>

void setCallPtr
  (CallPtr3wRet<P1,P2,P3,R0>& aCallPtr,
   ObjectA* aObjectA, 
   R0 (ObjectB::* & aMethodB)(P1,P2,P3))
{
   typedef R0 (ObjectB::*MethodB)(P1,P2,P3);
   aCallPtr = MethodTranslator3wRet<P1,P2,P3,R0,ObjectA,MethodB>(aObjectA,aMethodB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// four args - no return -- CallPtr4

template
  <class  P1,class  P2,class  P3,class  P4>

class  CallPtr4 : public BaseCallPtr
{
public:
   CallPtr4(){}

   void operator()(P1 p1,P2 p2,P3 p3,P4 p4)
   {
      mThunk(this,p1,p2,p3,p4);
   }

protected:
   typedef void (*Thunk)(BaseCallPtr*,P1,P2,P3,P4);

   CallPtr4(Thunk aThunk,void* aObject,void* aMethod,size_t aMethodSize)
   :  BaseCallPtr(aObject,aMethod,aMethodSize),mThunk(aThunk){}

private:
   Thunk mThunk;
};

//-----------------------------------------------------------------------------
// translate CallPtr4

template
  <class  P1,class  P2,class  P3,class  P4,
  class  Object, 
  class  Method>

class  MethodTranslator4 : public CallPtr4<P1,P2,P3,P4>
{
public:
   MethodTranslator4(Object* aObject,Method& aMethod)
   :  CallPtr4<P1,P2,P3,P4>(thunk,aObject,&aMethod,sizeof(Method)){}

   static void thunk(BaseCallPtr* aCallPtr,P1 p1,P2 p2,P3 p3,P4 p4)
   {
      Object* tObject = (Object *)aCallPtr->getObject();
      Method& tMethod = (*(Method*)(void *)(aCallPtr->getMethod()));
      (tObject->*tMethod)(p1,p2,p3,p4);
   }
};

//-----------------------------------------------------------------------------
// set CallPtr4

template
  <class  P1,class  P2,class  P3,class  P4,
   class  ObjectA,
   class  ObjectB>

void setCallPtr(CallPtr4<P1,P2,P3,P4>& aCallPtr,
  ObjectA* aObjectA,
  void (ObjectB::* & aMethodB)(P1,P2,P3,P4))
{
   typedef void (ObjectB::*MethodB)(P1,P2,P3,P4);
   aCallPtr = MethodTranslator4<P1,P2,P3,P4,ObjectA,MethodB>(aObjectA,aMethodB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// four args - with return -- CallPtr4wRet

template
  <class  P1,class  P2,class  P3,class  P4,class  R0>

class  CallPtr4wRet : public BaseCallPtr
{
public:
   CallPtr4wRet(){}
   R0 operator()(P1 p1,P2 p2,P3 p3,P4 p4)
   {
      return (mThunk(this,p1,p2,p3,p4));
   }

protected:
   typedef R0 (*Thunk)(BaseCallPtr*,P1,P2,P3,P4);

   CallPtr4wRet(Thunk aThunk,void* aObject,void* aMethod,size_t aMethodSize)
   :  BaseCallPtr(aObject,aMethod,aMethodSize),mThunk(aThunk){}

private:
   Thunk mThunk;
};

//-----------------------------------------------------------------------------
// translate CallPtr4wRet

template
  <class  P1,class  P2,class  P3,class  P4,class  R0,
   class  Object, 
   class  Method>

class  MethodTranslator4wRet : public CallPtr4wRet<P1,P2,P3,P4,R0>
{
public:
   MethodTranslator4wRet(Object* aObject,Method& aMethod)
   :  CallPtr4wRet<P1,P2,P3,P4,R0>(thunk,aObject,&aMethod,sizeof(Method)){}

   static R0 thunk(BaseCallPtr* aCallPtr,P1 p1,P2 p2,P3 p3,P4 p4)
   {
      Object* tObject = (Object *)aCallPtr->getObject();
      Method& tMethod = (*(Method*)(void *)(aCallPtr->getMethod()));
      return ((tObject->*tMethod)(p1,p2,p3,p4));
   }
};

//-----------------------------------------------------------------------------
// set CallPtr4wRet

template
  <class  P1,class  P2,class  P3,class  P4,class  R0,
   class  ObjectA,
   class  ObjectB>

void setCallPtr
  (CallPtr4wRet<P1,P2,P3,P4,R0>& aCallPtr,
   ObjectA* aObjectA,
   R0 (ObjectB::* & aMethodB)(P1,P2,P3,P4))
{
   typedef R0 (ObjectB::*MethodB)(P1,P2,P3,P4);
   aCallPtr = MethodTranslator4wRet<P1,P2,P3,P4,R0,ObjectA,MethodB>(aObjectA,aMethodB);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
// CallPointer0

class  CallPointer0 : public CallPtr0
{
public:
   CallPointer0(){}

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   CallPointer0(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   void bind(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// CallPointer0wRet

template
  <class  R0>

class  CallPointer0wRet : public CallPtr0wRet<R0>
{
public:
   CallPointer0wRet(){}

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   CallPointer0wRet(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   void bind(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// CallPointer1

template
  <class  P1>

class  CallPointer1 : public CallPtr1<P1>
{
public:
   CallPointer1(){}

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   CallPointer1(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   void bind(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// CallPointer1wRet

template
  <class  P1,class  R0>

class  CallPointer1wRet : public CallPtr1wRet<P1,R0>
{
public:
   CallPointer1wRet(){}

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   CallPointer1wRet(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   void bind(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// CallPointer2

template
  <class  P1,class  P2>

class  CallPointer2 : public CallPtr2<P1,P2>
{
public:
   CallPointer2(){}

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   CallPointer2(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   void bind(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// CallPointer2wRet

template
  <class  P1,class  P2,class  R0>

class  CallPointer2wRet : public CallPtr2wRet<P1,P2,R0>
{
public:
   CallPointer2wRet(){}

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   CallPointer2wRet(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   void bind(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// CallPointer3

template
  <class  P1,class  P2,class  P3>

class  CallPointer3 : public CallPtr3<P1,P2,P3>
{
public:
   CallPointer3(){}

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   CallPointer3(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   void bind(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// CallPointer3wRet

template
  <class  P1,class  P2,class  P3,class  R0>

class  CallPointer3wRet : public CallPtr3wRet<P1,P2,P3,R0>
{
public:
   CallPointer3wRet(){}

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   CallPointer3wRet(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   void bind(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// CallPointer4

template
  <class  P1,class  P2,class  P3,class  P4>

class  CallPointer4 : public CallPtr4<P1,P2,P3,P4>
{
public:
   CallPointer4(){}

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   CallPointer4(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   void bind(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// CallPointer4wRet

template
  <class  P1,class  P2,class  P3,class  P4,class  R0>

class  CallPointer4wRet : public CallPtr4wRet<P1,P2,P3,P4,R0>
{
public:
   CallPointer4wRet(){}

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   CallPointer4wRet(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }

   //------------------------------------------------
   template
     <class  Object,
      class  Method>

   void bind(Object aObject,Method aMethod)
   {
      setCallPtr(*this,aObject,aMethod);
   }
};


}//namespace

#endif
