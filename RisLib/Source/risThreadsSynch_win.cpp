/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h> 

#include "my_functions.h"

#include <stdio.h>
#include <assert.h>
#include "risThreadsSynch.h"

#pragma unmanaged

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BinarySemaphore::Implementation
{
public:
   HANDLE mHandle;
};

//******************************************************************************

BinarySemaphore::BinarySemaphore() 
{
   mImplementation = new Implementation;
   mImplementation->mHandle = CreateEvent(NULL,FALSE,FALSE,NULL);
   mStatusCode=0;
}

//******************************************************************************

BinarySemaphore::~BinarySemaphore() 
{
   CloseHandle(mImplementation->mHandle);
   delete mImplementation;
}

//******************************************************************************

void* BinarySemaphore::getHandlePtr()
{
   return (void*)&mImplementation->mHandle;
}

//******************************************************************************

void BinarySemaphore::reset()
{
   ResetEvent(mImplementation->mHandle);
}

//******************************************************************************

void BinarySemaphore::put()
{
   SetEvent(mImplementation->mHandle);
}

//******************************************************************************

bool BinarySemaphore::get(int timeout)
{
   return WaitForSingleObject(mImplementation->mHandle,timeout) != WAIT_TIMEOUT ;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class CountingSemaphore::Implementation
{
public:
   HANDLE mHandle;
};

//******************************************************************************

CountingSemaphore::CountingSemaphore() 
{
   mImplementation = new Implementation;
   mImplementation->mHandle = CreateSemaphore(NULL,0,1000000,NULL);
}

CountingSemaphore::CountingSemaphore(int aInitial) 
{
   mImplementation = new Implementation;
   mImplementation->mHandle = CreateSemaphore(NULL,aInitial,1000000,NULL);
}

//******************************************************************************

CountingSemaphore::~CountingSemaphore() 
{
   CloseHandle(mImplementation->mHandle);
   delete mImplementation;
}

//******************************************************************************

void* CountingSemaphore::getHandlePtr()
{
   return (void*)&mImplementation->mHandle;
}

//******************************************************************************

void CountingSemaphore::put()
{
   ReleaseSemaphore(mImplementation->mHandle,1,NULL);
}

//******************************************************************************

bool CountingSemaphore::get(int timeout)
{
   switch (WaitForSingleObjectEx(mImplementation->mHandle,timeout,TRUE))
   {
      case WAIT_OBJECT_0      : return true;  break;
      case WAIT_TIMEOUT       : return false; break;
      case WAIT_IO_COMPLETION : return false; break;
      default                 : return false; break;
   } 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class MutexSemaphore::Implementation
{
public:
   HANDLE mHandle;
};

//******************************************************************************

MutexSemaphore::MutexSemaphore() 
{
   mImplementation = new Implementation;
   mImplementation->mHandle = CreateMutex(NULL,FALSE,NULL);
}

//******************************************************************************

MutexSemaphore::~MutexSemaphore() 
{
   CloseHandle(mImplementation->mHandle);
   delete mImplementation;
}

//******************************************************************************

void* MutexSemaphore::getHandlePtr()
{
   return (void*)&mImplementation->mHandle;
}

//******************************************************************************

void MutexSemaphore::put()
{
   ReleaseMutex(mImplementation->mHandle);
}

//******************************************************************************

bool MutexSemaphore::get(int timeout)
{
   return WaitForSingleObject(mImplementation->mHandle,timeout) != WAIT_TIMEOUT ;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class CriticalSection::Implementation
{
public:
   CRITICAL_SECTION mCriticalSection; 
};

//******************************************************************************

CriticalSection::CriticalSection() 
{
   mImplementation = new Implementation;
   InitializeCriticalSectionAndSpinCount(&mImplementation->mCriticalSection,0x80000400); 
}

//******************************************************************************

CriticalSection::~CriticalSection() 
{
   DeleteCriticalSection(&mImplementation->mCriticalSection);
   delete mImplementation;
}

//******************************************************************************

void CriticalSection::enter()
{
   EnterCriticalSection(&mImplementation->mCriticalSection); 
}

//******************************************************************************

void CriticalSection::leave()
{
   LeaveCriticalSection(&mImplementation->mCriticalSection);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class SlimLock::Implementation
{
public:
   SRWLOCK mSRWLock; 
};

//******************************************************************************

SlimLock::SlimLock() 
{
   mImplementation = new Implementation;
   InitializeSRWLock(&mImplementation->mSRWLock); 
}

//******************************************************************************

SlimLock::~SlimLock() 
{
   delete mImplementation;
}

//******************************************************************************

void SlimLock::acquire()
{
   AcquireSRWLockExclusive(&mImplementation->mSRWLock); 
}

//******************************************************************************

void SlimLock::release()
{
   ReleaseSRWLockExclusive(&mImplementation->mSRWLock); 
}

//******************************************************************************

void SlimLock::lock()
{
   AcquireSRWLockExclusive(&mImplementation->mSRWLock); 
}

//******************************************************************************

void SlimLock::unlock()
{
   ReleaseSRWLockExclusive(&mImplementation->mSRWLock); 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class ConditionVariable::Implementation
{
public:
   SRWLOCK            mSRWLock; 
   CONDITION_VARIABLE mConditionVariable; 
   int                mPredicate;
};

//******************************************************************************

ConditionVariable::ConditionVariable() 
{
   mImplementation = new Implementation;
   InitializeSRWLock(&mImplementation->mSRWLock); 
   InitializeConditionVariable(&mImplementation->mConditionVariable); 
   mImplementation->mPredicate = 0;
}

//******************************************************************************

ConditionVariable::~ConditionVariable() 
{
   delete mImplementation;
}

//******************************************************************************

void ConditionVariable::acquireLock()
{
   AcquireSRWLockExclusive(&mImplementation->mSRWLock); 
}

//******************************************************************************

void ConditionVariable::releaseLock()
{
   ReleaseSRWLockExclusive(&mImplementation->mSRWLock); 
}

//******************************************************************************

void ConditionVariable::waitFor()
{
   AcquireSRWLockExclusive(&mImplementation->mSRWLock); 

   while (mImplementation->mPredicate == 0)
   {
      SleepConditionVariableSRW(&mImplementation->mConditionVariable, &mImplementation->mSRWLock, INFINITE, 0);
   }
   mImplementation->mPredicate = 0;

   ReleaseSRWLockExclusive(&mImplementation->mSRWLock); 
}

//******************************************************************************

void ConditionVariable::wakeUp(int aPredicate)
{
//    AcquireSRWLockExclusive(&mImplementation->mSRWLock); 
   mImplementation->mPredicate = aPredicate;
//    ReleaseSRWLockExclusive(&mImplementation->mSRWLock); 

   WakeConditionVariable(&mImplementation->mConditionVariable);

}


}//namespace
}//namespace

