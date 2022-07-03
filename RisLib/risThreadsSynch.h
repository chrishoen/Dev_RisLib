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

   // Specific implementation variables.
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

   // Specific implementation variables.
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

   // Reset the semaphore.
   void reset();

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

   // Specific implementation variables.
   class Specific;
   Specific* mSpecific;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor. Create the mutex.
   MutexSemaphore();

   // Destructor. Delete the mutex.
   virtual ~MutexSemaphore();

   // Lock the mutex.
   void lock   ();

   // Unlock the mutex.
   void unlock ();
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

   // Specific implementation variables.
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
   void enter();

   // Leave the critical section.
   void leave();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a named semaphore.

class NamedSemaphore
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Specific implementation variables.
   class Specific;
   Specific* mSpecific;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor. Create the mutex.
   NamedSemaphore(const char* aName);

   // Constructor. default. Initialize the data structure.
   NamedSemaphore();

   // Create the mutex. Call this if using default constructor.
   void initialize(const char* aName);
   bool create(const char* aName);
   bool open(const char* aName);
   void close();

   // Destructor. Delete the mutex.
   virtual ~NamedSemaphore();

   // Get from the semaphore.
   void get();

   // put to the semaphore.
   void put();
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

   // Specific implementation variables.
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
}//namespace
}//namespace


