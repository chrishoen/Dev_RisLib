#ifndef _SMSTHREADSSCHEDULER_H_
#define _SMSTHREADSSCHEDULER_H_

/*==============================================================================
Description:
This is a function execution sheduler
==============================================================================*/
#include "risCallPointer.h"
#include "risContainers.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a function execution scheduler.
//
// The scheduler has a priority queue that maintains a schedule of scalls,
// which are objects that contain a scheduled time to execute, a function
// pointer to call, and parameters that are to be passed to the function when
// it is called. The scalls are templated for the parameters.
//
// It has a method that can be called periodically to check the
// execution time of the element at the front of the queue. If the time
// is at or later then the current time then the element is executed 
// and then removed from the queue.
//
// The scheduler uses an internal time which is relative to a start event.
// It is updated on an owner timer call. Scheduling occurs when the timer 
// is stopped, programmed with a sequence of scalls, and then started.
//
// The priority queue is based on a linked list. It has an array of elements
// and arrays of indices that implement the linked list. It allows scalls to 
// be added or removed.
//
// The queue elements are time ordered from earlier time to later time, where 
// an earlier time is higher priority that a later time. The queue is ordered 
// from front to back, where the front is the element with the highest
// priority (earliest time) and the back is the lowest priority (latest time).
//
// The priority queue is implemented as a linked list. It uses an element array
// which is indexed, and front and back neighbor arrays that contain the
// indices of the elements to the front and back of a selected element. It is
// structured as follows:
//
//    mElement[mFrontIndex]     front element, highest priority, earliest time
//    mElement[]
//    mElement[]
//    mElement[mFrontNeighborIndex[tIndex]]  the element to the front of the individual element
//    mElement[tIndex]]                      an individual element
//    mElement[mBackNeighborIndex [tIndex]]  the element to the back  of the individual element 
//    mElement[]
//    mElement[]
//    mElement[]
//    mElement[mBackIndex]      back element, lowest priority, latest time
//
// The front element front neighbor index points to itself and the back element
// back neighbor index points to itself, that is:
//
// mFrontNeighborIndex[mFrontIndex] == mFrontIndex or front.front == front
// mBackNeighborIndex [mBackIndex]  == mBackIndex  or back.back   == back
//
// The queue has two arrays of search keys that are used to search for an
// element to be removed. Search key one is obtained when an scall is bound
// to a scheduler. This allows each scall to have a unique search key one. 
// Search key two is passed in the scheduler add call, which is called
// when an scall is scheduled. This allows each scheduled scall to have
// a unique combinations of two search keys. Note that an scall can be 
// scheduled more than once, with different times, different parameters,
// and different search key twos.
//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseSCall; 

class Scheduler
{
public:
   Scheduler();
  ~Scheduler();
   void reset();

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Timer:

   // Timer period
   void configure(double aTimerPeriod);
   double mTimerPeriod;

   // This initializes a schedule.
   // Scheduling entails stopping the timer, programming it with a sequence 
   // of scalls, and then starting it.
   void initializeSchedule ();

   // This starts a schedule.
   // Scheduling entails stopping the timer, programming it with a sequence 
   // of scalls, and then starting it.
   void startSchedule ();

   // Timer execution, this is called periodically by the owner.   
   // Updates the time variables, calls any scalls that are ready
   // to execute and then removes them.
   void onTimer();

   // The current schedule time
   double mScheduleTime;

   // The sequence times
   void startSequenceTime();
   void stopSequenceTime();
   void startSubSequenceTime();
   void stopSubSequenceTime();

   double mSequenceTime;
   bool   mSequenceTimeFlag;
   double mSubSequenceTime;
   bool   mSubSequenceTimeFlag;

   // If true then onTimer will execute scalls
   bool   mExecutionEnable;

   // This flag indicates that the next on timer will start a relative schedule.
   bool   mStartScheduleFlag;

   // Timer enable
   void enableTimer();
   void disableTimer();

   //--------------------------------------------------------------
   //--------------------------------------------------------------
   //--------------------------------------------------------------
   // Priority queue:

   // Add an scall to the priority queue. It allocates queue element and index
   // storage and inserts the element into the queue. It also stores the search
   // keys. For the insertion, it can either start searching for the insertion
   // point at the back or the front of the queue.

   void add(
      BaseSCall* aSCall,
      int  aSearchKey1,
      int  aSearchKey2,
      bool aStartSearchAtBack);

   // Return the scall at the front of the priority queue
   BaseSCall* front();

   // Remove the scall at the front of the priority queue
   void removeFront();
  
   // Remove an scall from the priority queue. It searches the queue, based on
   // the two search keys, for the element. The search can be either from the
   // queue back or front. After the element is found it is removed and
   // deleted. There is an option to execute it before removal.

   bool remove(
      int  aSearchKey1,
      int  aSearchKey2,
      bool aStartSearchAtBack,
      bool aExecuteBeforeRemoval);

   // Remove all scalls
   void flush();

   //--------------------------------------------------------------
   // Priority queue data:

   enum {ArraySize =  1000};

   // Number of allocated elements
   int mNumOfElements;

   // Queue element allocator
   Ris::Containers::Allocator<ArraySize> mAllocator;

   // Array of queue elements
   // element = mElement[tIndex]
   BaseSCall* mElement     [ArraySize];

   // Linked list element neighbor indices
   // element.front = mElement[mFrontNeighborIndex [tIndex]]
   // element.back  = mElement[mBackNeighborIndex  [tIndex]]
   int mFrontNeighborIndex [ArraySize];
   int mBackNeighborIndex  [ArraySize];

   // Linked list front and back
   // front = mElement[mFrontIndex]
   // back  = mElement[mBackindex]
   int mFrontIndex;
   int mBackIndex;

   // Linked list search keys
   int mSearchKey1         [ArraySize];
   int mSearchKey2         [ArraySize];

   // Bind key, used for removal
   // This is incremented by scall bind calls
   // and is used as the first search index
   int mBindKey;

   // Helpers
   bool isEmpty(){return mNumOfElements==0;}
   bool isFull() {return mNumOfElements==ArraySize;}
   void show();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// SCall

//******************************************************************************
// Base class

class BaseSCall
{
public:

   //---------------------------------------------------------------------------
   // Constructor
   BaseSCall()
   {
      reset();
   }
   void reset()
   {
      mScheduler=0;
      mBindKey=0;
   }

   //---------------------------------------------------------------------------
   // Scheduler to which the SCall is bound
   Scheduler* mScheduler;

   // Time that the scheduler should call execute():
   double  mExecuteTime;

   // Bind key, used for removal search key one
   int     mBindKey;

   //---------------------------------------------------------------------------
   // Execute, it is called by the scheduler at mExecuteAbsTime
   virtual void execute()=0;

   //---------------------------------------------------------------------------
   // Remove self from the scheduler
   void remove(int aSearchKey2)
   {
      mScheduler->remove(mBindKey,aSearchKey2,false,false);
   }
   //---------------------------------------------------------------------------
   // Execute now and then remove self from the scheduler 
   void executeNow(int aSearchKey2)
   {
      mScheduler->remove(mBindKey,aSearchKey2,false,true);
   }
};

//******************************************************************************
// SCall1 is a class template for an SCall with 1 argument

template <class X1>
class SCall1 : public BaseSCall
{
public:
   //---------------------------------------------------------------------------
   // Deferred procedure arguments:

   X1 mX1;

   //---------------------------------------------------------------------------
   // Binds the SCall to a scheduler and a deferred procedure call

   template <class CallObject,class CallMethod>
   void bind(Scheduler* aScheduler,CallObject aCallObject,CallMethod aCallMethod)
   {
      mScheduler = aScheduler;
      mBindKey = ++mScheduler->mBindKey;
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   //---------------------------------------------------------------------------
   // Schedules the SCall with the bound scheduler. It creates a new copy of 
   // the SCall, sets its arguments, and adds it to the scheduler.

   void schedule(int aSearchKey2,double aExecuteTime,X1 aX1)
   {
      // Create a new copy of this SCall.
      SCall1* tSCall = new SCall1(*this);
      // Set its arguments.
      tSCall->mX1=aX1;

      // Set its execution time
      tSCall->mExecuteTime = aExecuteTime;

      // Add it to the scheduler
      mScheduler->add(tSCall,mBindKey,aSearchKey2,true);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the scheduler at the scheduled time

   // Execute CallPointer, it contains the address of the deferred procedure.
   typedef Ris::CallPointer1<X1> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the bound scheduler at the scheduled time
   void execute()
   {
      mExecuteCallPointer(mX1);
   }
};

//******************************************************************************
// SCall2 is a class template for an SCall with 2 arguments

template <class X1,class X2>
class SCall2 : public BaseSCall
{
public:
   //---------------------------------------------------------------------------
   // Deferred procedure arguments:

   X1 mX1;
   X2 mX2;

   //---------------------------------------------------------------------------
   // Binds the SCall to a scheduler and a deferred procedure call

   template <class CallObject,class CallMethod>
   void bind(Scheduler* aScheduler,CallObject aCallObject,CallMethod aCallMethod)
   {
      mScheduler = aScheduler;
      mBindKey = ++mScheduler->mBindKey;
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   //---------------------------------------------------------------------------
   // Schedules the SCall with the bound scheduler. It creates a new copy of 
   // the SCall, sets its arguments, and adds it to the scheduler.

   //---------------------------------------------------------------------------
   // Schedules the SCall with the bound scheduler. It creates a new copy of 
   // the SCall, sets its arguments, and adds it to the scheduler.

   void schedule(int aSearchKey2,double aExecuteTime,X1 aX1,X2 aX2)
   {
      // Create a new copy of this SCall.
      SCall2* tSCall = new SCall2(*this);
      // Set its arguments.
      tSCall->mX1=aX1;
      tSCall->mX2=aX2;
      // Set its execution time
      tSCall->mExecuteTime = aExecuteTime;
      // Add it to the scheduler
      mScheduler->add(tSCall,mBindKey,aSearchKey2,true);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the scheduler at the scheduled time

   // Execute CallPointer, it contains the address of the deferred procedure.
   typedef Ris::CallPointer2<X1,X2> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the bound scheduler at the scheduled time
   void execute()
   {
      mExecuteCallPointer(mX1,mX2);
   }
};

//******************************************************************************
// SCall3 is a class template for an SCall with 3 arguments

template <class X1,class X2,class X3>
class SCall3 : public BaseSCall
{
public:
   //---------------------------------------------------------------------------
   // Deferred procedure arguments:

   X1 mX1;
   X2 mX2;
   X3 mX3;

   //---------------------------------------------------------------------------
   // Binds the SCall to a scheduler and a deferred procedure call

   template <class CallObject,class CallMethod>
   void bind(Scheduler* aScheduler,CallObject aCallObject,CallMethod aCallMethod)
   {
      mScheduler = aScheduler;
      mBindKey = ++mScheduler->mBindKey;
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   //---------------------------------------------------------------------------
   // Schedules the SCall with the bound scheduler. It creates a new copy of 
   // the SCall, sets its arguments, and adds it to the scheduler.

   void schedule(int aSearchKey2,double aExecuteTime,X1 aX1,X2 aX2,X3 aX3)
   {
      // Create a new copy of this SCall.
      SCall3* tSCall = new SCall3(*this);
      // Set its arguments.
      tSCall->mX1=aX1;
      tSCall->mX2=aX2;
      tSCall->mX3=aX3;
      // Set its execution time
      tSCall->mExecuteTime = aExecuteTime;
      // Add it to the scheduler
      mScheduler->add(tSCall,mBindKey,aSearchKey2,true);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the scheduler at the scheduled time

   // Execute CallPointer, it contains the address of the deferred procedure.
   typedef Ris::CallPointer3<X1,X2,X3> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the bound scheduler at the scheduled time
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3);
   }
};

//******************************************************************************
// SCall4 is a class template for an SCall with 4 arguments

template <class X1,class X2,class X3,class X4>
class SCall4 : public BaseSCall
{
public:
   //---------------------------------------------------------------------------
   // Deferred procedure arguments:

   X1 mX1;
   X2 mX2;
   X3 mX3;
   X4 mX4;

   //---------------------------------------------------------------------------
   // Binds the SCall to a scheduler and a deferred procedure call

   template <class CallObject,class CallMethod>
   void bind(Scheduler* aScheduler,CallObject aCallObject,CallMethod aCallMethod)
   {
      mScheduler = aScheduler;
      mBindKey = ++mScheduler->mBindKey;
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   //---------------------------------------------------------------------------
   // Schedules the SCall with the bound scheduler. It creates a new copy of 
   // the SCall, sets its arguments, and adds it to the scheduler.

   void schedule(int aSearchKey2,double aExecuteTime,X1 aX1,X2 aX2,X3 aX3,X4 aX4)
   {
      // Create a new copy of this SCall.
      SCall4* tSCall = new SCall4(*this);
      // Set its arguments.
      tSCall->mX1=aX1;
      tSCall->mX2=aX2;
      tSCall->mX3=aX3;
      tSCall->mX4=aX4;
      // Set its execution time
      tSCall->mExecuteTime = aExecuteTime;
      // Add it to the scheduler
      mScheduler->add(tSCall,mBindKey,aSearchKey2,true);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the scheduler at the scheduled time

   // Execute CallPointer, it contains the address of the deferred procedure.
   typedef Ris::CallPointer4<X1,X2,X3,X4> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the bound scheduler at the scheduled time
   void execute()
   {
      mExecuteCallPointer(mX1,mX2,mX3,mX4);
   }
};

//******************************************************************************
// SCall0 is a class template for an SCall with 0 arguments

class SCall0 : public BaseSCall
{
public:
   //---------------------------------------------------------------------------
   // Deferred procedure arguments:

   //---------------------------------------------------------------------------
   // Binds the SCall to a scheduler and a deferred procedure call

   template <class CallObject,class CallMethod>
   void bind(Scheduler* aScheduler,CallObject aCallObject,CallMethod aCallMethod)
   {
      mScheduler = aScheduler;
      mBindKey = ++mScheduler->mBindKey;
      mExecuteCallPointer.bind (aCallObject,aCallMethod);
   }

   //---------------------------------------------------------------------------
   // Schedules the SCall with the bound scheduler. It creates a new copy of 
   // the SCall, sets its arguments, and adds it to the scheduler.

   void schedule(int aSearchKey2,double aExecuteTime)
   {
      // Create a new copy of this SCall.
      SCall0* tSCall = new SCall0(*this);
      // Set its execution time
      tSCall->mExecuteTime = aExecuteTime;
      // Add it to the scheduler
      mScheduler->add(tSCall,mBindKey,aSearchKey2,true);
   }

   //---------------------------------------------------------------------------
   // Execute, it is called by the scheduler at the scheduled time

   // Execute CallPointer, it contains the address of the deferred procedure.
   typedef Ris::CallPointer0<> ExecuteCallPointer;
   ExecuteCallPointer mExecuteCallPointer;

   // This is called by the bound scheduler at the scheduled time
   void execute()
   {
      mExecuteCallPointer();
   }
};

//******************************************************************************

//******************************************************************************
}//namespace
}//namespace

#endif
