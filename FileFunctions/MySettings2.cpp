//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "risFileFunctions.h"

#include "MySettings2.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

MySettings2::MySettings2()
{
   reset();
}

void MySettings2::reset()
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

Json::Value MySettings2::getJsonValueFromMembers()
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

void MySettings2::setMembersFromJsonValue(Json::Value aJsonValue)
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
      Prn::print(0, "MySettings2::setMembersFromJsonValue EXCEPTION");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Begin a read/modify/write operation. Read member variables
// from the json file.
// 
// If the json file doesn't exist then set the member variables to 
// defaults, open and lock the file, and exit.
// If the file does exist then open and lock the file, read the
// member variables from the file, and exit.
// After this executes the file remains locked until a call to
// doReadModifyWriteBegin.

void MySettings2::doReadModifyWriteBegin()
{
   // If the json file doesn't exist then set the member variables
   // to defaults, open the file with a write lock, and exit. This will
   // create a file for the first time with defaults.
   if (!Ris::portableFilePathExists(mFilePath))
   {
      Prn::print(0, "MySettings2::doRead NOT EXIST, writing defaults");
      reset();
      mFile = Ris::doOpenAndLockForWrite(mFilePath);
      return;
   }

   // Open the file with a write lock. Read from the file into a buffer. 
   mFile = Ris::doOpenAndLockForReadWrite(mFilePath);
   char* tBuffer = new char[1001];
   int tRet = (int)fread(tBuffer, 1, 1000, mFile);
   if (tRet < 0)
   {
      Prn::print(0, "MySettings2::doRead FAIL1");
      Ris::doUnlockAndClose(mFile);
      return;
   }

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
      Prn::print(0, "MySettings2::doRead FAIL2 parse error");
      return;
   }

   // Set the class member variables from the json value.
   setMembersFromJsonValue(tJsonValue);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// End a read/modify/write operation. Write modified member variables
// to the json file.
//
// Reopen the file to the beginning, truncating it.
// Write the member variables to the file.
// Unlock and close the file.

void MySettings2::doReadModifyWriteEnd()
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

   // Reopen the file for write. This seeks to the beginning and truncates
   // but does not change the file lock.
   // Write the string to the file. 
   // Unlock the file and close it.
   freopen(0, "w", mFile);
   fwrite(tString.c_str(), 1, tString.length(), mFile);
   Ris::doUnlockAndClose(mFile);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read the member variables from the json file. Use file locks.
// If the file doesn't exist then set the member variables to 
// defaults and write to the file so that it does exist. This will
// create a file for the first time with defaults.

void MySettings2::doRead()
{
   // If the json file doesn't exist then set the member variables
   // to defaults, write them to the file, and exit. This will
   // create a file for the first time with defaults.
   if (!Ris::portableFilePathExists(mFilePath))
   {
      Prn::print(0, "MySettings2::doRead NOT EXIST, writing defaults");
      reset();
      doReadModifyWriteBegin();
      doReadModifyWriteEnd();
      return;
   }

   // Open the file with a read lock. Read from the file into a buffer. 
   // Unlock the file and close it.
   FILE* tFile = Ris::doOpenAndLockForRead(mFilePath);
   char* tBuffer = new char[1001];
   int tRet = (int)fread(tBuffer, 1, 1000, tFile);
   if (tRet < 0)
   {
      Prn::print(0, "MySettings2::doRead FAIL1");
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
      Prn::print(0, "MySettings2::doRead FAIL2 parse error");
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

void MySettings2::show()
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
