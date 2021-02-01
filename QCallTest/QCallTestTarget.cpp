//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#define _QCALLTESTTARGET_CPP_
#include "QCallTestTarget.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

QCallTestTarget::QCallTestTarget()
{
   // Initialize base class variables.
   mCallQueSize = 1000;
   mCallQueue.initialize(mCallQueSize);

   // Initialize qcalls.
   mTest1QCall.bind(this, &QCallTestTarget::executeTest1);

   // Initialize member variables.
   mTPFlag = true;
}

QCallTestTarget::~QCallTestTarget()
{
   mCallQueue.finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Process the call queue.

void QCallTestTarget::doProcessCallQueue()
{
   // Try to read a qcall from the queue.
   if (Ris::Threads::BaseQCall* tQCall = (Ris::Threads::BaseQCall*)mCallQueue.tryRead())
   {
      // Execute the qcall.
      tQCall->execute();
      // Delete the qcall.
      delete tQCall;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Try to write a qcall to the to the target queue. Return true if
// successful. This is called by qcall invocations to enqueue a qcall.
// It writes to the call queue and posts to the waitable semaphore, which
// then wakes up the thread run function to process the call queue.

bool QCallTestTarget::tryWriteQCall(Ris::Threads::BaseQCall* aQCall)
{
   // Try to write to the call queue.
   if (!mCallQueue.tryWrite(aQCall))
   {
      // The write was not successful.
      return false;
   }

   // Successful.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test function. This is bound to the qcall.

void QCallTestTarget::executeTest1(int aCode)
{
   if (mTPFlag)
   {
      Prn::print(Prn::View11, "executeTest1 %d", aCode);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
