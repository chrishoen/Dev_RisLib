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

class BinarySemaphore::Specific
{
public:
   HANDLE mHandle;
};

//******************************************************************************

BinarySemaphore::BinarySemaphore() 
{
   mSpecific = new Specific;
   mSpecific->mHandle = CreateEvent(NULL,FALSE,FALSE,NULL);
   mStatusCode=0;
}

//******************************************************************************

BinarySemaphore::~BinarySemaphore() 
{
   CloseHandle(mSpecific->mHandle);
   delete mSpecific;
}

//******************************************************************************

void BinarySemaphore::reset()
{
   ResetEvent(mSpecific->mHandle);
}

//******************************************************************************

void BinarySemaphore::put()
{
   SetEvent(mSpecific->mHandle);
}

//******************************************************************************

bool BinarySemaphore::get(int timeout)
{
   return WaitForSingleObject(mSpecific->mHandle,timeout) != WAIT_TIMEOUT ;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class CountingSemaphore::Specific
{
public:
   HANDLE mHandle;
};

//******************************************************************************

CountingSemaphore::CountingSemaphore() 
{
   mSpecific = new Specific;
   mSpecific->mHandle = CreateSemaphore(NULL,0,1000000,NULL);
}

CountingSemaphore::CountingSemaphore(int aInitial) 
{
   mSpecific = new Specific;
   mSpecific->mHandle = CreateSemaphore(NULL,aInitial,1000000,NULL);
}

//******************************************************************************

CountingSemaphore::~CountingSemaphore() 
{
   CloseHandle(mSpecific->mHandle);
   delete mSpecific;
}

//******************************************************************************

void CountingSemaphore::put()
{
   ReleaseSemaphore(mSpecific->mHandle,1,NULL);
}

//******************************************************************************

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

class MutexSemaphore::Specific
{
public:
   SRWLOCK mSRWLock;
};

//******************************************************************************

MutexSemaphore::MutexSemaphore() 
{
   mSpecific = new Specific;
   InitializeSRWLock(&mSpecific->mSRWLock); 
}

//******************************************************************************

MutexSemaphore::~MutexSemaphore() 
{
   delete mSpecific;
}

//******************************************************************************

void MutexSemaphore::lock()
{
   AcquireSRWLockExclusive(&mSpecific->mSRWLock);
}

//******************************************************************************

void MutexSemaphore::unlock()
{
   ReleaseSRWLockExclusive(&mSpecific->mSRWLock); 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class CriticalSection::Specific
{
public:
   CRITICAL_SECTION mCriticalSection; 
};

//******************************************************************************

CriticalSection::CriticalSection() 
{
   mSpecific = new Specific;
   InitializeCriticalSectionAndSpinCount(&mSpecific->mCriticalSection,0x80000400); 
}

//******************************************************************************

CriticalSection::~CriticalSection() 
{
   DeleteCriticalSection(&mSpecific->mCriticalSection);
   delete mSpecific;
}

//******************************************************************************

void CriticalSection::enter()
{
   EnterCriticalSection(&mSpecific->mCriticalSection); 
}

//******************************************************************************

void CriticalSection::leave()
{
   LeaveCriticalSection(&mSpecific->mCriticalSection);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class ConditionVariable::Specific
{
public:
   SRWLOCK            mSRWLock; 
   CONDITION_VARIABLE mConditionVariable; 
   int                mPredicate;
};

//******************************************************************************

ConditionVariable::ConditionVariable() 
{
   mSpecific = new Specific;
   InitializeSRWLock(&mSpecific->mSRWLock); 
   InitializeConditionVariable(&mSpecific->mConditionVariable); 
   mSpecific->mPredicate = 0;
}

//******************************************************************************

ConditionVariable::~ConditionVariable() 
{
   delete mSpecific;
}

//******************************************************************************

void ConditionVariable::acquireLock()
{
   AcquireSRWLockExclusive(&mSpecific->mSRWLock); 
}

//******************************************************************************

void ConditionVariable::releaseLock()
{
   ReleaseSRWLockExclusive(&mSpecific->mSRWLock); 
}

//******************************************************************************

void ConditionVariable::waitFor()
{
   AcquireSRWLockExclusive(&mSpecific->mSRWLock); 

   while (mSpecific->mPredicate == 0)
   {
      SleepConditionVariableSRW(&mSpecific->mConditionVariable, &mSpecific->mSRWLock, INFINITE, 0);
   }
   mSpecific->mPredicate = 0;

   ReleaseSRWLockExclusive(&mSpecific->mSRWLock); 
}

//******************************************************************************

void ConditionVariable::wakeUp(int aPredicate)
{
//    AcquireSRWLockExclusive(&mSpecific->mSRWLock); 
   mSpecific->mPredicate = aPredicate;
//    ReleaseSRWLockExclusive(&mSpecific->mSRWLock); 

   WakeConditionVariable(&mSpecific->mConditionVariable);

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

