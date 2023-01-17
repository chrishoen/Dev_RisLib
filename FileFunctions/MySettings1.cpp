//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risFileFunctions.h"

#include "MySettings1.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

MySettings1::MySettings1()
{
   reset();
}

void MySettings1::reset()
{
   if (Ris::portableIsWindows())
   {
      strcpy(mFilePath, "c:/aaa_prime/files/mysettings.json");
   }
   else
   {
      strcpy(mFilePath, "/opt/prime/files/mysettings.json");
   }
   mAlarmEnable = false;
   mBuzzerEnable = false;
   mRawInput = 9000;
   mInput = -60;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a json value derived from the class member variables.

Json::Value MySettings1::getJsonValueFromMembers()
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

void MySettings1::setMembersFromJsonValue(Json::Value aJsonValue)
{
   try
   {
      mAlarmEnable = aJsonValue["AlarmEnable"].asBool();
      mBuzzerEnable = aJsonValue["BuzzerEnable"].asBool();
      mRawInput = std::stof(aJsonValue["RawInput"].asString());
      mInput = std::stof(aJsonValue["Input"].asString());
   }
   catch (...)
   {
      Prn::print(0, "MySettings1::setMembersFromJsonValue EXCEPTION");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write the class member variables to the json file.
// Use file locks

void MySettings1::doWrite()
{
   // Get a json value from the member variables.
   Json::Value tJsonValue = getJsonValueFromMembers();

   // Write the json value to a string.
   std::string tString;
   std::stringstream tStream;
   Json::StreamWriterBuilder tBuilder;
   Json::StreamWriter* tWriter = tBuilder.newStreamWriter();
   tWriter->write(tJsonValue, &tStream);
   tStream << std::endl;
   tString = tStream.str();
   delete tWriter;

   // Open the file with a write lock. Write the string to the file. 
   // Unlock the file and close it.
   FILE* tFile = Ris::doOpenAndLockForWrite(mFilePath);
   fwrite(tString.c_str(), 1, tString.length(), tFile);
   Ris::doUnlockAndClose(tFile);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read the member variables from the json file. Use file locks.
// If the file doesn't exist then set the member variables to 
// defaults and write the file so that it does exist. This will
// create a file for the first time with defaults.

void MySettings1::doRead()
{
   // If the json file doesn't exist then set the member variables
   // to defaults, write them to the file, and exit. This will
   // create a file for the first time with defaults.
   if (!Ris::portableFilePathExists(mFilePath))
   {
      Prn::print(0, "MySettings1::doRead NOT EXIST, writing defaults");
      reset();
      doWrite();
      return;
   }

   // Open the file with a read lock. Read from the file into a buffer. 
   // Unlock the file and close it.
   FILE* tFile = Ris::doOpenAndLockForRead(mFilePath);
   char* tBuffer = new char[1001];
   int tRet = (int)fread(tBuffer, 1, 1000, tFile);
   if (tRet < 0)
   {
      Prn::print(0, "MySettings1::doRead FAIL1");
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
   delete tReader;
   delete[] tBuffer;
   if (!tPass)
   {
      Prn::print(0, "MySettings1::doRead FAIL2 parse error");
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

void MySettings1::show()
{
   Prn::print(0, "");
   Prn::print(0, "BuzzerEnable           %s", my_string_from_bool(mBuzzerEnable));
   Prn::print(0, "AlarmEnable            %s", my_string_from_bool(mAlarmEnable));
   Prn::print(0, "RawInput               %.1f", mRawInput);
   Prn::print(0, "Input                  %.1f", mInput);
   Prn::print(0, "");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
