#pragma once

/*==============================================================================
Settings json file class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include "json.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class contains a set of member variables that are stored in a
// json file. The file is intended to have multiple writers and multiple
// readers.

class MySettings2
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   static const int cMaxStringSize = 200;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Json file.
   char mFilePath[cMaxStringSize];
   FILE* mFile;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members. These are read from/written to the json file.

   // If true then the alarm is enabled.
   bool mAlarmEnable;

   // If true then the buzzer is enabled.
   bool mBuzzerEnable;

   // Uncalibrated Measurement input.
   float mRawInput;

   // Calibrated measurement input.
   float mInput;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   MySettings2();
   void reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Begin a read/modify/write operation. Read member variables
   // from the json file.
   // 
   // If the json file doesn't exist then set the member variables to 
   // defaults, open and lock the file, and exit.
   // If the file does exist then open and lock the file, read the
   // member variables from the file, and exit.
   // After this executes the file remains locked until a call to
   // doReadModifyWriteBegin.
   void doReadModifyWriteBegin();

   // End a read/modify/write operation. Write modified member variables
   // to the json file.
   //
   // Reopen the file to the beginning, truncating it.
   // Write the member variables to the file.
   // Unlock and close the file.
   void doReadModifyWriteEnd();

   // Read the member variables from the json file. Use file locks.
   // If the file doesn't exist then set the member variables to 
   // defaults and write the file so that it does exist. This will
   // create a file for the first time with defaults.
   void doRead();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Helpers.

   // Return a json value from the member variables.
   Json::Value getJsonValueFromMembers();

   // Set the member variables from a json value.
   void setMembersFromJsonValue(Json::Value aValue);

   // Print the member variables.
   void show();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
