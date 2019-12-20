//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "json.h"
#include "risFileFunctions.h"
#include "risSystemCalls.h"

#include "risNetEth0.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get the current network settings.

void Eth0::doGetNetsettings()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Get address and mask from ifconfig.

   // Execute system command into a response string list.
   // An example response is:
   // "inet 192.168.1.35  netmask 255.255.255.0  broadcast 192.168.1.255"
   std::vector<std::string> tResponse;
   Ris::doSystemCommand("ifconfig eth0", tResponse);

   // Stream string variables for the response list entry 1.
   std::stringstream tStream(tResponse[1]);
   std::string tToken;
   // Variables for parsing the response  list entry 1.
   std::vector<std::string> tParse;
   char tDelimiter = ' ';

   // Parse the response string entry 1 into the a list of strings.
   while (std::getline(tStream, tToken, tDelimiter))
   {
      if (tToken.length())
      {
         tParse.push_back(tToken);
      }
   }

   // Parse the list of strings into member variables.
   for (int i = 0; i < tParse.size(); i++)
   {
      Prn::print(0, "tParse $ %s", tParse[i].c_str());
      if (tParse[i] == "inet") mAddress = tParse[i + 1];
      if (tParse[i] == "netmask") mMask = tParse[i + 1];
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
