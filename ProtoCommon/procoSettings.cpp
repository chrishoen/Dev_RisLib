//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>

#include "my_functions.h"
#include "risCmdLineFile.h"
#include "risPortableCalls.h"
#include "procoDefs.h"


#define  _PROCOSETTINGS_CPP_
#include "procoSettings.h"

namespace ProtoComm
{

//******************************************************************************

ProtoCommSettings::ProtoCommSettings()
{
   mSection[0]=0;

   mMyAppNumber = 0;
   mMyAppRole = 0;

   mTcpServerIPAddress[0]=0;
   mTcpServerPort = 0;

   mMyUdpIPAddress[0]=0;
   mMyUdpPort = 0;

   mOtherUdpIPAddress[0]=0;
   mOtherUdpPort = 0;
}

void ProtoCommSettings::show()
{
   printf("ProtoCommSettings************************************************ %s\n", mSection);
   printf("MyAppNumber               %d\n", mMyAppNumber);
   printf("MyAppRole                 %s\n", getAppRoleNameByEnum(mMyAppRole));

   printf("TcpServer  %-12s   %d\n",mTcpServerIPAddress,mTcpServerPort);

   printf("MyUdp      %-12s   %d\n",mMyUdpIPAddress,mMyUdpPort);
   printf("OtherUdp   %-12s   %d\n",mOtherUdpIPAddress,mOtherUdpPort);
}

//******************************************************************************
// For a given command line "Begin Section", this returns true
// if "Section", the first command line argument, is the same as the section 
// specified in initialize.

bool ProtoCommSettings::isMySection(Ris::CmdLineCmd* aCmd)
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

void ProtoCommSettings::execute(Ris::CmdLineCmd* aCmd)
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
   if(aCmd->isCmd("MyAppRole"       ))   mMyAppRole   = getAppRoleEnumByName(aCmd->argString(1));

   if(aCmd->isCmd("TcpServer"       ))   executeOnTcpServer (aCmd);
   if(aCmd->isCmd("MyUdp"           ))   executeOnMyUdp     (aCmd);
   if(aCmd->isCmd("OtherUdp"        ))   executeOnOtherUdp  (aCmd);
}



//******************************************************************************
//******************************************************************************
//******************************************************************************
// Specific execute 

void ProtoCommSettings::executeOnTcpServer(Ris::CmdLineCmd* aCmd)
{
   aCmd->copyArgString(1, mTcpServerIPAddress);
   mTcpServerPort = aCmd->argInt(2);
}

void ProtoCommSettings::executeOnMyUdp(Ris::CmdLineCmd* aCmd)
{
   aCmd->copyArgString(1, mMyUdpIPAddress);
   mMyUdpPort = aCmd->argInt(2);
}

void ProtoCommSettings::executeOnOtherUdp(Ris::CmdLineCmd* aCmd)
{
   aCmd->copyArgString(1, mOtherUdpIPAddress);
   mOtherUdpPort = aCmd->argInt(2);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool ProtoCommSettings::initialize(char* aSection)
{ 
   // File path
   char tFilePath[200];

   strcpy(tFilePath, Ris::portableGetSettingsDir());
   strcat(tFilePath, "procoSettings.txt");

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
      printf("ProtoCommSettings::file open failed\n");
      return false;
   }
}

}//namespace



