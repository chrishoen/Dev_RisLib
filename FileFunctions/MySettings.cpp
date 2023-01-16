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

   mAlarmEnable = false;
   mBuzzerEnable = false;
   mRawInput = 9000;
   mInput = -60;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a json value from the class member variables.

Json::Value MySettings::getJsonValueFromMembers()
{
   // Json variables.
   Json::Value tJsonValue;
   char tBuffer[40];

   // Copy members.
   tJsonValue["AlarmEnable"] = mAlarmEnable;
   tJsonValue["BuzzerEnable"] = mBuzzerEnable;
   tJsonValue["RawInput"] = my_string_from_float(tBuffer, "%.2f", mRawInput);
   tJsonValue["Input"] = my_string_from_float(tBuffer, "%.2f", mInput);

   // Done.
   return tJsonValue;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the class member variables from a json value.

void MySettings::setMembersFromJsonValue(Json::Value aJsonValue)
{
   try
   {
      // Copy members.
      mAlarmEnable = aJsonValue["AlarmEnable"].asBool();
      mBuzzerEnable = aJsonValue["BuzzerEnable"].asBool();
      mRawInput = std::stof(aJsonValue["RawInput"].asString());
      mInput = std::stof(aJsonValue["Input"].asString());
   }
   catch (...)
   {
      Prn::print(mPF, "MySettings::setMembersFromJsonValue EXCEPTION");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a string with json for all of the variables.

std::string MySettings::getJsonString()
{
   // Json variable.
   Json::Value tJsonValue = getJsonValueFromMembers();

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
   // Json variable.
   Json::Value tJsonValue = getJsonValueFromMembers();

   // Write the json variable to a string.
   std::string tString;
   Json::FastWriter tWriter;
   tString = tWriter.write(tJsonValue);

   FILE* tFile = Ris::doOpenAndLockForWrite(mFilePath);
   fwrite(tString.c_str(), 1, tString.length(), tFile);
   Ris::doUnlockAndClose(tFile);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read from the json file.

void MySettings::doRead()
{
   // If the json file doesn't exist then set the class member variables
   // to defaults, write the class to the file, and exit. This will
   // create a file for the first time with defaults.
   if (!Ris::portableFilePathExists(mFilePath))
   {
      Prn::print(mPF, "MySettings::doRead NOT EXIST, writing defaults");
      initialize();
      doWrite();
      mValidFlag = true;
      return;
   }

   // Open the file with a read lock. Read from the file into a buffer. 
   // Unlock the file and close it.
   FILE* tFile = Ris::doOpenAndLockForRead(mFilePath);
   char* tBuffer = new char[1001];
   int tRet = fread(tBuffer, 1, 1000, tFile);
   if (tRet < 0)
   {
      Prn::print(mPF, "MySettings::doRead FAIL1");
      Ris::doUnlockAndClose(tFile);
      return;
   }
   Ris::doUnlockAndClose(tFile);

   // Parse the buffer into a json value.
   char* tBeginDoc = tBuffer;
   char* tEndDoc = tBuffer + tRet;
   Json::Value tJsonValue;
   std::string tError;
   Json::CharReaderBuilder tBuilder;
   Json::CharReader* tReader = tBuilder.newCharReader();
   bool tPass = tReader->parse(tBeginDoc, tEndDoc, &tJsonValue, &tError);
   delete[] tBuffer;
   delete tReader;
   if (!tPass)
   {
      Prn::print(mPF, "MySettings::doRead FAIL2 parse error");
      return;
   }

   // Set the class member variables from the json value.
   setMembersFromJsonValue(tJsonValue);
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
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
