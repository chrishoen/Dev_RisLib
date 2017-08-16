//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"


#define  _PROCOSETTINGS_CPP_
#include "protoserialSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoSerial
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
   strcpy(BaseClass::mDefaultFileName, "ProtoSerial_Settings.txt");

   mMyAppNumber = 0;
   mMyAppRole = 0;

   mTcpServerIPAddress[0]=0;
   mTcpServerPort = 0;

   mMyUdpIPAddress[0]=0;
   mMyUdpPort = 0;

   mOtherUdpIPAddress[0]=0;
   mOtherUdpPort = 0;

   mSerialPortNumber = 0;
   mSerialPortSetup[0] = 0;
   mSerialRxTimeout = -1;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Settings::show()
{
   printf("\n");
   printf("Settings************************************************ %s\n", mTargetSection);

   printf("MyAppNumber               %5d\n", mMyAppNumber);
   printf("MyAppRole                 %5s\n", asStringAppRole(mMyAppRole));

   printf("TcpServer          %-12s   %5d\n",mTcpServerIPAddress,mTcpServerPort);

   printf("MyUdp              %-12s   %5d\n",mMyUdpIPAddress,mMyUdpPort);
   printf("OtherUdp           %-12s   %5d\n",mOtherUdpIPAddress,mOtherUdpPort);

   printf("SerialPort                 %5d\n",  mSerialPortNumber);
   printf("SerialPort                 %-12s\n",mSerialPortSetup);
   printf("SerialRxTimeout            %5d\n",  mSerialRxTimeout);
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

   if (aCmd->isCmd("MyAppNumber"))   mMyAppNumber = aCmd->argInt(1);

   if (aCmd->isCmd("MyAppRole"))
   {
      if (aCmd->isArgString(1,asStringAppRole(cTcpServer)))   mMyAppRole = cTcpServer;
      if (aCmd->isArgString(1,asStringAppRole(cTcpClient)))   mMyAppRole = cTcpClient;
      if (aCmd->isArgString(1,asStringAppRole(cUdpPeer)))     mMyAppRole = cUdpPeer;
   }

   if (aCmd->isCmd("TcpServer"))
   {
      aCmd->copyArgString(1, mTcpServerIPAddress,cMaxStringSize);
      mTcpServerPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("MyUdp"))
   {
      aCmd->copyArgString(1, mMyUdpIPAddress,cMaxStringSize);
      mMyUdpPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("OtherUdp"))
   {
      aCmd->copyArgString(1, mOtherUdpIPAddress,cMaxStringSize);
      mOtherUdpPort = aCmd->argInt(2);
   }

   if (aCmd->isCmd("SerialPortNumber"))  mSerialPortNumber = aCmd->argInt(1);
   if (aCmd->isCmd("SerialPortSetup"))   aCmd->copyArgString(1, mSerialPortSetup,cMaxStringSize);
   if (aCmd->isCmd("SerialRxTimeout"))   mSerialRxTimeout = aCmd->argInt(1);
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
   case cTcpServer         : return "TcpServer";
   case cTcpClient         : return "TcpClient";
   case cUdpPeer           : return "UdpPeer";
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