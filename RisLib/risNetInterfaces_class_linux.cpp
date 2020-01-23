//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "json.h"
#include "risPortableCalls.h"
#include "risFileFunctions.h"
#include "my_functions.h"
#include "prnPrint.h"

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

   mEth0ValidFlag = false;
   mEth0DhcpFlag = false;
   mEth0Address = "none";
   mEth0Mask = "none";
   mEth0Gateway = "none";
   mEth0WantAddress = "none";
   mEth0WantMask = "none";
   mEth0WantGateway = "none";
   mWlan0ValidFlag = false;
   mWlan0DhcpFlag = false;
   mWlan0Address = "none";
   mWlan0Mask = "none";
   mWlan0Gateway = "none";
   mWlan0WantAddress = "none";
   mWlan0WantMask = "none";
   mWlan0WantGateway = "none";
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
   tJsonValue["Eth0ValidFlag"]    = mEth0ValidFlag;
   tJsonValue["Eth0DhcpFlag"]     = mEth0DhcpFlag;
   tJsonValue["Eth0Address"]      = mEth0Address;
   tJsonValue["Eth0Mask"]         = mEth0Mask;
   tJsonValue["Eth0Gateway"]      = mEth0Gateway;
   tJsonValue["Eth0WantAddress"]  = mEth0WantAddress;
   tJsonValue["Eth0WantMask"]     = mEth0WantMask;
   tJsonValue["Eth0WantGateway"]  = mEth0WantGateway;
   tJsonValue["Wlan0ValidFlag"]   = mWlan0ValidFlag;
   tJsonValue["Wlan0DhcpFlag"]    = mWlan0DhcpFlag;
   tJsonValue["Wlan0Address"]     = mWlan0Address;
   tJsonValue["Wlan0Mask"]        = mWlan0Mask;
   tJsonValue["Wlan0Gateway"]     = mWlan0Gateway;
   tJsonValue["Wlan0WantAddress"] = mWlan0WantAddress;
   tJsonValue["Wlan0WantMask"]    = mWlan0WantMask;
   tJsonValue["Wlan0WantGateway"] = mWlan0WantGateway;

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
   mEth0ValidFlag    = tJsonValue["Eth0ValidFlag"].asBool();
   mEth0DhcpFlag     = tJsonValue["Eth0DhcpFlag"].asBool();
   mEth0Address      = tJsonValue["Eth0Address"].asString();
   mEth0Mask         = tJsonValue["Eth0Mask"].asString();
   mEth0Gateway      = tJsonValue["Eth0Gateway"].asString();
   mEth0WantAddress  = tJsonValue["Eth0WantAddress"].asString();
   mEth0WantMask     = tJsonValue["Eth0WantMask"].asString();
   mEth0WantGateway  = tJsonValue["Eth0WantGateway"].asString();
   mWlan0ValidFlag   = tJsonValue["Wlan0ValidFlag"].asBool();
   mWlan0DhcpFlag    = tJsonValue["Wlan0DhcpFlag"].asBool();
   mWlan0Address     = tJsonValue["Wlan0Address"].asString();
   mWlan0Mask        = tJsonValue["Wlan0Mask"].asString();
   mWlan0Gateway     = tJsonValue["Wlan0Gateway"].asString();
   mWlan0WantAddress = tJsonValue["Wlan0WantAddress"].asString();
   mWlan0WantMask    = tJsonValue["Wlan0WantMask"].asString();
   mWlan0WantGateway = tJsonValue["Wlan0WantGateway"].asString();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Interfaces::show(int aPF)
{
   Prn::print(aPF, "");
   Prn::print(aPF, "FilePath %s", mFilePath.c_str());

   Prn::print(aPF, "Eth0ValidFlag           %s", my_string_from_bool(mEth0ValidFlag));
   Prn::print(aPF, "Eth0DhcpFlag            %s", my_string_from_bool(mEth0DhcpFlag));
   Prn::print(aPF, "Eth0Address             %s", mEth0Address.c_str());
   Prn::print(aPF, "Eth0Mask                %s", mEth0Mask.c_str());
   Prn::print(aPF, "Eth0Gateway             %s", mEth0Gateway.c_str());
   Prn::print(aPF, "Eth0WantAddress         %s", mEth0WantAddress.c_str());
   Prn::print(aPF, "Eth0WantMask            %s", mEth0WantMask.c_str());
   Prn::print(aPF, "Eth0WantGateway         %s", mEth0WantGateway.c_str());
   Prn::print(aPF, "Wlan0ValidFlag          %s", my_string_from_bool(mWlan0ValidFlag));
   Prn::print(aPF, "Wlan0DhcpFlag           %s", my_string_from_bool(mWlan0DhcpFlag));
   Prn::print(aPF, "Wlan0Address            %s", mWlan0Address.c_str());
   Prn::print(aPF, "Wlan0Mask               %s", mWlan0Mask.c_str());
   Prn::print(aPF, "Wlan0Gateway            %s", mWlan0Gateway.c_str());
   Prn::print(aPF, "Wlan0WantAddress        %s", mWlan0WantAddress.c_str());
   Prn::print(aPF, "Wlan0WantMask           %s", mWlan0WantMask.c_str());
   Prn::print(aPF, "Wlan0WantGateway        %s", mWlan0WantGateway.c_str());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Interfaces::showEth0(int aPF)
{
   Prn::print(aPF, "");
   Prn::print(aPF, "Eth0ValidFlag           %s", my_string_from_bool(mEth0ValidFlag));
   Prn::print(aPF, "Eth0DhcpFlag            %s", my_string_from_bool(mEth0DhcpFlag));
   Prn::print(aPF, "Eth0Address             %s", mEth0Address.c_str());
   Prn::print(aPF, "Eth0Mask                %s", mEth0Mask.c_str());
   Prn::print(aPF, "Eth0Gateway             %s", mEth0Gateway.c_str());
   Prn::print(aPF, "Eth0WantAddress         %s", mEth0WantAddress.c_str());
   Prn::print(aPF, "Eth0WantMask            %s", mEth0WantMask.c_str());
   Prn::print(aPF, "Eth0WantGateway         %s", mEth0WantGateway.c_str());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
