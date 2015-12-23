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

      // Initialize the critical section.
      InitializeCriticalSectionAndSpinCount(&mImplementation->mCriticalSection,0x80000400); 

   }

   //******************************************************************************

   CriticalSection::~CriticalSection() 
   {
      // Release resources used by the critical section object.
      DeleteCriticalSection(&mImplementation->mCriticalSection);
      delete mImplementation;
   }

   //******************************************************************************

   void CriticalSection::enter()
   {
      // Request ownership of the critical section.
      EnterCriticalSection(&mImplementation->mCriticalSection); 
   }

   //******************************************************************************

   void CriticalSection::leave()
   {
      // Release ownership of the critical section.
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

      // Initialize the lock.
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
      // Acquire the lock
      AcquireSRWLockExclusive(&mImplementation->mSRWLock); 
   }

   //******************************************************************************

   void SlimLock::release()
   {
      // Release the lock
      ReleaseSRWLockExclusive(&mImplementation->mSRWLock); 
   }


}//namespace
}//namespace

