#ifndef _RISLOGIC_H_
#define _RISLOGIC_H_

/*==============================================================================

These are analagous to digital logic components.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Logic
{

//******************************************************************************
// This class provides an AND OR masked latch.
// It has a mask array and a latch array and some condition flags.
// Initially mask bits are set and over time latch bits are set.
// The masked latch bits are ANDed and ORed to set the condition flags
// that are used to indicate if ANY or ALL masked latch bits are set.
// ANY is the OR  of all of the masked latch bits.
// ALL is the AND of all of the masked latch bits.
//
// For example:
//
//    reset();
//
//    setMask(2,true);
//    setMask(3,true);
//
//    setLatch(2,false);
//    setLatch(3,true);
//    
//    isAny() returns true
//    isAll() returns false
//
//    setLatch(2,true);
//    setLatch(3,true);
//    
//    isAny() returns true
//    isAll() returns true
//******************************************************************************

class AndOrLatch
{
public:
   AndOrLatch();

   //--------------------------------------------------------------
   // Interface:

   // Resets
   void reset();
   void resetMask();
   void resetLatch();

   // Mask and latch
   void setMask  (int aIndex, bool aValue);
   void setLatch (int aIndex, bool aValue);

   // Returns true if any masked latch value  is true.
   // It ORs  the masked latch values. 
   bool isAny();
   // Returns true if all masked latch values are true.
   // It ANDs the masked latch values. 
   bool isAll();
   
   // Condition flags
   bool mAnyFlag;  // True if any masked latch bit  is  true
   bool mAllFlag;  // True if all masked latch bits are true

   //--------------------------------------------------------------
   // Members:

   enum {LatchSize=32};

   // Latch array
   bool  mLatch[LatchSize];      

   // Mask array
   bool  mMask[LatchSize];      
};

}//namespace
}//namespace


#endif

