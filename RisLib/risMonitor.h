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
// This is a variable that can be bound to another variable and used to
// monitor it. It provides an update function that calculates delta,
// min, and max.

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

   // Constructor.
   Monitor ()
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
         mValue = *mPtr->load(std::memory_order_relaxed);
         mDelta = 0;
         mMin = mValue;
         mMax = mValue;
         mLastValue = mValue;
      }
      else
      {
         mValue = *mPtr->load(std::memory_order_relaxed);
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

