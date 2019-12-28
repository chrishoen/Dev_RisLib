/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 

#include "my_functions.h"

#include <stdio.h>
#include <assert.h>
#include "risThreadsSynch.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

// Specific implementation variables.
class BinarySemaphore::Specific
{
public:
   HANDLE mHandle;
};

// Constructor. Create the semaphore.
BinarySemaphore::BinarySemaphore()
{
   mSpecific = new Specific;
   mSpecific->mHandle = CreateEvent(NULL,FALSE,FALSE,NULL);
}

// Reset the semaphore.
void BinarySemaphore::reset()
{
   ResetEvent(mSpecific->mHandle);
}

// Destructor. Delete the semaphore.
BinarySemaphore::~BinarySemaphore()
{
   CloseHandle(mSpecific->mHandle);
   delete mSpecific;
}

// Put to the semaphore.
void BinarySemaphore::put()
{
   SetEvent(mSpecific->mHandle);
}

// Get from the semaphore, block until timeout, return true if no timeout.
bool BinarySemaphore::get(int timeout)
{
   return WaitForSingleObject(mSpecific->mHandle,timeout) != WAIT_TIMEOUT ;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Counting semaphore.

// Specific implementation variables.
class CountingSemaphore::Specific
{
public:
   HANDLE mHandle;
};

// Constructor. Create the semaphore.
CountingSemaphore::CountingSemaphore()
{
   mSpecific = new Specific;
   mSpecific->mHandle = CreateSemaphore(NULL,0,1000000,NULL);
}

// Constructor. Create the semaphore.
CountingSemaphore::CountingSemaphore(int aInitial)
{
   mSpecific = new Specific;
   mSpecific->mHandle = CreateSemaphore(NULL,aInitial,1000000,NULL);
}

// Destructor. Delete the semaphore.
CountingSemaphore::~CountingSemaphore()
{
   CloseHandle(mSpecific->mHandle);
   delete mSpecific;
}

// Put to the semaphore.
void CountingSemaphore::put()
{
   ReleaseSemaphore(mSpecific->mHandle,1,NULL);
}

// Get from the semaphore, block until timeout, return true if no timeout.
bool CountingSemaphore::get(int timeout)
{
   switch (WaitForSingleObject(mSpecific->mHandle,timeout))
   {
      case WAIT_OBJECT_0      : return true;  break;
      case WAIT_TIMEOUT       : return false; break;
      default                 : return false; break;
   } 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Mutex semaphore.

// Specific implementation variables.
class MutexSemaphore::Specific
{
public:
   SRWLOCK mSRWLock;
};

// Constructor. Create the mutex.
MutexSemaphore::MutexSemaphore()
{
   mSpecific = new Specific;
   InitializeSRWLock(&mSpecific->mSRWLock); 
}

// Destructor. Delete the mutex.
MutexSemaphore::~MutexSemaphore()
{
   delete mSpecific;
}

// Lock the mutex.
void MutexSemaphore::lock()
{
   AcquireSRWLockExclusive(&mSpecific->mSRWLock);
}

// Unlock the mutex.
void MutexSemaphore::unlock()
{
   ReleaseSRWLockExclusive(&mSpecific->mSRWLock); 
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Named mutex.

// Specific implementation variables.
class NamedMutex::Specific
{
public:
   HANDLE mHandle;
};

// Constructor. Create the mutex.
NamedMutex::NamedMutex(const char* aName)
{
   mSpecific = new Specific;
   initialize(aName);
}

// Constructor. default. Initialize the data structure.
NamedMutex::NamedMutex()
{
   mSpecific = new Specific;
}

// Create the mutex. Call this if using default constructor.
void NamedMutex::initialize(const char* aName)
{
   mSpecific->mHandle = CreateMutex(NULL, FALSE, aName);
}

// Destructor. Delete the mutex.
NamedMutex::~NamedMutex()
{
   CloseHandle(mSpecific->mHandle);
   delete mSpecific;
}

// Lock the mutex.
void NamedMutex::lock()
{
   WaitForSingleObject(mSpecific->mHandle, INFINITE); 
}

// Unlock the mutex.
void NamedMutex::unlock()
{
   ReleaseMutex(&mSpecific->mHandle);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Critical section.

// Specific implementation variables.
class CriticalSection::Specific
{
public:
   CRITICAL_SECTION mCriticalSection; 
};

// Constructor. Create the critical section.
CriticalSection::CriticalSection()
{
   mSpecific = new Specific;
   InitializeCriticalSectionAndSpinCount(&mSpecific->mCriticalSection,0x80000400); 
}

// Destructor. Delete the critical section.
CriticalSection::~CriticalSection()
{
   DeleteCriticalSection(&mSpecific->mCriticalSection);
   delete mSpecific;
}

// Enter the critical section.
void CriticalSection::enter()
{
   EnterCriticalSection(&mSpecific->mCriticalSection); 
}

// Leave the critical section.
void CriticalSection::leave()
{
   LeaveCriticalSection(&mSpecific->mCriticalSection);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

