/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include "GSettings.h"

void example()
{
   int    tN =   0;
   double tX = 0.0;
   bool   tF = false;

   // Access
   tN = gGSettings.mCode1;
   tN = gGSettings.mPLT.mCode1;

   tF = gGSettings.mPLT.mOuterEncoder.mInverted;
   tN = gGSettings.mPLT.mOuterEncoder.mOffsetAngle;
   tX = gGSettings.mPLT.mOuterEncoder.mPinAngle;

   tX = gGSettings.mPLT.mFog_ScaleFactor[0];

   tX = gGSettings.mPLT.mAccelX_ConvFactor;
   tX = gGSettings.mPLT.mMHD1_ScaleFactor;
   tX = gGSettings.mPLT.mOuterMotor_ScaleFactor;

   tX = gGSettings.mPLT.mOuterNotchFilter.mUD;

   tX = gGSettings.mPLT.mOuterServoLoop.mK1;

   // Read only alias
   tN = getGSettings().mCode1;
   tN = getGSettings().mPLT.mCode1;

   tF = getGSettings().mPLT.mOuterEncoder.mInverted;
   tN = getGSettings().mPLT.mOuterEncoder.mOffsetAngle;
   tX = getGSettings().mPLT.mOuterEncoder.mPinAngle;

   tX = getGSettings().mPLT.mFog_ScaleFactor[0];

   tX = getGSettings().mPLT.mAccelX_ConvFactor;
   tX = getGSettings().mPLT.mMHD1_ScaleFactor;
   tX = getGSettings().mPLT.mOuterMotor_ScaleFactor;

   tX = getGSettings().mPLT.mOuterNotchFilter.mUD;

   tX = getGSettings().mPLT.mOuterServoLoop.mK1;

}
