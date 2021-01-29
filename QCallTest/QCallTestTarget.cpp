//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "QCallTestTarget.h"


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

QCallTestTarget::QCallTestTarget()
{
   mCallQueSize=1000;
   mCallQueue.initialize(mCallQueSize);
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
