#ifndef _PLT_SETTINGS_H_
#define _PLT_SETTINGS_H_

/*==============================================================================

Global settings file, PLT specific section.

==============================================================================*/
#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Encoder settings class

class PLT_Encoder_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members.

   bool           mInverted;
   unsigned int   mOffsetAngle;
   double         mPinAngle;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   PLT_Encoder_Settings();
   void show(char* aLabel);

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute(Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// MotorWinding settings class

class PLT_MotorWinding_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members.

   bool            mSinPositive;
   bool            mCosPositive;
   unsigned int    mOffsetAngle;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   PLT_MotorWinding_Settings();
   void show(char* aLabel);

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute(Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// NotchFilter settings class

class PLT_NotchFilter_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members.

   double  mUD;
   double  mLD;
   double  mW1;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   PLT_NotchFilter_Settings();
   void show(char* aLabel);

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute(Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Servo loop parameters settings class

class PLT_ServoLoop_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members.

   double  mK1;
   double  mW3;
   double  mW4;
   double  mW5;
   double  mW6;
   double  mW7;
   double  mW8;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   PLT_ServoLoop_Settings();
   void show(char* aLabel);

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute(Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
// Settings for the PLT CSC

class PLT_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding CSC section of
   // the settings file

   // Integrity checks
   int   mCode1;
   int   mCode2;

   // Encoders
   PLT_Encoder_Settings mOuterEncoder;
   PLT_Encoder_Settings mMiddleEncoder;
   PLT_Encoder_Settings mInnerEncoder;

   // MotorWinding
   PLT_MotorWinding_Settings mOuterMotorWinding;
   PLT_MotorWinding_Settings mMiddleMotorWinding;
   PLT_MotorWinding_Settings mInnerMotorWinding;

   // FOGs
   double  mFog_ScaleFactor[3];
   double  mFog_RateLimit[3];//new

   // Scale/conversion factors
   double  mAccelX_ConvFactor;
   double  mAccelY_ConvFactor;
   double  mAccelZ_ConvFactor;

   double  mMHD1_ScaleFactor;
   double  mMHD2_ScaleFactor;
   double  mMHD3_ScaleFactor;

   double  mOuterMotor_ScaleFactor;
   double  mMiddleMotor_ScaleFactor;
   double  mInnerMotor_ScaleFactor;

   double  mOmegaR2;
   double  mInitialServoGain;
   double  mFinalServoGain;
   double  mStabTorqueUpperLimit;
   double  mOuterTorqueUpperLimit;
   double  mOuterSaturatedTorqueTrigger;


   // Notch filters
   PLT_NotchFilter_Settings mOuterNotchFilter;
   PLT_NotchFilter_Settings mInner1NotchFilter;
   PLT_NotchFilter_Settings mInner2NotchFilter;

   // Number of poles
   int     mOuterNumOfPoles;
   int     mMiddleNumOfPoles;
   int     mInnerNumOfPoles;

   // MHDs
   bool    mMHD1Present;
   bool    mMHD2Present;
   bool    mMHD3Present;

   // MEMS
   bool    mMEMSFramePresent;
   bool    mMEMSOuterPresent;
   bool    mMEMSMiddlePresent;

   // FOG/MHD mixing frequency
   double  mFOGMHDMixingFreq;

   // Feed forward enables
   bool    mFeedFwdEnable_Friction;
   bool    mFeedFwdEnable_MassImbalance;
   bool    mFeedFwdEnable_Reaction;
   bool    mFeedFwdEnable_Rotation;

   // Servo loop parameters
   PLT_ServoLoop_Settings mOuterServoLoop;
   PLT_ServoLoop_Settings mMiddleServoLoop;
   PLT_ServoLoop_Settings mInnerServoLoop;

   double mBinnTemperatureSetPointBias;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   PLT_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute (Ris::CmdLineCmd* aCmd);

   // Execute specific commands
   void executeOnBegin           (Ris::CmdLineCmd* aCmd);

};

#endif

