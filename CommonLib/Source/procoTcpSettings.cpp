//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"


#define  _PROCOTCPSETTINGS_CPP_
#include "procoTcpSettings.h"

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

TcpSettings::TcpSettings()
{
   reset();
}

void TcpSettings::reset()
{
   BaseClass::reset();
   strcpy(BaseClass::mDefaultFileName, "ProtoComm_TcpSettings.txt");

   mMyAppNumber = 0;
   mMyAppRole = 0;

   mTcpServerIPAddress[0]=0;
   mTcpServerPort = 0;
   mTcpMaxSessions = 0;

   mMyUdpIPAddress[0]=0;
   mMyUdpPort = 0;

   mOtherUdpIPAddress[0]=0;
   mOtherUdpPort = 0;

   mSerialPortDevice[0] = 0;
   mSerialPortSetup[0] = 0;
   mSerialRxTimeout = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void TcpSettings::show()
{
   printf("\n");
   printf("TcpSettings************************************************ %s\n", mTargetSection);

   printf("MyAppNumber               %5d\n", mMyAppNumber);
   printf("MyAppRole                 %5s\n", asStringAppRole(mMyAppRole));

   printf("TcpServer          %-12s   %5d\n", mTcpServerIPAddress, mTcpServerPort);
   printf("TcpMaxSessions             %5d\n", mTcpMaxSessions);

   printf("MyUdp              %-12s   %5d\n",mMyUdpIPAddress,mMyUdpPort);
   printf("OtherUdp           %-12s   %5d\n",mOtherUdpIPAddress,mOtherUdpPort);

   printf("SerialPortDevice           %-12s\n", mSerialPortDevice);
   printf("SerialPortSetup            %-12s\n", mSerialPortSetup);
   printf("SerialRxTimeout            %5d\n",   mSerialRxTimeout);
   printf("TcpSettings************************************************\n");
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void TcpSettings::execute(Ris::CmdLineCmd* aCmd)
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

   if (aCmd->isCmd("TcpMaxSessions")) mTcpMaxSessions = aCmd->argInt(1);

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

   if (aCmd->isCmd("SerialPortDevice"))  aCmd->copyArgString(1, mSerialPortDevice, cMaxStringSize);
   if (aCmd->isCmd("SerialPortSetup"))   aCmd->copyArgString(1, mSerialPortSetup, cMaxStringSize);
   if (aCmd->isCmd("SerialRxTimeout"))   mSerialRxTimeout = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void TcpSettings::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* TcpSettings::asStringAppRole(int aX)
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