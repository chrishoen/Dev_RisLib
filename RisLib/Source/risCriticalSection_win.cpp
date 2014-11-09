/*==============================================================================

This file contains functions that imlement a critical section.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#ifdef _AFXDLL
// MFC project
#define _WIN32_WINNT 0x0500
#include <afxwin.h>
#else
#define _WIN32_WINNT 0x0500
// NOT MFC project
#include <windows.h> 
#endif

#include <stdio.h>
#include <assert.h>

#include "risCriticalSection.h"

#pragma unmanaged

 namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

// Global variable

class CriticalSectionWrapper
{
public:
   CriticalSectionWrapper()
   {
      // Initialize the critical section one time only.
      InitializeCriticalSectionAndSpinCount(&mCriticalSection,0x80000400); 
   }

   ~CriticalSectionWrapper()
   {
    // Release resources used by the critical section object.
    DeleteCriticalSection(&mCriticalSection);
   }

   void enterCriticalSection()
   {
       // Request ownership of the critical section.
       EnterCriticalSection(&mCriticalSection); 
   }
   void leaveCriticalSection()
   {
       // Release ownership of the critical section.
       LeaveCriticalSection(&mCriticalSection);
   }
   
   CRITICAL_SECTION mCriticalSection; 
};

CriticalSectionWrapper gCriticalSectionWrapper;

void enterCriticalSection()
{
   gCriticalSectionWrapper.enterCriticalSection();
}

void leaveCriticalSection()
{
   gCriticalSectionWrapper.leaveCriticalSection();
}

}//namespace










