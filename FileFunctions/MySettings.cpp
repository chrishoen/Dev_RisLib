//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risFileFunctions.h"

#include "MySettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

void MySettings::initialize()
{
   mValidFlag = false;
   
   strcpy(mFilePath,"/opt/prime/files/mysettings.json");
   strcpy(mLockFileName, "mysettings");
   mLockFile = 0;
   mPF = 0;

   mAlarmEnable = false;
   mBuzzerEnable = false;
   mRawInput = 9000;
   mInput = -60;
   mOverrideInput = -60;
   mThreshHi = -99;
   mThreshLo = -60;
   mInRange = false;
   mDetect = false;
   mCState = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

// Lock the json file and read from it.
void MySettings::doReadModifyWriteBegin()
{
   mLockFile = Ris::doLockFile_OpenAndLockForWrite(mLockFileName);
   doRead();
}

// Perform the calculation, write to the json file, and unlock it.
void MySettings::doReadModifyWriteEnd()
{
   doWrite();
   Ris::doLockFile_UnlockAndClose(mLockFile);
}

// Lock the json file, read from it, and unlock it.
void MySettings::doProtectedRead()
{
   mLockFile = Ris::doLockFile_OpenAndLockForRead(mLockFileName);
   doRead();
   Ris::doLockFile_UnlockAndClose(mLockFile);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a json value for all of the variables.

Json::Value MySettings::getJsonValue()
{
   // Json variables.
   Json::Value tJsonValue;
   char tBuffer[40];

   // Copy members.
   tJsonValue["AlarmEnable"] = mAlarmEnable;
   tJsonValue["BuzzerEnable"] = mBuzzerEnable;
   tJsonValue["RawInput"] = my_string_from_float(tBuffer, "%.2f", mRawInput);
   tJsonValue["Input"] = my_string_from_float(tBuffer, "%.2f", mInput);
   tJsonValue["ThreshHi"] = my_string_from_float(tBuffer, "%.2f", mThreshHi);
   tJsonValue["ThreshLo"] = my_string_from_float(tBuffer, "%.2f", mThreshLo);
   tJsonValue["InRange"] = mInRange;
   tJsonValue["Detect"] = mDetect;
   tJsonValue["CState"] = mCState;

   // Done.
   return tJsonValue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a string with json for all of the variables.

std::string MySettings::getJsonString()
{
   // If this is not valid then read from the json file.
   if (!mValidFlag)
   {
      doProtectedRead();
   }

   // Json variable.
   Json::Value tJsonValue = getJsonValue();

   std::string tString;
   Json::FastWriter tWriter;
   tString = tWriter.write(tJsonValue);
   return tString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the json file.

void MySettings::doWrite()
{
   // Json variables.
   Json::Value tJsonValue;
   char tBuffer[40];

   // Copy members.
   tJsonValue["AlarmEnable"] = mAlarmEnable;
   tJsonValue["BuzzerEnable"] = mBuzzerEnable;
   tJsonValue["ThreshHi"] = my_string_from_float(tBuffer, "%.2f", mThreshHi);

   // Write to json file.
   Ris::doWriteJsonToFile(
      tJsonValue,           // Input
      mFilePath);           // Input
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read from the json file.

void MySettings::doRead()
{
   // Json variables.
   Json::Value tJsonValue;

   // If the json file doesn't exist.
   if (!Ris::portableFilePathExists(mFilePath))
   {
      Prn::print(mPF, "MySettings::doRead NOT EXIST");
      // Write to the file, so that it exists.
      initialize();
      doWrite();
      mValidFlag = true;
      return;
   }

   try
   {
      // Read from json file.
      Ris::doReadJsonFromFile(
         tJsonValue,           // Output
         mFilePath);           // Input

      // Copy members.
      mAlarmEnable = tJsonValue["AlarmEnable"].asBool();
      mBuzzerEnable = tJsonValue["BuzzerEnable"].asBool();
      mThreshHi = std::stof(tJsonValue["ThreshHi"].asString());
   }
   catch (...)
   {
      Prn::print(mPF, "MySettings::doRead EXCEPTION");
   }

   mValidFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void MySettings::show()
{
   if (!Prn::getFilter(mPF)) return;
   Prn::print(mPF, "");

   Prn::print(mPF, "BuzzerEnable           %s", my_string_from_bool(mBuzzerEnable));
   Prn::print(mPF, "AlarmEnable            %s", my_string_from_bool(mAlarmEnable));
   Prn::print(mPF, "RawInput               %.1f", mRawInput);
   Prn::print(mPF, "Input                  %.1f", mInput);
   Prn::print(mPF, "ThreshHi               %.1f", mThreshHi);
   Prn::print(mPF, "ThreshLo               %.1f", mThreshLo);
   Prn::print(mPF, "InRange                %s",   my_string_from_bool(mInRange));
   Prn::print(mPF, "Detect                 %s",   my_string_from_bool(mDetect));
   Prn::print(mPF, "CState                 %s",   my_string_from_bool(mCState));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
