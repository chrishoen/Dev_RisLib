/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrint.h"

#include "risLogic.h"

namespace Ris
{
namespace Logic
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

AndOrLatch::AndOrLatch()
{
   reset();
}

//******************************************************************************
// Return flags

bool AndOrLatch::isAny() {return mAnyFlag;}
bool AndOrLatch::isAll() {return mAllFlag;}

//******************************************************************************
// Resets

void AndOrLatch::reset()
{
   mAnyFlag=false;
   mAllFlag=false;

   for (int i=0;i<LatchSize;i++)
   {
      mMask[i]=false;
      mLatch[i]=false;
   }
}

void AndOrLatch::resetMask()
{
   mAnyFlag=false;
   mAllFlag=false;

   for (int i=0;i<LatchSize;i++)
   {
      mMask[i]=false;
   }
}

void AndOrLatch::resetLatch()
{
   mAnyFlag=false;
   mAllFlag=false;

   for (int i=0;i<LatchSize;i++)
   {
      mLatch[i]=false;
   }
}


//******************************************************************************
// Set mask bit

void AndOrLatch::setMask(int aIndex, bool aValue)
{
   mMask[aIndex] = aValue;
}

//******************************************************************************
// Set latch bit and update the condition flags

void AndOrLatch::setLatch(int aIndex, bool aValue)
{
   mLatch[aIndex] = aValue;

   mAnyFlag = false;
   mAllFlag = true;

   // For each masked latch bit, update condition flags
   for (int i=0;i<LatchSize;i++)
   {
      if (mMask[i])
      {
         if (mLatch[i])
         {
            mAnyFlag = true;
         }
         else
         {
            mAllFlag = false;
         }
      }
   }
}

}//namespace
}//namespace

