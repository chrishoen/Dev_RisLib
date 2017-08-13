//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"


#define  _PROCOSETTINGS_CPP_
#include "procoSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Settings::Settings()
{
   reset();
}

void Settings::reset()
{
   BaseClass::reset();
   strcpy(BaseClass::mDefaultFileName, "ProtoCommSettings.txt");

   mMyAppNumber = 0;
   mMyAppRole = 0;

   mTcpServerIPAddress[0]=0;
   mTcpServerPort = 0;

   mMyUdpIPAddress[0]=0;
   mMyUdpPort = 0;

   mOtherUdpIPAddress[0]=0;
   mOtherUdpPort = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Settings::show()
{
   printf("\n");
   printf("Settings************************************************ %s\n", mTargetSection);

   printf("MyAppNumber               %d\n", mMyAppNumber);
   printf("MyAppRole                 %s\n", asStringAppRole(mMyAppRole));

   printf("TcpServer  %-12s   %d\n",mTcpServerIPAddress,mTcpServerPort);

   printf("MyUdp      %-12s   %d\n",mMyUdpIPAddress,mMyUdpPort);
   printf("OtherUdp   %-12s   %d\n",mOtherUdpIPAddress,mOtherUdpPort);

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("TcpServer"))
   {
      aCmd->copyArgString(1, mTcpServerIPAddress,cMaxStringSize);
      mTcpServerPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("MyUpd"))
   {
      aCmd->copyArgString(1, mMyUdpIPAddress,cMaxStringSize);
      mMyUdpPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("OtherUpd"))
   {
      aCmd->copyArgString(1, mOtherUdpIPAddress,cMaxStringSize);
      mOtherUdpPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("AppRole"))
   {
      if (aCmd->isArgString(1,asStringAppRole(cTcpServer)))   mMyAppRole = cTcpServer;
      if (aCmd->isArgString(1,asStringAppRole(cTcpClient)))   mMyAppRole = cTcpClient;
      if (aCmd->isArgString(1,asStringAppRole(cUdpPeer)))     mMyAppRole = cUdpPeer;
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void Settings::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* Settings::asStringAppRole(int aX)
{
   switch (aX)
   {
   case cNone              : return "None";
   case cTcpServer         : return "cTcpServer";
   case cTcpClient         : return "cTcpClient";
   case cUdpPeer           : return "cUdpPeer";
   default : return "UNKNOWN";
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace