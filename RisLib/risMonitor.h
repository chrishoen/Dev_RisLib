#pragma once

/*==============================================================================
Status variables.
=============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <atomic>

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// The following rules are for shared memory regions that are shared between
// different processes (who therefore have different address spaces):
// 
// 1) No constructors.
// 2) No pointers.
// 3) No dynamic memory, this means no std::vector, ...
// 4) No vtables, this means no virtual functions.
// 5) Be careful with your atomic loads and stores.
//
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a variable that can be bound to another variable and used to
// monitor it. It provides an update function that calculates delta,
// min, and max. 
// 
// Instances are shared memory safe. There is no constructor, there is an
// explicit reset function that is called by the bind function. If an
// instance lives in shared memory, care must be taken that the instance
// is initialized (bind) and updated in only one owner address space.

template <typename T>
class Monitor 
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // A pointer to the variable to that this variable is bound to.
   T* mPtr;

   // These are updated by the update method.
   T mValue;
   T mLastValue;
   T mDelta;
   T mMin;
   T mMax;

   // True if this is the first update.
   bool mFirstFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // There is no constructor.
   void reset ()
   {
      mValue = 0;
      mLastValue = 0;
      mDelta = 0;
      mMin = 0;
      mMax = 0;
      mFirstFlag = true;
   }

   // Bind to another variable.
   void bind(T* aPtr)
   {
      reset();
      mPtr = aPtr;
   }

   // Update the variables.
   void update()
   {
      if (mFirstFlag)
      {
         mFirstFlag = false;
         mValue = *mPtr;
         mDelta = 0;
         mMin = mValue;
         mMax = mValue;
         mLastValue = mValue;
      }
      else
      {
         mValue = *mPtr;
         mDelta = mValue - mLastValue;
         if (mValue < mMin) mMin = mValue;
         if (mValue > mMax) mMax = mValue;
         mLastValue = mValue;
      }
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a variable that can be bound to an atomic variable and used to
// monitor it. It provides an update function that calculates delta,
// min, and max.

template <typename T>
class MonitorAtomic
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // A pointer to the variable to that this variable is bound to.
   std::atomic<T>* mPtr;

   // These are updated by the update method.
   T mValue;
   T mLastValue;
   T mDelta;
   T mMin;
   T mMax;

   // True if this is the first update.
   bool mFirstFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   MonitorAtomic()
   {
      mValue = 0;
      mLastValue = 0;
      mDelta = 0;
      mMin = 0;
      mMax = 0;
      mFirstFlag = true;
   }

   // Bind to another variable.
   void bind(std::atomic<T>* aPtr)
   {
      mPtr = aPtr;
   }

   // Update the variables.
   void update()
   {
      if (mFirstFlag)
      {
         mFirstFlag = false;
         mValue = mPtr->load(std::memory_order_relaxed);
         mDelta = 0;
         mMin = mValue;
         mMax = mValue;
         mLastValue = mValue;
      }
      else
      {
         mValue = mPtr->load(std::memory_order_relaxed);
         mDelta = mValue - mLastValue;
         if (mValue < mMin) mMin = mValue;
         if (mValue > mMax) mMax = mValue;
         mLastValue = mValue;
      }
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

