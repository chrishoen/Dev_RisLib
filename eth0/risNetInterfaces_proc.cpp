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
   // Get the network settings for eth0 and wlan0. These subfunctions
   // set the member variables associated with the two interfaces.
   doGetNetsettingsEth0Address();
   doGetNetsettingsWlan0Address();
   doGetNetsettingsEth0Gateway();
   doGetNetsettingsWlan0Gateway();

   // Defaults.
   mShowValidFlag = false;
   mShowDhcpFlag = false;
   mShowAddress = "none";
   mShowMask = "none";
   mShowGateway = "none";

   // Set the show member variables according to the  member variables 
   // for the two interfaces. eth0 takes precedence over wlan0.
   if (mEth0ValidFlag)
   {
      mShowValidFlag = mEth0ValidFlag;
      mShowDhcpFlag = mEth0DhcpFlag;
      mShowAddress = mEth0Address;
      mShowMask = mEth0Mask;
      mShowGateway = mEth0Gateway;
   }
   else if (mWlan0ValidFlag)
   {
      mShowValidFlag = mWlan0ValidFlag;
      mShowDhcpFlag = mWlan0DhcpFlag;
      mShowAddress = mWlan0Address;
      mShowMask = mWlan0Mask;
      mShowGateway = mWlan0Gateway;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get the current network settings. Sub function.

void Interfaces::doGetNetsettingsEth0Address()
{
   // Do this first.
   mEth0Address = "none";
   mEth0Mask = "none";

   // Execute system command into a response string list.
   // An example response is:
   // "inet 192.168.1.35  netmask 255.255.255.0  broadcast 192.168.1.255"
   std::vector<std::string> tResponse;
   Ris::doSystemCommand("ifconfig eth0", tResponse);

   // Guard.
   if (tResponse.size() == 0) return;

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

void Interfaces::doGetNetsettingsWlan0Address()
{
   // Do this first.
   mWlan0Address = "none";
   mWlan0Mask = "none";

   // Execute system command into a response string list.
   // An example response is:
   // "inet 192.168.1.35  netmask 255.255.255.0  broadcast 192.168.1.255"
   std::vector<std::string> tResponse;
   Ris::doSystemCommand("ifconfig wlan0", tResponse);

   // Guard.
   if (tResponse.size() == 0) return;

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

void Interfaces::doGetNetsettingsEth0Gateway()
{
   // Do this first.
   mEth0Gateway = "none";
   mEth0DhcpFlag = false;
   mEth0ValidFlag = false;

   // Execute system command into a response string list.
   // An example response is:
   // "inet 192.168.1.35  netmask 255.255.255.0  broadcast 192.168.1.255"
   // "default via 192.168.1.1 dev eth0 proto dhcp src 192.168.1.35 metric 202"
   std::vector<std::string> tResponse;
   Ris::doSystemCommand("ip route show dev eth0", tResponse);

   // Guard.
   if (tResponse.size() == 0) return;

   // The interface is valid.
   mEth0ValidFlag = true;

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
      if (tParse[i] == "via") mEth0Gateway = tParse[i + 1];
      if (tParse[i] == "dhcp") mEth0DhcpFlag = true;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get the current network settings. Sub function.

void Interfaces::doGetNetsettingsWlan0Gateway()
{
   // Do this first.
   mWlan0Gateway = "none";
   mWlan0DhcpFlag = false;

   // Execute system command into a response string list.
   // An example response is:
   // "inet 192.168.1.35  netmask 255.255.255.0  broadcast 192.168.1.255"
   // "default via 192.168.1.1 dev eth0 proto dhcp src 192.168.1.35 metric 202"
   std::vector<std::string> tResponse;
   Ris::doSystemCommand("ip route show dev wlan0", tResponse);

   // Guard.
   if (tResponse.size() == 0) return;

   // The interface is valid.
   mWlan0ValidFlag = true;

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
      if (tParse[i] == "via") mWlan0Gateway = tParse[i + 1];
      if (tParse[i] == "dhcp") mWlan0DhcpFlag = true;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
