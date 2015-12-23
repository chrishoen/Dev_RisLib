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


}//namespace
}//namespace

