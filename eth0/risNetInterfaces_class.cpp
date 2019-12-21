//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "json.h"
#include "risFileFunctions.h"

#include "risNetInterfaces.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

Interfaces::Interfaces()
{
   if (Ris::portableIsWindows())
   {
      mFilePath = "c:/aaa_cproc/files/netinterfaces.json";
   }
   else
   {
      mFilePath = "/opt/prime/files/netinterfaces.json";
   }

   mEth0DhcpFlag = false;
   mWlan0DhcpFlag = false;
   mEth0Address = "none";
   mEth0Mask = "none";
   mWlan0Address = "none";
   mWlan0Mask = "none";
   mGateway = "none";
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the json file.

void Interfaces::doWrite()
{
   // Json variables.
   Json::Value tJsonValue;

   // Copy members.
   tJsonValue["Eth0DhcpFlag"] = mEth0DhcpFlag;
   tJsonValue["Wlan0DhcpFlag"] = mWlan0DhcpFlag;
   tJsonValue["Eth0Address"] = mEth0Address;
   tJsonValue["Eth0Mask"] = mEth0Mask;
   tJsonValue["Wlan0Address"] = mWlan0Address;
   tJsonValue["Wlan0Mask"] = mWlan0Mask;
   tJsonValue["Gateway"] = mGateway;

   // Write to json file.
   Ris::doWriteJsonToFile(
      tJsonValue,           // Input
      mFilePath);           // Input
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read from the json file.

void Interfaces::doRead()
{
   // Json variables.
   Json::Value tJsonValue;

   // Guard.
   if (!Ris::portableFilePathExists(mFilePath.c_str())) return;

   // Read from json file.
   Ris::doReadJsonFromFile(
      tJsonValue,           // Output
      mFilePath);           // Input

   // Copy members.
   mEth0DhcpFlag = tJsonValue["Eth0DhcpFlag"].asBool();
   mWlan0DhcpFlag = tJsonValue["Wlan0DhcpFlag"].asBool();
   mEth0Address = tJsonValue["Eth0Address"].asString();
   mEth0Mask = tJsonValue["Eth0Mask"].asString();
   mWlan0Address = tJsonValue["Wlan0Address"].asString();
   mWlan0Mask = tJsonValue["Wlan0Mask"].asString();
   mGateway = tJsonValue["Gateway"].asString();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Interfaces::show(int aPF)
{
   Prn::print(aPF, "");
   Prn::print(aPF, "FilePath %s", mFilePath.c_str());

   Prn::print(aPF, "Eth0DhcpFlag            %s", my_string_from_bool(mEth0DhcpFlag));
   Prn::print(aPF, "Wlan0DhcpFlag           %s", my_string_from_bool(mWlan0DhcpFlag));
   Prn::print(aPF, "Eth0Address             %s", mEth0Address.c_str());
   Prn::print(aPF, "Eth0Mask                %s", mEth0Mask.c_str());
   Prn::print(aPF, "Wlan0Address            %s", mWlan0Address.c_str());
   Prn::print(aPF, "Wlan0Mask               %s", mWlan0Mask.c_str());
   Prn::print(aPF, "Gateway                 %s", mGateway.c_str());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
