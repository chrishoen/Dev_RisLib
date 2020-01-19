/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "my_functions.h"
#include "prnPrint.h"
#include "risNanoConvert.h"
#include "risNamedSemaphore.h"


namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Named mutex.

// Specific implementation variables.
class NamedSemaphore::Specific
{
public:
   bool   mValidFlag;
   sem_t* mHandle;
   Specific()
   {
      mValidFlag = false;
      mHandle = 0;
   }
};

// Constructor. Create the mutex.
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

// Create the mutex. Call this if using default constructor.
void NamedSemaphore::initialize(const char* aName)
{
   mSpecific->mHandle = sem_open(aName, O_CREAT, O_RDWR, 1);
   if (mSpecific->mHandle != SEM_FAILED)
   {
      mSpecific->mValidFlag = true;
   }
   else
   {
      printf("NamedSemaphore failed");
      mSpecific->mValidFlag = false;
   }

}

// Destructor. Delete the mutex.
NamedSemaphore::~NamedSemaphore()
{
   sem_close(mSpecific->mHandle);
   delete mSpecific;
}

// Lock the mutex.
void NamedSemaphore::get()
{
   sem_wait(mSpecific->mHandle);
}

// Unlock the mutex.
void NamedSemaphore::put()
{
   sem_post(mSpecific->mHandle);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


