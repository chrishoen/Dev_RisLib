//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "json.h"
#include "risFileFunctions.h"
#include "risSystemCalls.h"

#include "risNetInterfaces.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get the current network settings.

void Interfaces::doGetNetsettings()
{
   doGetNetsettingsEth0();
   doGetNetsettingsWlan0();
   doGetNetsettingsGateway();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get the current network settings. Sub function.

void Interfaces::doGetNetsettingsEth0()
{
   // Do this first.
   mEth0Address = "none";
   mEth0Mask = "none";

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
      if (tParse[i] == "inet") mEth0Address = tParse[i + 1];
      if (tParse[i] == "netmask") mEth0Mask = tParse[i + 1];
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get the current network settings. Sub function.

void Interfaces::doGetNetsettingsWlan0()
{
   // Do this first.
   mWlan0Address = "none";
   mWlan0Mask = "none";

   // Execute system command into a response string list.
   // An example response is:
   // "inet 192.168.1.35  netmask 255.255.255.0  broadcast 192.168.1.255"
   std::vector<std::string> tResponse;
   Ris::doSystemCommand("ifconfig wlan0", tResponse);

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
      if (tParse[i] == "inet") mWlan0Address = tParse[i + 1];
      if (tParse[i] == "netmask") mWlan0Mask = tParse[i + 1];
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get the current network settings. Sub function.

void Interfaces::doGetNetsettingsGateway()
{
   // Do this first.
   mGateway = "none";
   mEth0DhcpFlag = false;

   // Execute system command into a response string list.
   // An example response is:
   // "inet 192.168.1.35  netmask 255.255.255.0  broadcast 192.168.1.255"
   // "default via 192.168.1.1 dev eth0 proto dhcp src 192.168.1.35 metric 202"
   std::vector<std::string> tResponse;
   Ris::doSystemCommand("ip route show", tResponse);

   // Stream string variables for the response list entry 1.
   std::stringstream tStream(tResponse[0]);
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
      if (tParse[i] == "via") mGateway = tParse[i + 1];
      if (tParse[i] == "dhcp") mEth0DhcpFlag = true;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
