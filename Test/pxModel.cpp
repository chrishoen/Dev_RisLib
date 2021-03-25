/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineCmd.h"

#define  _PXMODEL_CPP_
#include "pxModel.h"

namespace PX
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Model::Model()
{
   reset();
}

void Model::reset()
{
   mEnable = true;
   mPF = 1;
   mAdjustZ = 0.050;
   mFirstFlag = true;
   mUpFlag = false;
   mLastUpFlag = false;
   mUpChangeFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Modify a gcode.Return true if there was a change.

bool Model::doModify(char* aGCodeString)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.
   
   // Guard.
   if (!mEnable) return false;

   // Copy the gcode string into a command line command and parse it.
   Ris::CmdLineCmd* tCmd = new Ris::CmdLineCmd(aGCodeString);

   // Guard.
   if (!(tCmd->isCmd("G1") && tCmd->gcodeIsValidZ()))
   {
      Prn::print(mPF, "doModify************************IGNORE");
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Process the direction.

   // Locals.
   double tInputZ = tCmd->gcodeDoubleZ();
   double tOutputZ = 0;

   Prn::print(mPF, "doModify************************  %.3f", tInputZ);

   // If first.
   if (mFirstFlag)
   {
      mFirstFlag = false;
      mUpFlag = tInputZ > 0;
      mLastUpFlag = tInputZ > 0;
   }
   // If not first.
   else
   {
      mLastUpFlag = mUpFlag;
      mUpFlag = tInputZ > 0;
   }

   // Test for a change in direction.
   mUpChangeFlag = mUpFlag != mLastUpFlag;

   // If no change then done.
   if (!mUpChangeFlag) return false;


   // Adjust the z.
   if (mUpFlag)
   {
      Prn::print(mPF, "change up");
      tOutputZ = tInputZ + mAdjustZ;
   }
   else
   {
      Prn::print(mPF, "change down");
      tOutputZ = tInputZ - mAdjustZ;
   }

   // Modify the gcode string.
   sprintf(aGCodeString, "G1 Z%.3f", tOutputZ);

   // Done.
   return true;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace