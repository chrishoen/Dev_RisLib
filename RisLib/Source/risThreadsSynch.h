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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This encapsulates a binary semaphore.

   class BinarySemaphore
   {
   public:

      BinarySemaphore();            //creates the semaphore
      virtual ~BinarySemaphore();   //deletes the semaphore
      void reset();                 //resets the semaphore

      void put ();                  //put to the semaphore
      bool get (int timeout= -1);   //get from the semaphore, block until timeout,
                                    //return true if no timeout

      int mStatusCode;              //user status code

      void* getHandlePtr();

   protected:
      class Specific;
      Specific* mSpecific;
   };

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This encapsulates a counting semaphore.

   class CountingSemaphore
   {
   public:

      CountingSemaphore();             //creates the semaphore
      CountingSemaphore(int aInitial); //creates the semaphore
      virtual ~CountingSemaphore();    //deletes the semaphore

      void put ();                     //put to the semaphore
      bool get (int timeout= -1);      //get from the semaphore, block until timeout,
                                       //return true if no timeout

      void* getHandlePtr();

   protected:
      class Specific;
      Specific* mSpecific;
   };

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This encapsulates a mutex semaphore.

   class MutexSemaphore
   {
   public:

      MutexSemaphore();               //creates the mutex semaphore
      virtual ~MutexSemaphore();      //deletes the mutex semaphore

      bool lock    (int timeout= -1); // lock the mutex, block until timeout
      void unlock  ();                // unlock the mutex

      void* getHandlePtr();

   protected:
      class Specific;
      Specific* mSpecific;
   };

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
      class Specific;
      Specific* mSpecific;
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

      void lock ();
      void unlock ();

   protected:
      class Specific;
      Specific* mSpecific;
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
      class Specific;
      Specific* mSpecific;
   };
  
}//namespace
}//namespace
#endif


