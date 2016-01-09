#ifndef _RISTHREADSSYNCH_H_
#define _RISTHREADSSYNCH_H_
/*==============================================================================

Realtime infastructure library Threads

This file contains classes that encapsulate standard rtos multithreading
synchronization constructs. It supplies events, semaphores, and threads.
The purpose of the classes is to wrap the rtos api thread synchronization
calls.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "ris_priorities.h"

namespace Ris
{
namespace Threads
{

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This encapsulates a critical section.

   class CriticalSection
   {
   public:

      CriticalSection();
      virtual ~CriticalSection();

      void enter ();
      void leave ();

   protected:
      class Implementation;
      Implementation* mImplementation;
   };

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This encapsulates a slim reader/writer lock.

   class SlimLock
   {
   public:

      SlimLock();
      virtual ~SlimLock();

      void acquire ();
      void release ();

      void lock ();
      void unlock ();

   protected:
      class Implementation;
      Implementation* mImplementation;
   };

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This encapsulates a condition variable

   class ConditionVariable
   {
   public:

      ConditionVariable();
     ~ConditionVariable();

      void acquireLock ();
      void releaseLock ();

      void waitFor ();
      void wakeUp (int aPredicate = 1);

   protected:
      class Implementation;
      Implementation* mImplementation;
   };

   //******************************************************************************
   // This encapsulates an alertable binary semaphore.

   class AlertableSemaphore
   {
   public:

      AlertableSemaphore();
      virtual ~AlertableSemaphore();

      void put ();
      bool get (int timeout= -1);

   protected:
      class Implementation;
      Implementation* mImplementation;
   };


   
}//namespace
}//namespace
#endif


