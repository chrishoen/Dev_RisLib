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


   //******************************************************************************
   //******************************************************************************
   //******************************************************************************

   class AlertableSemaphore::Implementation
   {
   public:
      HANDLE mHandle;
   };

   //******************************************************************************

   AlertableSemaphore::AlertableSemaphore() 
   {
      mImplementation = new Implementation;
      mImplementation->mHandle = CreateEvent(NULL,FALSE,FALSE,NULL);
   }

   //******************************************************************************

   AlertableSemaphore::~AlertableSemaphore() 
   {
      CloseHandle(mImplementation->mHandle);
      delete mImplementation;
   }

   //******************************************************************************

   void AlertableSemaphore::put()
   {
      SetEvent(mImplementation->mHandle);
   }

   //******************************************************************************

   bool AlertableSemaphore::get(int timeout)
   {
      return WaitForSingleObjectEx(mImplementation->mHandle,timeout,true) != WAIT_TIMEOUT ;
   }


}//namespace
}//namespace

