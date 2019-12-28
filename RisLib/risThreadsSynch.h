#pragma once
/*==============================================================================
This file contains classes that encapsulate standard rtos multithreading
synchronization constructs. It supplies events, semaphores, and mutexes.
The purpose of the classes is to wrap the rtos api thread synchronization
calls.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a binary semaphore.

class BinarySemaphore
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Specific implementation variables. pimpl pattern.
   class Specific;
   Specific* mSpecific;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.
   
   // Constructor. Create the semaphore.
   BinarySemaphore();

   // Reset the semaphore.
   void reset();

   // Destructor. Delete the semaphore.
   virtual ~BinarySemaphore();

   // Put to the semaphore.
   void put ();

   // Get from the semaphore, block until timeout, return true if no timeout.
   bool get (int timeout= -1);   
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a counting semaphore.

class CountingSemaphore
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Specific implementation variables. pimpl pattern.
   class Specific;
   Specific* mSpecific;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor. Create the semaphore.
   CountingSemaphore();
   CountingSemaphore(int aIntial);

   // Destructor. Delete the semaphore.
   virtual ~CountingSemaphore();

   // Put to the semaphore.
   void put();

   // Get from the semaphore, block until timeout, return true if no timeout.
   bool get(int timeout = -1);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a mutex semaphore.

class MutexSemaphore
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Specific implementation variables. pimpl pattern.
   class Specific;
   Specific* mSpecific;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor. Create the semaphore.
   MutexSemaphore();

   // Destructor. Delete the semaphore.
   virtual ~MutexSemaphore();

   // Lock the mutex.
   void lock   ();

   // Unlock the mutex.
   void unlock ();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a named mutex semaphore.

class NamedMutex
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Specific implementation variables. pimpl pattern.
   class Specific;
   Specific* mSpecific;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor. Create the mutex.
   NamedMutex(const char* aName);

   // Constructor. default. Initialize the data structure.
   NamedMutex();

   // Create the mutex. Call this if using default constructor.
   void initialize(const char* aName);

   // Destructor. Delete the mutex.
   virtual ~NamedMutex();

   // Lock the mutex.
   void lock();

   // Unlock the mutex.
   void unlock();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a critical section.

class CriticalSection
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Specific implementation variables. pimpl pattern.
   class Specific;
   Specific* mSpecific;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor. Create the critical section.
   CriticalSection();

   // Destructor. Delete the critical section.
   virtual ~CriticalSection();

   // Enter the critical section.
   void enter ();

   // Leave the critical section.
   void leave ();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a condition variable.

class ConditionVariable
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Specific implementation variables. pimpl pattern.
   class Specific;
   Specific* mSpecific;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor. Create the condition variable.
   ConditionVariable();

   // Destructor. Delete the condition variable.
   ~ConditionVariable();

   // Acquire the lock.
   void acquireLock ();

   // Release the lock.
   void releaseLock ();

   // Wait for the condition variable.
   void waitFor ();

   // Wakeup the condition variable.
   void wakeUp (int aPredicate = 1);
};
  
//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


