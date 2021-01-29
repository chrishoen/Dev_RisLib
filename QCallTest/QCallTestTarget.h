#pragma once

/*==============================================================================

 
==============================================================================*/

//******************************************************************************
//******************************************************************************

#include "risThreadsQCall.h"
#include "risLCPointerQueue.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

class QCallTestTarget : public Ris::Threads::BaseThread,public BaseQCallTarget
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Pointer queue that contains pointers to qcalls.
   // QCall invocations enqueue qcalls to this queue.
   // This thread dequeues qcalls from it.
   Ris::LCPointerQueue mCallQueue;

   // Call queue size. Inheritors can set this in their constructors.
   int mCallQueSize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   QCallTestTarget();
  ~QCallTestTarget();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, qcall target overloads:

   // Process the call queue.
   void doProcessCallQueue();

   // Try to write a qcall to the to the target queue. Return true if
   // successful. This is called by qcall invocations to enqueue a qcall.
   // It writes to the call queue and posts to the waitable semaphore, which
   // then wakes up the thread run function to process the call queue.
   bool tryWriteQCall(Ris::Threads::BaseQCall* aQCall) override;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
