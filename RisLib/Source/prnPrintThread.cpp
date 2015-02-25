//******************************************************************************
//******************************************************************************
//******************************************************************************
/* ----------------------------------------------------------------------------

WHO:     UView.cpp

WHAT:    program PrintBlock class

---------------------------------------------------------------------------- */
//******************************************************************************
// imports
//******************************************************************************

#define  _PRNPRINTTHREAD_CPP_
#include "prnPrintThread.h"

namespace Prn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
PrintBlock::PrintBlock()
{
   mString[0]=0;
}
PrintBlock::PrintBlock(char* aString)
{
   strcpy(mString,aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

PrintThread::PrintThread()
{
   // QCall CallPointers
   mPrintBlockQCall.bind(this,&PrintThread::executeOnPrintBlock);
};

//******************************************************************************
// This sets base thread configuration members

void PrintThread::configure(RedirectCallPointer aRedirectCallPointer)
{
   mRedirectCallPointer = aRedirectCallPointer;
}
void PrintThread::configureThread()
{
   // Set base class configuration members to defaults
   BaseThread::configureThread();

   // Set base class thread priority
// BaseThread::mThreadPriority  = 3;
}

//******************************************************************************

void PrintThread::executeOnPrintBlock (Prn::PrintBlock* aPrintBlock)
{
   // Print string
   if (!mRedirectCallPointer.isValid())
   {
      puts(aPrintBlock->mString);
   }
   else
   {
      mRedirectCallPointer(aPrintBlock->mString);
   }

   // Delete
   delete aPrintBlock;
};
}//namespace

