//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>

#include "my_functions.h"
#include "risCmdLineFile.h"
#include "risPortableCalls.h"

#define  _EXAMPLESETTINGS_CPP_
#include "exampleSettings.h"

namespace ExampleMsg
{

//******************************************************************************

Settings::Settings()
{
   mSection[0]=0;

   mMyAppNumber = 0;

   mMyUdpIPAddress[0]=0;
   mMyUdpPort = 0;

   mOtherUdpIPAddress[0]=0;
   mOtherUdpPort = 0;
}

void Settings::show()
{
   printf("Settings ******* %s\n", mSection);

   printf("MyAppNumber               %d\n", mMyAppNumber);

   printf("MyUdp      %-12s   %d\n",mMyUdpIPAddress,mMyUdpPort);
   printf("OtherUdp   %-12s   %d\n",mOtherUdpIPAddress,mOtherUdpPort);

   printf("Settings ******* %s\n", mSection);
}

//******************************************************************************
// For a given command line "Begin Section", this returns true
// if "Section", the first command line argument, is the same as the section 
// specified in initialize.

bool Settings::isMySection(Ris::CmdLineCmd* aCmd)
{
   bool tFlag=false;

   if (aCmd->numArg()==1)
   {
      if (aCmd->isArgString(1,mSection))
      {
         tFlag=true;
      }
   }

   return tFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is called for each command line in the settings file.
// It processes commands, with arguments
// BEGIN starts a section, END exits a section
// Only commands for a section are processed

void Settings::execute(Ris::CmdLineCmd* aCmd)
{
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Section commands


	if(aCmd->isCmd("SectionBegin"      ))  mSectionFlag=isMySection(aCmd);
   if(aCmd->isCmd("SectionEnd"        ))  mSectionFlag=false;

   if (!mSectionFlag) return;

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Only process commands for the section specified in initialize.

   if(aCmd->isCmd("MyAppNumber"     ))   mMyAppNumber = aCmd->argInt(1);
   if(aCmd->isCmd("MyUdp"           ))   executeOnMyUdp     (aCmd);
   if(aCmd->isCmd("OtherUdp"        ))   executeOnOtherUdp  (aCmd);
}



//******************************************************************************
//******************************************************************************
//******************************************************************************
// Specific execute 

void Settings::executeOnMyUdp(Ris::CmdLineCmd* aCmd)
{
   aCmd->copyArgString(1, mMyUdpIPAddress,cMaxStringSize);
   mMyUdpPort = aCmd->argInt(2);
}

void Settings::executeOnOtherUdp(Ris::CmdLineCmd* aCmd)
{
   aCmd->copyArgString(1, mOtherUdpIPAddress,cMaxStringSize);
   mOtherUdpPort = aCmd->argInt(2);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool Settings::initialize(char* aSection)
{ 
   // File path
   char tFilePath[200];

   strcpy(tFilePath, Ris::portableGetSettingsDir());
   strcat(tFilePath, "exampleSettings.txt");

   // Copy arguments
   strcpy(mSection,aSection);
   // Apply settings file to this executive   
   Ris::CmdLineFile tCmdLineFile;

   if (tCmdLineFile.open(tFilePath))
   {
      tCmdLineFile.execute(this);
      tCmdLineFile.close();
      return true;
   }
   else
   {
      printf("Settings::file open failed\n");
      return false;
   }
}

}//namespace



