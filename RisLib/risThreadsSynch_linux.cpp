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
#include <errno.h>
#include "my_functions.h"
#include "prnPrint.h"
#include "risNanoConvert.h"
#include "risThreadsSynch.h"


namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Binary semaphore.

// Specific implementation variables.
class BinarySemaphore::Specific
{
public:
   sem_t mHandle;
};

// Constructor. Create the semaphore.
BinarySemaphore::BinarySemaphore()
{
   mSpecific = new Specific;
   sem_init(&mSpecific->mHandle,0,0);
}

// Reset the semaphore.
void BinarySemaphore::reset()
{
   // If there is a pending event, then clear it.
   int tCount = 0;
   sem_getvalue(&mSpecific->mHandle, &tCount);
   if (tCount > 0)
   {
      sem_wait(&mSpecific->mHandle);
   }
}

// Destructor. Delete the semaphore.
BinarySemaphore::~BinarySemaphore()
{
   sem_destroy(&mSpecific->mHandle);
   delete mSpecific;
}

// Put to the semaphore.
void BinarySemaphore::put()
{
   // Get current semaphore count
   int tCount=0;
   sem_getvalue(&mSpecific->mHandle, &tCount); 

   // If not zero then return, this is a binary semaphore
   if (tCount != 0) return;

   // Post to semahore
   sem_post(&mSpecific->mHandle);
}

// Get from the semaphore, block until timeout, return true if no timeout.
bool BinarySemaphore::get(int aTimeout)
{
   switch (aTimeout)
   {
   case 0 :
      return true;
      break;
   case -1 :
      sem_wait(&mSpecific->mHandle);
      return true;
      break;
   default :
      // Time variables
      timespec   tBeginTimespec;
      timespec   tEndTimespec;
      timespec   tExpireTimespec;
      long long  tBeginTimeNs;
      long long  tEndTimeNs;
      long long  tExpireTimeNs;

      // Get current timespec at beginning
      clock_gettime(CLOCK_REALTIME, &tBeginTimespec);

      // Convert to ns
      tBeginTimeNs = Ris::NanoConvert::getNsFromTimespec(&tBeginTimespec);

      // Calculate expiration time from timeout (which is in milliseconds)
      tExpireTimeNs = tBeginTimeNs + Ris::NanoConvert::getNsFromMs(aTimeout);

      // Convert to timespec
      tExpireTimespec = Ris::NanoConvert::getTimespecFromNs(tExpireTimeNs);

      // Semaphore timed wait with expiration timespec
      sem_timedwait(&mSpecific->mHandle,&tExpireTimespec);

      // Get current timespec at end
      clock_gettime(CLOCK_REALTIME, &tEndTimespec);

      // Convert to ns
      tEndTimeNs = Ris::NanoConvert::getNsFromTimespec(&tEndTimespec);

      // Compare
      bool tNoTimeout = tExpireTimeNs >= tEndTimeNs ? true : false; 

      // Result
      return tNoTimeout;
   }
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Counting semaphore.

// Specific implementation variables.
class CountingSemaphore::Specific
{
public:
   sem_t mHandle;
};

// Constructor. Create the semaphore.
CountingSemaphore::CountingSemaphore()
{
   mSpecific = new Specific;
   sem_init(&mSpecific->mHandle, 0, 0);
}

// Constructor. Create the semaphore.
CountingSemaphore::CountingSemaphore(int aInitial)
{
   mSpecific = new Specific;
   sem_init(&mSpecific->mHandle, 0, aInitial);
}

// Reset the semaphore.
void CountingSemaphore::reset()
{
   while (true)
   {
      // If there is a pending event, then clear it.
      int tCount = 0;
      sem_getvalue(&mSpecific->mHandle, &tCount);
      if (tCount > 0)
      {
         sem_wait(&mSpecific->mHandle);
      }
      else
      {
         break;
      }
   }
}

// Destructor. Delete the semaphore.
CountingSemaphore::~CountingSemaphore()
{
   sem_destroy(&mSpecific->mHandle);
   delete mSpecific;
}

// Put to the semaphore.
void CountingSemaphore::put()
{
   // Get current semaphore count
   int tCount = 0;
   sem_getvalue(&mSpecific->mHandle, &tCount);

   // Post to semahore
   sem_post(&mSpecific->mHandle);
}

// Get from the semaphore, block until timeout, return true if no timeout.
bool CountingSemaphore::get(int aTimeout)
{
   switch (aTimeout)
   {
   case 0:
      return true;
      break;
   case -1:
      sem_wait(&mSpecific->mHandle);
      return true;
      break;
   default:
      // Time variables
      timespec   tBeginTimespec;
      timespec   tEndTimespec;
      timespec   tExpireTimespec;
      long long  tBeginTimeNs;
      long long  tEndTimeNs;
      long long  tExpireTimeNs;

      // Get current timespec at beginning
      clock_gettime(CLOCK_REALTIME, &tBeginTimespec);

      // Convert to ns
      tBeginTimeNs = Ris::NanoConvert::getNsFromTimespec(&tBeginTimespec);

      // Calculate expiration time from timeout (which is in milliseconds)
      tExpireTimeNs = tBeginTimeNs + Ris::NanoConvert::getNsFromMs(aTimeout);

      // Convert to timespec
      tExpireTimespec = Ris::NanoConvert::getTimespecFromNs(tExpireTimeNs);

      // Semaphore timed wait with expiration timespec
      sem_timedwait(&mSpecific->mHandle, &tExpireTimespec);

      // Get current timespec at end
      clock_gettime(CLOCK_REALTIME, &tEndTimespec);

      // Convert to ns
      tEndTimeNs = Ris::NanoConvert::getNsFromTimespec(&tEndTimespec);

      // Compare
      bool tNoTimeout = tExpireTimeNs >= tEndTimeNs ? true : false;

      // Result
      return tNoTimeout;
   }
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Mutex semaphore.

// Specific implementation variables.
class MutexSemaphore::Specific
{
public:
   pthread_mutex_t mMutex;
};

// Constructor. Create the mutex.
MutexSemaphore::MutexSemaphore()
{
   mSpecific = new Specific;
   pthread_mutex_init(&mSpecific->mMutex, NULL);
}

// Destructor. Delete the mutex.
MutexSemaphore::~MutexSemaphore()
{
   pthread_mutex_destroy(&mSpecific->mMutex);
   delete mSpecific;
}

// Lock the mutex.
void MutexSemaphore::lock()
{
   pthread_mutex_lock(&mSpecific->mMutex);
}

// Unlock the mutex.
void MutexSemaphore::unlock()
{
   pthread_mutex_unlock(&mSpecific->mMutex);
}

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
   mSpecific->mHandle = sem_open(aName, O_CREAT, 0644, 1);
   if (mSpecific->mHandle != SEM_FAILED)
   {
      mSpecific->mValidFlag = true;
   }
   else
   {
      printf("NamedSemaphore failed %d\n", errno);
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
// Named mutex.

// Specific implementation variables.
class NamedMutex::Specific
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
   mSpecific->mHandle = sem_open(aName, O_CREAT, 0644, 1);
   if (mSpecific->mHandle != SEM_FAILED)
   {
      mSpecific->mValidFlag = true;
   }
   else
   {
      printf("NamedMutex failed %d\n", errno);
      mSpecific->mValidFlag = false;
   }

}

// Destructor. Delete the mutex.
NamedMutex::~NamedMutex()
{
   sem_close(mSpecific->mHandle);
   delete mSpecific;
}

// Lock the mutex.
void NamedMutex::lock()
{
   sem_wait(mSpecific->mHandle);
}

// Unlock the mutex.
void NamedMutex::unlock()
{
   sem_post(mSpecific->mHandle);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


