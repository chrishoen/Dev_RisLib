/*==============================================================================

==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "my_functions.h"
#include "risCmdLineFile.h"
#include "risPortableCalls.h"

#include "PLT_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

PLT_Encoder_Settings::PLT_Encoder_Settings()
{
   mInverted = true;
   mOffsetAngle = 0;
   mPinAngle = 0.0;
}

void PLT_Encoder_Settings::show(char* aLabel)
{
   printf("%-20s Inverted           %6s\n",   aLabel,my_string_from_bool(mInverted));
   printf("%-20s OffsetAngle        %6d\n",   aLabel,mOffsetAngle);
   printf("%-20s PinAngle           %6.1f\n", aLabel,mPinAngle);
   printf("\n");
}

//******************************************************************************

void PLT_Encoder_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("End"))            nestedPop(aCmd);

   if (aCmd->isCmd("Inverted"))       mInverted     = aCmd->argBool(1);
   if (aCmd->isCmd("OffsetAngle"))    mOffsetAngle  = aCmd->argUnsigned(1);
   if (aCmd->isCmd("PinAngle"))       mPinAngle     = aCmd->argDouble(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

PLT_MotorWinding_Settings::PLT_MotorWinding_Settings()
{
   mSinPositive = true;
   mCosPositive = true;
   mOffsetAngle = 0;
}

void PLT_MotorWinding_Settings::show(char* aLabel)
{
   printf("%-20s SinPositive        %6s\n",   aLabel,my_string_from_bool(mSinPositive));
   printf("%-20s CosPositive        %6s\n",   aLabel,my_string_from_bool(mCosPositive));
   printf("%-20s OffsetAngle        %6d\n",   aLabel,mOffsetAngle);
   printf("\n");
}

//******************************************************************************

void PLT_MotorWinding_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("End"))            nestedPop(aCmd);

   if (aCmd->isCmd("SinPositive"))    mSinPositive  = aCmd->argBool(1);
   if (aCmd->isCmd("CosPositive"))    mCosPositive  = aCmd->argBool(1);
   if (aCmd->isCmd("OffsetAngle"))    mOffsetAngle  = aCmd->argUnsigned(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

PLT_NotchFilter_Settings::PLT_NotchFilter_Settings()
{
   mUD = 0.0;
   mLD = 0.0;
   mW1 = 0.0;
}

void PLT_NotchFilter_Settings::show(char* aLabel)
{
   printf("%-20s UD               %8.4f\n", aLabel,mUD);
   printf("%-20s LD               %8.4f\n", aLabel,mLD);
   printf("%-20s W1               %8.4f\n", aLabel,mW1);
   printf("\n");
}

//******************************************************************************

void PLT_NotchFilter_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))  nestedPop(aCmd);

   if (aCmd->isCmd("UD"))   mUD = aCmd->argDouble(1);
   if (aCmd->isCmd("LD"))   mLD = aCmd->argDouble(1);
   if (aCmd->isCmd("W1"))   mW1 = aCmd->argDouble(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

PLT_ServoLoop_Settings::PLT_ServoLoop_Settings()
{
   mK1 = 0.0;
   mW3 = 0.0;
   mW4 = 0.0;
   mW5 = 0.0;
   mW6 = 0.0;
   mW7 = 0.0;
   mW8 = 0.0;
}

void PLT_ServoLoop_Settings::show(char* aLabel)
{
   printf("%-20s K1            %8.4e\n",aLabel,mK1);
   printf("%-20s W3               %8.1f\n",aLabel,mW3);
   printf("%-20s W4               %8.1f\n",aLabel,mW4);
   printf("%-20s W5               %8.1f\n",aLabel,mW5);
   printf("%-20s W6               %8.1f\n",aLabel,mW6);
   printf("%-20s W7               %8.1f\n",aLabel,mW7);
   printf("%-20s W8               %8.1f\n",aLabel,mW8);
   printf("\n");
}

//******************************************************************************

void PLT_ServoLoop_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))  nestedPop(aCmd);

   if (aCmd->isCmd("K1"))   mK1 = aCmd->argDouble(1);
   if (aCmd->isCmd("W3"))   mW3 = aCmd->argDouble(1);
   if (aCmd->isCmd("W4"))   mW4 = aCmd->argDouble(1);
   if (aCmd->isCmd("W5"))   mW5 = aCmd->argDouble(1);
   if (aCmd->isCmd("W6"))   mW6 = aCmd->argDouble(1);
   if (aCmd->isCmd("W7"))   mW7 = aCmd->argDouble(1);
   if (aCmd->isCmd("W8"))   mW8 = aCmd->argDouble(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

PLT_Settings::PLT_Settings()
{
   mCode1 = 0;
   mCode2 = 0;

   mMHD1Present = false;
   mMHD2Present = false;
   mMHD3Present = false;

   mMEMSFramePresent = false;
   mMEMSOuterPresent = false;
   mMEMSMiddlePresent = false;

   mFOGMHDMixingFreq = 0.0;

   mFeedFwdEnable_Friction = true;
   mFeedFwdEnable_MassImbalance = true;
   mFeedFwdEnable_Reaction = true;
   mFeedFwdEnable_Rotation = true;

   mOuterNumOfPoles = 0;
   mMiddleNumOfPoles = 0;
   mInnerNumOfPoles = 0;

   mFog_ScaleFactor [0] = 1.0;
   mFog_ScaleFactor [1] = 1.0;
   mFog_ScaleFactor [2] = 1.0;

   mAccelX_ConvFactor = 0.0;
   mAccelY_ConvFactor = 0.0;
   mAccelZ_ConvFactor = 0.0;

   mMHD1_ScaleFactor = 0.0;
   mMHD2_ScaleFactor = 0.0;
   mMHD3_ScaleFactor = 0.0;

   mOuterMotor_ScaleFactor = 0.0;
   mMiddleMotor_ScaleFactor = 0.0;
   mInnerMotor_ScaleFactor = 0.0;

   mOmegaR2 = 0.0;
   mInitialServoGain = 0.0;
   mFinalServoGain = 0.0;
   mStabTorqueUpperLimit = 0.0;
   mOuterTorqueUpperLimit = 0.0;
   mOuterSaturatedTorqueTrigger = 0.0;

   mBinnTemperatureSetPointBias=0.0;
}

void PLT_Settings::show()
{
   printf("\n");
   printf("PLT_Settings****************************************************************\n");
   printf("\n");

   printf("Code1               %d\n", mCode1);
   printf("Code2               %d\n", mCode2);
   printf("\n");

   mOuterEncoder.show  ("OuterEncoder");
   mMiddleEncoder.show ("MiddleEncoder");
   mInnerEncoder.show  ("InnerEncoder");
   printf("\n");

   mOuterMotorWinding.show  ("OuterMotorWinding");
   mMiddleMotorWinding.show ("MiddleMotorWinding");
   mInnerMotorWinding.show  ("InnerMotorWinding");
   printf("\n");

   printf("Fog1_ScaleFactor                      %8.1f\n",   mFog_ScaleFactor[0]);
   printf("Fog2_ScaleFactor                      %8.1f\n",   mFog_ScaleFactor[1]);
   printf("Fog3_ScaleFactor                      %8.1f\n",   mFog_ScaleFactor[2]);
   printf("\n");

   printf("AccelX_ConvFactor                    %8.6f\n",    mAccelX_ConvFactor);
   printf("AccelY_ConvFactor                    %8.6f\n",    mAccelY_ConvFactor);
   printf("AccelZ_ConvFactor                    %8.6f\n",    mAccelZ_ConvFactor);
   printf("\n");

   printf("MHD1_ScaleFactor                     %8.6f\n",    mMHD1_ScaleFactor);
   printf("MHD2_ScaleFactor                     %8.6f\n",    mMHD2_ScaleFactor);
   printf("MHD3_ScaleFactor                     %8.6f\n",    mMHD3_ScaleFactor);
   printf("\n");

   printf("OuterMotor_ScaleFactor                %8.5f\n",   mOuterMotor_ScaleFactor);
   printf("MiddleMotor_ScaleFactor               %8.5f\n",   mMiddleMotor_ScaleFactor);
   printf("InnerMotor_ScaleFactor                %8.5f\n",   mInnerMotor_ScaleFactor);
   printf("\n");

   printf("OmegaR2                               %8.4f\n",   mOmegaR2);
   printf("InitialServoGain                     %8.2e\n",    mInitialServoGain);
   printf("FinalServoGain                       %8.2e\n",    mFinalServoGain);
   printf("StabTorqueUpperLimit                  %8.4f\n",   mStabTorqueUpperLimit);
   printf("OuterTorqueUpperLimit                 %8.4f\n",   mOuterTorqueUpperLimit);
   printf("OuterSaturatedTorqueTrigger           %8.4f\n",   mOuterSaturatedTorqueTrigger);
   printf("\n");

   mOuterNotchFilter.show   ("OuterNotchFilter");
   mInner1NotchFilter.show  ("Inner1NotchFilter");
   mInner2NotchFilter.show  ("Inner2NotchFilter");
   printf("\n");

   printf("OuterNumOfPoles                       %8d\n",     mOuterNumOfPoles);
   printf("MiddleNumOfPoles                      %8d\n",     mMiddleNumOfPoles);
   printf("InnerNumOfPoles                       %8d\n",     mInnerNumOfPoles);
   printf("\n");

   printf("MHD1Present                           %8s\n",     my_string_from_bool(mMHD1Present));
   printf("MHD2Present                           %8s\n",     my_string_from_bool(mMHD2Present));
   printf("MHD3Present                           %8s\n",     my_string_from_bool(mMHD3Present));
   printf("\n");

   printf("MEMSFramePresent                      %8s\n",     my_string_from_bool(mMEMSFramePresent));
   printf("MEMSOuterPresent                      %8s\n",     my_string_from_bool(mMEMSOuterPresent));
   printf("MEMSMiddlePresent                     %8s\n",     my_string_from_bool(mMEMSMiddlePresent));
   printf("\n");

   printf("FOGMHDMixingFreq                      %8.1f\n",   mFOGMHDMixingFreq);
   printf("\n");

   printf("FeedFwdEnable_Friction                %8s\n",     my_string_from_bool(mFeedFwdEnable_Friction));
   printf("FeedFwdEnable_MassImbalance           %8s\n",     my_string_from_bool(mFeedFwdEnable_MassImbalance));
   printf("FeedFwdEnable_Reaction                %8s\n",     my_string_from_bool(mFeedFwdEnable_Reaction));
   printf("FeedFwdEnable_Rotation                %8s\n",     my_string_from_bool(mFeedFwdEnable_Rotation));
   printf("\n");

   mOuterServoLoop.show  ("OuterServoLoop");
   mMiddleServoLoop.show ("MiddleServoLoop");
   mInnerServoLoop.show  ("InnerServoLoop");
   printf("\n");

   printf("BinnTemperatureSetPointBias           %8.4f\n",   mBinnTemperatureSetPointBias);
}

//******************************************************************************
// Base class overload, called for each line in the file

void PLT_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("End"))                 nestedPop(aCmd);

   if (aCmd->isCmd("Begin"))               executeOnBegin(aCmd);

   // Read variables
   if (aCmd->isCmd("Code1"))               mCode1      = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))               mCode2      = aCmd->argInt(1);

   if (aCmd->isCmd("Fog1_ScaleFactor"))             mFog_ScaleFactor[0]      = aCmd->argDouble(1);
   if (aCmd->isCmd("Fog2_ScaleFactor"))             mFog_ScaleFactor[1]      = aCmd->argDouble(1);
   if (aCmd->isCmd("Fog3_ScaleFactor"))             mFog_ScaleFactor[2]      = aCmd->argDouble(1);

   if (aCmd->isCmd("AccelX_ConvFactor"))            mAccelX_ConvFactor       = aCmd->argDouble(1);
   if (aCmd->isCmd("AccelY_ConvFactor"))            mAccelY_ConvFactor       = aCmd->argDouble(1);
   if (aCmd->isCmd("AccelZ_ConvFactor"))            mAccelZ_ConvFactor       = aCmd->argDouble(1);
   if (aCmd->isCmd("MHD1_ScaleFactor"))             mMHD1_ScaleFactor        = aCmd->argDouble(1);
   if (aCmd->isCmd("MHD2_ScaleFactor"))             mMHD2_ScaleFactor        = aCmd->argDouble(1);
   if (aCmd->isCmd("MHD3_ScaleFactor"))             mMHD3_ScaleFactor        = aCmd->argDouble(1);
   if (aCmd->isCmd("OuterMotor_ScaleFactor"))       mOuterMotor_ScaleFactor  = aCmd->argDouble(1);
   if (aCmd->isCmd("MiddleMotor_ScaleFactor"))      mMiddleMotor_ScaleFactor = aCmd->argDouble(1);
   if (aCmd->isCmd("InnerMotor_ScaleFactor"))       mInnerMotor_ScaleFactor  = aCmd->argDouble(1);

   if (aCmd->isCmd("OmegaR2"))                      mOmegaR2                     = aCmd->argDouble(1);
   if (aCmd->isCmd("InitialServoGain"))             mInitialServoGain            = aCmd->argDouble(1);
   if (aCmd->isCmd("FinalServoGain"))               mFinalServoGain              = aCmd->argDouble(1);
   if (aCmd->isCmd("StabTorqueUpperLimit"))         mStabTorqueUpperLimit        = aCmd->argDouble(1);
   if (aCmd->isCmd("OuterTorqueUpperLimit"))        mOuterTorqueUpperLimit       = aCmd->argDouble(1);
   if (aCmd->isCmd("OuterSaturatedTorqueTrigger"))  mOuterSaturatedTorqueTrigger = aCmd->argDouble(1);

   if (aCmd->isCmd("OuterNotchFilter"))    mOuterNotchFilter.execute(aCmd);
   if (aCmd->isCmd("Inner1NotchFilter"))   mInner1NotchFilter.execute(aCmd);
   if (aCmd->isCmd("Inner2NotchFilter"))   mInner2NotchFilter.execute(aCmd);

   if (aCmd->isCmd("OuterNumOfPoles"))     mOuterNumOfPoles = aCmd->argInt(1);
   if (aCmd->isCmd("MiddleNumOfPoles"))    mMiddleNumOfPoles = aCmd->argInt(1);
   if (aCmd->isCmd("InnerNumOfPoles"))     mInnerNumOfPoles = aCmd->argInt(1);

   if (aCmd->isCmd("MHD1Present"))         mMHD1Present       = aCmd->argBool(1);
   if (aCmd->isCmd("MHD2Present"))         mMHD2Present       = aCmd->argBool(1);
   if (aCmd->isCmd("MHD3Present"))         mMHD3Present       = aCmd->argBool(1);

   if (aCmd->isCmd("MEMSFramePresent"))    mMEMSFramePresent  = aCmd->argBool(1);
   if (aCmd->isCmd("MEMSOuterPresent"))    mMEMSOuterPresent  = aCmd->argBool(1);
   if (aCmd->isCmd("MEMSMiddlePresent"))   mMEMSMiddlePresent = aCmd->argBool(1);

   if (aCmd->isCmd("FOGMHDMixingFreq"))    mFOGMHDMixingFreq = aCmd->argDouble(1);

   if (aCmd->isCmd("FeedFwdEnable_Friction"))       mFeedFwdEnable_Friction = aCmd->argBool(1);
   if (aCmd->isCmd("FeedFwdEnable_MassImbalance"))  mFeedFwdEnable_MassImbalance = aCmd->argBool(1);
   if (aCmd->isCmd("FeedFwdEnable_Reaction"))       mFeedFwdEnable_Reaction = aCmd->argBool(1);
   if (aCmd->isCmd("FeedFwdEnable_Rotation"))       mFeedFwdEnable_Rotation = aCmd->argBool(1);

   if (aCmd->isCmd("BinnTemperatureSetPointBias"))  mBinnTemperatureSetPointBias = aCmd->argDouble(1);
}

//******************************************************************************
// Execute specific command

void PLT_Settings::executeOnBegin(Ris::CmdLineCmd* aCmd)
{
   //------------------------------------------------------
   if      (aCmd->isArgString(1, "OuterEncoder"))
   {
      nestedPush(aCmd,&mOuterEncoder);
   }
   else if (aCmd->isArgString(1, "MiddleEncoder"))
   {
      nestedPush(aCmd,&mMiddleEncoder);
   }
   else if (aCmd->isArgString(1, "InnerEncoder"))
   {
      nestedPush(aCmd,&mInnerEncoder);
   }

   //------------------------------------------------------
   if      (aCmd->isArgString(1, "OuterMotorWinding"))
   {
      nestedPush(aCmd,&mOuterMotorWinding);
   }
   else if (aCmd->isArgString(1, "MiddleMotorWinding"))
   {
      nestedPush(aCmd,&mMiddleMotorWinding);
   }
   else if (aCmd->isArgString(1, "InnerMotorWinding"))
   {
      nestedPush(aCmd,&mInnerMotorWinding);
   }

   //------------------------------------------------------
   else if (aCmd->isArgString(1, "OuterServoLoop"))
   {
      nestedPush(aCmd,&mOuterServoLoop);
   }
   else if (aCmd->isArgString(1, "MiddleServoLoop"))
   {
      nestedPush(aCmd,&mMiddleServoLoop);
   }
   else if (aCmd->isArgString(1, "InnerServoLoop"))
   {
      nestedPush(aCmd,&mInnerServoLoop);
   }

   //------------------------------------------------------
   else if (aCmd->isArgString(1, "OuterNotchFilter"))
   {
      nestedPush(aCmd,&mOuterNotchFilter);
   }
   else if (aCmd->isArgString(1, "Inner1NotchFilter"))
   {
      nestedPush(aCmd,&mInner1NotchFilter);
   }
   else if (aCmd->isArgString(1, "Inner2NotchFilter"))
   {
      nestedPush(aCmd,&mInner2NotchFilter);
   }
}

