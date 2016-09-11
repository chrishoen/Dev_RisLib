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

#define  _GSETTINGS_CPP_
#include "GSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

GSettings::GSettings()
{
   mValidFlag = false;
   mCode1     = 0;
   mCode2     = 0;
   mVersion   = 0;
   mShowAtInitialize=false;
}

//******************************************************************************

void GSettings::show()
{
   printf("\n");
   printf("GSettings*******************************************************************\n");
   printf("\n");

   printf("Code1               %d\n",mCode1);
   printf("Code2               %d\n",mCode2);
   printf("Version             %d\n",mVersion);
   printf("ShowAtInitialize    %s\n",my_string_from_bool(mShowAtInitialize));

   mSystem.show();
   mNetwork.show();
   mTransforms.show();
   mFpga.show();
   mPLT.show();
   mDIAG.show();
   mSCP.show();
   mRTC.show();
   mGGI.show();
   mDevIO.show();
}

//******************************************************************************
// Base class overload, called for each line in the file

void GSettings::execute(Ris::CmdLineCmd* aCmd)
{
   // Read variables
   if(aCmd->isCmd("Code1"))             mCode1   = aCmd->argInt (1);
   if(aCmd->isCmd("Code2"))             mCode2   = aCmd->argInt (1);
   if(aCmd->isCmd("Version"))           mVersion = aCmd->argInt (1);
   if(aCmd->isCmd("ShowAtInitialize"))  mShowAtInitialize = aCmd->argBool (1);
   // Begin section
   if(aCmd->isCmd("BEGIN"))             executeOnBegin(aCmd);
}

//******************************************************************************

void GSettings::executeOnBegin(Ris::CmdLineCmd* aCmd)
{
   if      (aCmd->isArgString(1, "System_Settings"))
   {
      nestedPush(aCmd,&mSystem);
   }
   else if (aCmd->isArgString(1, "Network_Settings"))
   {
      nestedPush(aCmd,&mNetwork);
   }
   else if (aCmd->isArgString(1, "Transforms_Settings"))
   {
      nestedPush(aCmd,&mTransforms);
   }
   else if (aCmd->isArgString(1, "Fpga_Settings"))
   {
      nestedPush(aCmd,&mFpga);
   }
   else if (aCmd->isArgString(1, "PLT_Settings"))
   {
      nestedPush(aCmd,&mPLT);
   }
   else if (aCmd->isArgString(1, "SCP_Settings"))
   {
      nestedPush(aCmd,&mSCP);
   }
   else if (aCmd->isArgString(1, "DIAG_Settings"))
   {
      nestedPush(aCmd,&mDIAG);
   }
   else if (aCmd->isArgString(1, "GGI_Settings"))
   {
      nestedPush(aCmd,&mGGI);
   }
   else if (aCmd->isArgString(1, "RTC_Settings"))
   {
      nestedPush(aCmd,&mRTC);
   }
   else if (aCmd->isArgString(1, "DevIO_Settings"))
   {
     nestedPush(aCmd, &mDevIO);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void GSettings::readFromDefaultFile()
{
   readFromFileName("GravSettings.txt");
}

//******************************************************************************

void GSettings::readFromFileName(char* aFileName)
{
   char tFilePath[200];

   strcpy(tFilePath, Ris::portableGetSettingsDir());

   if (aFileName != 0)
   {
      strcat(tFilePath, aFileName);
   }
   else
   {
      strcat(tFilePath, "GravSettings.txt");
   }

   readFromFilePath(tFilePath);
}

//******************************************************************************

void GSettings::readFromFilePath(char* aFilePath)
{ 
   Ris::CmdLineFile tCmdLineFile;

   // Open command line file
   if (!tCmdLineFile.open(aFilePath))
   {
      //printf("GSettings::readFromFilePath FAIL %s\n",aFilePath);
      mValidFlag = false;
      return;
   }

   // Read command line file, execute commands for each line in the file,
   // using this command line executive
   tCmdLineFile.execute(this);

   // Close command line file
   tCmdLineFile.close();

   //printf("GSettings::readFromFilePath PASS %s\n", aFilePath);
   mValidFlag = true;

   if (mShowAtInitialize)
   {
      show();
   }
}

