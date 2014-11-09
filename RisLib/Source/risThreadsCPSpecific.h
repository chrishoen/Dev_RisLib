#ifndef _RISTHREADSCPSPECIFIC_H_
#define _RISTHREADSCPSPECIFIC_H_
/*==============================================================================
Realtime infastructure library Threads

This defines classes that inherit from the thread base classes to provide
specific thread classes that provide call pointers that execute in the
thread run functions. They are used to run threads that don't inherit from 
the thread base classes, which is normally the case.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"
#include "risThreadsQCall.h"
#include "risLogic.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Call pointer types, these are used below

typedef CallPointer0<>      ThreadInitCallPointer;
typedef CallPointer0<>      ThreadExitCallPointer;
typedef CallPointer1<char*> ThreadExceptionCallPointer;
typedef CallPointer1<int>   ThreadExecuteOnTimerCallPointer;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// QCall thread that provides call pointers

class CPSpecificQCallThread : public BaseQCallThread
{
public:

   typedef Ris::Threads::BaseQCallThread BaseClass;

   //--------------------------------------------------------------
   // Constructor

   CPSpecificQCallThread();
   virtual ~CPSpecificQCallThread();

   //--------------------------------------------------------------
   // Thread base class overloads, these call the call pointers
   // listed below, if they have been bound.

   void threadInitFunction(); 
   void threadExitFunction(); 
   void threadExceptionFunction(char* aStr);
   void executeOnTimer(int aTimerCount);

   //--------------------------------------------------------------
   // Associated thread call pointers, these are called by the
   // above thread base overloads, if they are bound. They are
   // bound to functions by the instantiator before the thread
   // is launched. Any that are not bound result in a no op
   // for the thread run function

   ThreadInitCallPointer            mThreadInitCallPointer;
   ThreadExitCallPointer            mThreadExitCallPointer;
   ThreadExceptionCallPointer       mThreadExceptionCallPointer;
   ThreadExecuteOnTimerCallPointer  mThreadExecuteOnTimerCallPointer;
};

}//namespace
}//namespace

#endif


