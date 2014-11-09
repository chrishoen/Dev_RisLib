/*==============================================================================
Description:
==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>

#include "prnPrint.h"

#include "risThreadsScheduler.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

Scheduler::Scheduler()
{

   for (int i=0;i<ArraySize;i++)
   {
      mElement[i]=0;
   }
   reset();
}

//******************************************************************************

Scheduler::~Scheduler()
{
   flush();
}

//******************************************************************************

void Scheduler::reset()
{
   mTimerPeriod = 1.0;

   mExecutionEnable   = false;
   mStartScheduleFlag = false;

   mNumOfElements=0;
   mBindKey=0;

   mScheduleTime = 0.0;

   mSequenceTime     = 0.0;
   mSequenceTimeFlag = false;
   mSubSequenceTime     = 0.0;
   mSubSequenceTimeFlag = false;
}

//******************************************************************************

void Scheduler::configure(double aTimerPeriod)
{
   mTimerPeriod = aTimerPeriod;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Start schedule

void Scheduler::initializeSchedule ()
{
   mExecutionEnable   = false;
   mStartScheduleFlag = false;
   mNumOfElements     = 0;
   mBindKey           = 0;

   mScheduleTime       = 0.0;
}

void Scheduler::startSchedule ()
{
   mExecutionEnable  = true;
   mStartScheduleFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sequence tims

void Scheduler::startSequenceTime ()
{
   mSequenceTime     = 0.0;
   mSequenceTimeFlag = true;
}

void Scheduler::stopSequenceTime ()
{
   mSequenceTime     = 0.0;
   mSequenceTimeFlag = false;
}

void Scheduler::startSubSequenceTime ()
{
   mSubSequenceTime     = 0.0;
   mSubSequenceTimeFlag = true;
}

void Scheduler::stopSubSequenceTime ()
{
   mSubSequenceTime     = 0.0;
   mSubSequenceTimeFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// enable

void Scheduler::enableTimer()
{
   mExecutionEnable=true;
}
void Scheduler::disableTimer()
{
   mExecutionEnable=false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// onTimer

void Scheduler::onTimer()
{
   // If this is the first timer for a relative schedule
   // then start the relative schedule
   if (mStartScheduleFlag)
   {
      mStartScheduleFlag = false;
      mScheduleTime = 0.0;
   }

   // Guard
   if (!mExecutionEnable) return;

   // Execute any scalls that are ready to execute
   while(true)
   {
      // Guard
      if (isEmpty()) break;

      // If the current time is later than then next scheduled
      // scall time then process the next scheduled scall
      if (mScheduleTime >= front()->mExecuteTime)
      {
         BaseSCall* tSCall = front();
         tSCall->execute();
         removeFront();
      }
      else
      {
         break;
      }
   }

   // Update current time
   mScheduleTime += mTimerPeriod;

   // Update sequence times
   if (mSequenceTimeFlag)
   {
      mSequenceTime += mTimerPeriod;
   }
   // Update sequence times
   if (mSubSequenceTimeFlag)
   {
      mSubSequenceTime += mTimerPeriod;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// add

void Scheduler::add(
      BaseSCall* aSCall,
      int        aSearchKey1,
      int        aSearchKey2,
      bool       aStartSearchAtBack)
{
   //---------------------------------------------------------------------------
   // If the scall schedule time is at or earlier that the current time
   // scall time then process the scall to be added now and exit

   if (aSCall->mExecuteTime < mScheduleTime)
   {
      aSCall->execute();
      delete aSCall;
      return;
   }

   // Guard
   if (mNumOfElements >= ArraySize)
   {
      delete aSCall;
      return;
   }

   //---------------------------------------------------------------------------
   // Add the scall to the priority queue

   // Allocate an element array index
   int tElementIndex = 0;
   mAllocator.pop(tElementIndex);
   // Add the element to the element array at the allocated index
   mElement[tElementIndex] = aSCall;
   // Add the search keys to the search key arrays at the allocated index
   mSearchKey1[tElementIndex] = aSearchKey1;
   mSearchKey2[tElementIndex] = aSearchKey2;

   //---------------------------------------------------------------------------
   // Resolve the linked list indices

   // If the queue is empty
   if (mNumOfElements==0)
   {
      // Set the front index of the new element to point to itself
      mFrontNeighborIndex[tElementIndex] = tElementIndex;
      // Set the back  index of the new element to point to itself
      mBackNeighborIndex [tElementIndex] = tElementIndex;
      // Set the queue front index to point to the new element
      mFrontIndex                        = tElementIndex;
      // Set the queue back  index to point to the new element
      mBackIndex                         = tElementIndex;
      // Set to one
      mNumOfElements = 1;
   }
   // If the new element is at or later than (equal or lower priority) the back
   // element then add the new element after the queue back element
   else if (aSCall->mExecuteTime >= mElement[mBackIndex]->mExecuteTime)
   {
      // Set the front index of the new element to point to the current queue back element
      // element.front = back
      mFrontNeighborIndex [tElementIndex] = mBackIndex;
      // Set the back index of the new element to point to itself
      // element.back = element
      mBackNeighborIndex  [tElementIndex] = tElementIndex;
      // Set the back index of the current queue back element to point to the new element
      // back.back = element
      mBackNeighborIndex  [mBackIndex]    = tElementIndex;
      // Set the queue back index to point to the new element
      // back = element
      mBackIndex                          = tElementIndex;
      // Increment
      mNumOfElements++;
   }
   // If the new element is earlier than (higher priority) the front
   // element then add the new element before the queue front element
   else if(aSCall->mExecuteTime < mElement[mFrontIndex]->mExecuteTime)
   {
      // Set the back index of the new element to point to the current front element
      // element.back = front
      mBackNeighborIndex  [tElementIndex] = mFrontIndex;
      // Set the front index of the new element to point to itself
      // element.front = element
      mFrontNeighborIndex [tElementIndex] = tElementIndex;
      // Set the front index of the current queue front element to point to the new element
      // front.front = element
      mFrontNeighborIndex  [mFrontIndex]  = tElementIndex;
      // Set the queue front index to point to the new element
      // back = element
      mFrontIndex                         = tElementIndex;
      // Increment
      mNumOfElements++;
   }
   // The new element has to be inserted within the queue so the queue
   // is searched to find the entry point
   else
   {
      int  tSearchCount = 0;
      int  tSearchIndex = mBackIndex;
      bool tGoing       = true;
   
      if (aStartSearchAtBack)
      {
         // Start search at the back
         tSearchIndex = mBackIndex;
      }
      else
      {
        // Start search at the front
        tSearchIndex = mFrontIndex;
      }
   
      // Loop to search for the entry point from the back
      while (tGoing)
      {
         // If the new element is at or later than (equal or lower priority) the search 
         // element then insert the new element after the search element
         if(aSCall->mExecuteTime >= mElement[tSearchIndex]->mExecuteTime)
         {
            // Set the front index of the new element to point to the search element
            // element.front = search
            mFrontNeighborIndex [tElementIndex] = tSearchIndex;
            // Set the back index of the new element to point to the element at the back of the search element
            // element.back = search.back
            mBackNeighborIndex  [tElementIndex] = mBackNeighborIndex[tSearchIndex];
            // Set the back index of the element to the front of the new element to point to the new element
            // element.front.back = element
            mBackNeighborIndex  [mFrontNeighborIndex[tElementIndex]] = tElementIndex;
            // Set the front index of the element to the back of the new element to point to the new element
            // element.back.front = element
            mFrontNeighborIndex [mBackNeighborIndex[tElementIndex]]  = tElementIndex;
            // Increment
            mNumOfElements++;
            // Exit loop
            tGoing = false;
         }
         // If the new scall is earlier than (higher priority) the search element
         // then advance the search
         else
         {
            if (aStartSearchAtBack)
            {
               // Advance search toward front
               // search = search.front
               tSearchIndex = mFrontNeighborIndex [tSearchIndex];
            } 
            else
            {
               // Advance search toward back
               // search = search.back
               tSearchIndex = mBackNeighborIndex [tSearchIndex];
            } 
         }
         // Advance the loop count
         if (++tSearchCount==mNumOfElements)
         {
            // Guard
            tGoing=false;
         }
      }
   }
}

//******************************************************************************
// remove

bool Scheduler::remove(
   int  aSearchKey1,
   int  aSearchKey2,
   bool aStartSearchAtBack,
   bool aExecuteBeforeRemoval)
{
   //---------------------------------------------------------------------------
   // Search for the scall to remove from the priority queue

   // Guard
   if (mNumOfElements==0)return false;

   int  tRemoveIndex=0;
   bool tRemoveFlag=false;

   int  tSearchCount = 0;
   int  tSearchIndex;
   bool tGoing       = true;

   if (aStartSearchAtBack)
   {
      // Start search at the back
      tSearchIndex = mBackIndex;
   }
   else
   {
     // Start search at the front
     tSearchIndex = mFrontIndex;
   }

   // Loop to search for the element to remove
   while (tGoing)
   {
      // If the search keys are found then the loop finishes
      if(aSearchKey1==mSearchKey1[tSearchIndex] && aSearchKey2==mSearchKey2[tSearchIndex])
      {
         // Store the index of the element to remove
         tRemoveIndex = tSearchIndex;
         tRemoveFlag  = true;
         // Exit the loop
         tGoing       = false;
      }
      // If the search keys are not found then
      // advance the search index to the next element
      else
      {
         if (aStartSearchAtBack)
         {
            // Advance search toward front
            // search = search.front
            tSearchIndex = mFrontNeighborIndex [tSearchIndex];
         } 
         else
         {
            // Advance search toward back
            // search = search.back
            tSearchIndex = mBackNeighborIndex [tSearchIndex];
         } 
      }
      // Advance the loop count
      if (++tSearchCount==mNumOfElements)
      {
         // Guard
         tGoing=false;
      }
   }

   //---------------------------------------------------------------------------
   // Resolve the linked list indices

   // If element not found then return 
   if (!tRemoveFlag) return false;

   // If the priority queue only has one element
   if (mNumOfElements==1)
   {
      //Do nothing, the indices don't matter anymore
   }
   
   // If the element is at the front of the queue
   else if (tRemoveIndex==mFrontIndex)
   {
      //Set the queue front index to point to the element to the back of the removed element
      //front = remove.back
      mFrontIndex = mBackNeighborIndex [tRemoveIndex];
      //Set the front index of the queue front element to point to itself
      //front.front = front
      mFrontNeighborIndex [mFrontIndex] = mFrontIndex;
   }
   
   // Else If the element is at the back of the queue
   else if (tRemoveIndex==mBackIndex)
   {
      //Set the queue front index to point to the element to the back of the removed element
      //back = remove.front
      mBackIndex = mFrontNeighborIndex [tRemoveIndex];
      //Set the front index of the queue front element to point to itself
      //back.back = back
      mBackNeighborIndex [mBackIndex] = mBackIndex;
   }

   // Else the element is not at the front or the back
   else
   {
      //Set the back index of the element to the front of the removed element to 
      //point to the element to the back of the removed element
      //remove.front.back = remove.back
      mBackNeighborIndex  [mFrontNeighborIndex [tRemoveIndex]] = mBackNeighborIndex  [tRemoveIndex];
      //Set the front index of the element to the back of the removed element to 
      //point to the element at the front of the removed element
      //remove.back.front = remove.front
      mFrontNeighborIndex [mBackNeighborIndex  [tRemoveIndex]] = mFrontNeighborIndex [tRemoveIndex];
   }

   //---------------------------------------------------------------------------
   // Remove the element

   // Execute before removal
   if (aExecuteBeforeRemoval)
   {
      mElement[tRemoveIndex]->execute();
   }

   // Delete  the removed element
   delete mElement[tRemoveIndex];

   // Deallocate the removed element index
   mAllocator.push(tRemoveIndex);

   // Decrement
   mNumOfElements--;

   return true;
}

//******************************************************************************
// flush

void Scheduler::flush()
{
   // Guard
   if (mNumOfElements==0)return;

   // Loop through all of the elements in the queue
   // Start at the front of the queue
   int  tDeleteIndex = mFrontIndex;

   for (int i=0;i<mNumOfElements;i++)
   {
      // Delete the element
      delete mElement[tDeleteIndex];
      // Advance the index from front to back
      // index = index.back
      tDeleteIndex = mBackNeighborIndex [tDeleteIndex];
   }
   // Empty now
   mNumOfElements=0;
}

//******************************************************************************
// front

BaseSCall* Scheduler::front()
{
   // Return the element at the front of the queue
   return mElement[mFrontIndex];
}

//******************************************************************************
// getFrontSCall

void Scheduler::removeFront()
{
   // Guard
   if (mNumOfElements==0) mElement[0];

   // Store the front element index
   int tElementIndex = mFrontIndex;
   
   // Deallocate the front element index
   mAllocator.push(mFrontIndex);
   // Decrement
   mNumOfElements--;

   // Set the new front element index to point to the element to the back of the current front element    
   // front = front.back
   mFrontIndex = mBackNeighborIndex[mFrontIndex];
}
//******************************************************************************
// show

void Scheduler::show()
{
   Prn::print(0,0,"mScheduleTime %8.1f",mScheduleTime);
   Prn::print(0,0,"mNumOfElements %d",mNumOfElements);
   Prn::print(0,0,"mFrontIndex    %d",mFrontIndex);
   Prn::print(0,0,"mBackIndex     %d",mBackIndex);
   return;
   Prn::print(0,0,"");
   for(int i=0;i<mNumOfElements;i++)
   {
      Prn::print(0,0,"mNeighborIndex    %d  %d  %d",
         i,
         mFrontNeighborIndex[i],
         mBackNeighborIndex[i]);
   }
}
  
}//namespace
}//namespace

