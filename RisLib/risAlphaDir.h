#pragma once

/*==============================================================================
This unit provides a set of functions that provide management of a
relocatable alpha directory to be used by programs that use rislib.
==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constants.

   // Max string size used in subsequent calls.
   static const int cMaxAlphaStringSize = 400;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Alpha directory management functions.
//

   // Set the alpha directory path. All subsequent calls will be relative
   // to this.
   void setAlphaDirectory(char* aDirPath);

   // Return the alpha directory path. The input is a temp char buffer.
   // The buffer is written to with the requested string to and a pointer
   // to it is returned.
   char* getAlphaDirectory();

   // Return a file path to a file that lives in alpha. The first input is a
   // temp char buffer. The second input is an file name. If the file name is
   // null then the dir path is returned. The buffer is written to with the 
   // requested string to and a pointer to it is returned.
   char* getAlphaFilePath_Bin         (char* aBuffer, const char* aFileName = 0);
   char* getAlphaFilePath_Calibration (char* aBuffer, const char* aFileName = 0);
   char* getAlphaFilePath_Data        (char* aBuffer, const char* aFileName = 0);
   char* getAlphaFilePath_DLL         (char* aBuffer, const char* aFileName = 0);
   char* getAlphaFilePath_Files       (char* aBuffer, const char* aFileName = 0);
   char* getAlphaFilePath_Image       (char* aBuffer, const char* aFileName = 0);
   char* getAlphaFilePath_Lib         (char* aBuffer, const char* aFileName = 0);
   char* getAlphaFilePath_Log         (char* aBuffer, const char* aFileName = 0);
   char* getAlphaFilePath_RecData     (char* aBuffer, const char* aFileName = 0);
   char* getAlphaFilePath_Settings    (char* aBuffer, const char* aFileName = 0);
   char* getAlphaFilePath_TestImage   (char* aBuffer, const char* aFileName = 0);
   char* getAlphaFilePath_Work        (char* aBuffer, const char* aFileName = 0);


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

