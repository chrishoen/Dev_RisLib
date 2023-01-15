#pragma once

/*==============================================================================
Auxiliary alarm info.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <string>
#include "json.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class contains auxiliary alarm configuration and state information.
// This class is designed such that instances will reside in shared memory. 

class MySettings
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 100;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // True if valid.
   bool mValidFlag;

   // Json file path.
   char mFilePath[cMaxStringSize];
   char mLockFileName[cMaxStringSize];
   int mLockFile;

   // Instrumentation.
   int mPF;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If true then the alarm is enabled.
   bool mAlarmEnable;

   // If true then the buzzer is enabled.
   bool mBuzzerEnable;

   // Uncalibrated Measurement input.
   float mRawInput;

   // Calibrated measurement input.
   float mInput;

   // Measurement input override.
   float mOverrideInput;

   // High level carrier measurement thresholds.
   float mThreshHi;
   float mThreshLo;

   // If true then the input is above threshold low.
   bool mInRange;

   // If true then the input is above threshold high.
   bool mDetect;

   // Condition state. If true then the alarm is active.
   bool mCState;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Initialize, there's no constructor. This is called once by the process
   // who first creates the shared memory.
   void initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Lock the json file and read from it.
   void doReadModifyWriteBegin();

   // Perform the calculation, write to the json file, and unlock it.
   void doReadModifyWriteEnd();

   // Lock the json file, read from it, and unlock it.
   void doProtectedRead();

   // Read from the json file.
   void doRead();

   // Write to the json file.
   void doWrite();

   // Return a json value for all of the variables.
   Json::Value getJsonValue();

   // Return a string with json for all of the variables.
   std::string getJsonString();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Support.
   void show();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
