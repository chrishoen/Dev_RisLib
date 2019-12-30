/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 
//#include "synchapi.h"

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
   InitializeCriticalSectionAndSpinCount(&mSpecific->mCriticalSection, 0x80000400);
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
   printf("NamedMutex::create %s %lld %d\n",
      aName, (long long)mSpecific->mHandle, GetLastError());
   if (mSpecific->mHandle == 0)
   {
      printf("NamedMutex::initialize error101\n");
      return;
   }
   return;
   if (GetLastError() == ERROR_ALREADY_EXISTS)
   {
      printf("NamedMutex::initialize already exists\n");
      if (mSpecific->mHandle) CloseHandle(mSpecific->mHandle);
      mSpecific->mHandle = OpenMutex(MUTEX_ALL_ACCESS, FALSE, aName);
      printf("NamedMutex::open  %s %lld %d\n",
         aName, (long long)mSpecific->mHandle, GetLastError());
   }
}

bool NamedMutex::create(const char* aName)
{
   mSpecific->mHandle = CreateMutexA(NULL, FALSE, aName);
   printf("NamedMutex::create %s %lld %d\n",
      aName, (long long)mSpecific->mHandle, GetLastError());
   return mSpecific->mHandle != 0;
}

bool NamedMutex::open(const char* aName)
{
   mSpecific->mHandle = OpenMutex(MUTEX_ALL_ACCESS, FALSE, aName);
   printf("NamedMutex::open  %s %lld %d\n",
      aName, (long long)mSpecific->mHandle, GetLastError());
   return mSpecific->mHandle != 0;
}

// Destructor. Delete the mutex.
NamedMutex::~NamedMutex()
{
   printf("NamedMutex::destructor  %lld\n",(long long)mSpecific->mHandle);
   CloseHandle(mSpecific->mHandle);
   delete mSpecific;
}

// close the mutex.
void NamedMutex::close()
{
   printf("NamedMutex::close  %lld\n", (long long)mSpecific->mHandle);
   CloseHandle(mSpecific->mHandle);
}

// Lock the mutex.
void NamedMutex::lock()
{
   int tRet = WaitForSingleObject(mSpecific->mHandle, INFINITE);
   if (tRet != 0)
   {
      printf("NamedMutex::WaitForSingleObject error  %lld %d %x\n",
         (long long)mSpecific->mHandle, GetLastError(), tRet);
   }
}

// Unlock the mutex.
void NamedMutex::unlock()
{
   if (ReleaseMutex(&mSpecific->mHandle) == 0)
   {
      printf("NamedMutex::ReleaseMutex error  %lld %d\n",
         (long long)mSpecific->mHandle, GetLastError());
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Named semaphore.

// Specific implementation variables.
class NamedSemaphore::Specific
{
public:
   HANDLE mHandle;
};

// Constructor. Create the semaphore.
NamedSemaphore::NamedSemaphore(const char* aName)
{
   mSpecific = new Specific;
   initialize(aName);
}

// Constructor. default. Initialize the data structure.
NamedSemaphore::NamedSemaphore()
{
   mSpecific = new Specific;
}

// Create the semaphore. Call this if using default constructor.
void NamedSemaphore::initialize(const char* aName)
{
   mSpecific->mHandle = CreateSemaphore(NULL, 0, 100000, aName);
   printf("NamedSemaphore::create %s %lld %d\n",
      aName, (long long)mSpecific->mHandle, GetLastError());
   if (mSpecific->mHandle == 0)
   {
      printf("NamedSemaphore::initialize error101\n");
      return;
   }
}

bool NamedSemaphore::create(const char* aName)
{
// mSpecific->mHandle = CreateSemaphore(NULL, 0, 100000, aName);
   mSpecific->mHandle = CreateSemaphore(NULL, 1, 1, aName);
   printf("NamedSemaphore::create %s %lld %d\n",
      aName, (long long)mSpecific->mHandle, GetLastError());
   return mSpecific->mHandle != 0;
}

bool NamedSemaphore::open(const char* aName)
{
   mSpecific->mHandle = OpenSemaphore(MUTEX_ALL_ACCESS, FALSE, aName);
   printf("NamedSemaphore::open  %s %lld %d\n",
      aName, (long long)mSpecific->mHandle, GetLastError());
   return mSpecific->mHandle != 0;
}

// Destructor. Delete the semaphore.
NamedSemaphore::~NamedSemaphore()
{
   printf("NamedSemaphore::destructor  %lld\n", (long long)mSpecific->mHandle);
   CloseHandle(mSpecific->mHandle);
   delete mSpecific;
}

// close the semaphore.
void NamedSemaphore::close()
{
   printf("NamedSemaphore::close  %lld\n", (long long)mSpecific->mHandle);
   CloseHandle(mSpecific->mHandle);
}

// Lock the semaphore.
void NamedSemaphore::get()
{
   int tRet = WaitForSingleObject(mSpecific->mHandle, INFINITE);
   if (tRet != 0)
   {
      printf("NamedSemaphore::WaitForSingleObject error  %lld %d %x\n",
         (long long)mSpecific->mHandle, GetLastError(), tRet);
   }
}

// Unlock the semaphore.
void NamedSemaphore::put()
{
   if (ReleaseSemaphore(mSpecific->mHandle, 1, NULL) == 0)
   {
      printf("NamedSemaphore::ReleaseSemaphore error  %lld %d\n",
         (long long)mSpecific->mHandle, GetLastError());
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

