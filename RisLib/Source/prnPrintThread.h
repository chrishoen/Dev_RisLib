#ifndef _PRNPRINTTHREAD_H_
#define _PRNPRINTTHREAD_H_

//******************************************************************************
//******************************************************************************
//******************************************************************************
/* ----------------------------------------------------------------------------

WHAT:    program PrintBlock class
   
---------------------------------------------------------------------------- */
//******************************************************************************
// imports
//******************************************************************************
#include "risThreadsQCallThread.h"
#include "risThreadsQCall.h"
#include "prnPrint.h"

namespace Prn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class PrintBlock
{
public:
   PrintBlock();
   PrintBlock(char* aString);

   enum {STRING_SIZE = 400};
   char mString[STRING_SIZE];
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class PrintThread : public Ris::Threads::BaseQCallThread
{
public:
   typedef Ris::Threads::BaseQCallThread BaseClass;

   // Constructor
   PrintThread();

   // Configure thread members
   void configure(RedirectCallPointer aRedirectCallPointer);

   // Base class overload
   void configureThread();

   //--------------------------------------------------------------
   // QCall, PrintBlock:

   typedef Ris::Threads::QCall1<Prn::PrintBlock*> PrintBlockQCall;
   PrintBlockQCall mPrintBlockQCall;
   void executeOnPrintBlock (Prn::PrintBlock* aPrintBlock);

   //--------------------------------------------------------------
   // :

   RedirectCallPointer  mRedirectCallPointer;

};

//******************************************************************************

//global instance

#ifdef _PRNPRINTTHREAD_CPP_
         PrintThread gPrintThread;
#else
extern   PrintThread gPrintThread;
#endif

//******************************************************************************
}//namespace
#endif

