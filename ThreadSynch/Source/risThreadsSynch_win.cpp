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

   class ConditionVariableSRW::Implementation
   {
   public:
      SRWLOCK            mSRWLock; 
      CONDITION_VARIABLE mConditionVariable; 
      int                mPredicate;
   };

   //******************************************************************************

   ConditionVariableSRW::ConditionVariableSRW() 
   {
      mImplementation = new Implementation;
      InitializeSRWLock(&mImplementation->mSRWLock); 
      InitializeConditionVariable(&mImplementation->mConditionVariable); 
      mImplementation->mPredicate = 0;
   }

   //******************************************************************************

   ConditionVariableSRW::~ConditionVariableSRW() 
   {
      delete mImplementation;
   }

   //******************************************************************************

   void ConditionVariableSRW::acquireLock()
   {
      AcquireSRWLockExclusive(&mImplementation->mSRWLock); 
   }

   //******************************************************************************

   void ConditionVariableSRW::releaseLock()
   {
      ReleaseSRWLockExclusive(&mImplementation->mSRWLock); 
   }

   //******************************************************************************

   void ConditionVariableSRW::waitFor()
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

   void ConditionVariableSRW::wakeUp(int aPredicate)
   {
      //    AcquireSRWLockExclusive(&mImplementation->mSRWLock); 

      mImplementation->mPredicate = aPredicate;
      WakeConditionVariable(&mImplementation->mConditionVariable);

      //    ReleaseSRWLockExclusive(&mImplementation->mSRWLock); 
   }

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************

   class ConditionVariableCS::Implementation
   {
   public:
      CRITICAL_SECTION   mCriticalSection; 
      CONDITION_VARIABLE mConditionVariable; 
      int                mPredicate;
   };

   //******************************************************************************

   ConditionVariableCS::ConditionVariableCS() 
   {
      mImplementation = new Implementation;
      InitializeCriticalSectionAndSpinCount(&mImplementation->mCriticalSection,0x80000400); 
      InitializeConditionVariable(&mImplementation->mConditionVariable); 
      mImplementation->mPredicate = 0;
   }

   //******************************************************************************

   ConditionVariableCS::~ConditionVariableCS() 
   {
      DeleteCriticalSection(&mImplementation->mCriticalSection); 
      delete mImplementation;
   }

   //******************************************************************************

   void ConditionVariableCS::acquireLock()
   {
      EnterCriticalSection(&mImplementation->mCriticalSection); 
   }

   //******************************************************************************

   void ConditionVariableCS::releaseLock()
   {
      LeaveCriticalSection(&mImplementation->mCriticalSection); 
   }

   //******************************************************************************

   void ConditionVariableCS::waitFor()
   {
      EnterCriticalSection(&mImplementation->mCriticalSection); 

      while (mImplementation->mPredicate == 0)
      {
         SleepConditionVariableCS(&mImplementation->mConditionVariable, &mImplementation->mCriticalSection, INFINITE);
      }
      mImplementation->mPredicate = 0;

      LeaveCriticalSection(&mImplementation->mCriticalSection); 
   }

   //******************************************************************************

   void ConditionVariableCS::wakeUp(int aPredicate)
   {
      //    AcquireSRWLockExclusive(&mImplementation->mSRWLock); 

      mImplementation->mPredicate = aPredicate;
      WakeConditionVariable(&mImplementation->mConditionVariable);

      //    ReleaseSRWLockExclusive(&mImplementation->mSRWLock); 
   }


}//namespace
}//namespace

