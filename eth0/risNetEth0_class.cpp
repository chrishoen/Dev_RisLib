//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "json.h"
#include "risFileFunctions.h"

#include "risNetEth0.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

Eth0::Eth0()
{
   if (Ris::portableIsWindows())
   {
      mFilePath = "c:/aaa_cproc/files/eth0.json";
   }
   else
   {
      mFilePath = "/opt/prime/files/eth0.json";
   }

   mDhcpFlag = false;
   mAddress = "192.1681.179";
   mMask = "255.255.255.0";
   mGateway = "192.168.1.1";
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to the json file.

void Eth0::doWrite()
{
   // Json variables.
   Json::Value tJsonValue;

   // Copy members.
   tJsonValue["DhcpFlag"] = mDhcpFlag;
   tJsonValue["Address"] = mAddress;
   tJsonValue["Mask"] = mMask;
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

void Eth0::doRead()
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
   mDhcpFlag = tJsonValue["DhcpFlag"].asBool();
   mAddress = tJsonValue["Address"].asString();
   mMask = tJsonValue["Mask"].asString();
   mGateway = tJsonValue["Gateway"].asString();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void Eth0::show(int aPF)
{
   Prn::print(aPF, "");
   Prn::print(aPF, "FilePath %s", mFilePath.c_str());

   Prn::print(aPF, "DhcpFlag                %s", my_string_from_bool(mDhcpFlag));
   Prn::print(aPF, "Address                 %s", mAddress.c_str());
   Prn::print(aPF, "Mask                    %s", mMask.c_str());
   Prn::print(aPF, "Gateway                 %s", mGateway.c_str());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
