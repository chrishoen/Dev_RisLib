#pragma once

/*==============================================================================
Debian linux eth0 support class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <string>

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class provides support for debian linux eth0 and wlan0. It provides
// getting info on eth0 and wlan0 (address, mask, gateway, etc) and it provides
// the ability to set configuration for them. It obtains network settings and
// stores them in a json file. It provides the abilty to read the settings
// from the json file and set the network settings accordingly.

class Interfaces
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Json file path.
   std::string mFilePath;

   // Network interface variables for eth0.
   bool mEth0ValidFlag;
   bool mEth0DhcpFlag;
   std::string mEth0Address;
   std::string mEth0Mask;
   std::string mEth0Gateway;

   // Wanted static variables for eth0.
   std::string mEth0WantAddress;
   std::string mEth0WantMask;
   std::string mEth0WantGateway;

   // Network interface variables for wlan0.
   bool mWlan0ValidFlag;
   bool mWlan0DhcpFlag;
   std::string mWlan0Address;
   std::string mWlan0Mask;
   std::string mWlan0Gateway;

   // Wanted static variables for wlan0.
   std::string mWlan0WantAddress;
   std::string mWlan0WantMask;
   std::string mWlan0WantGateway;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods:

   // Constructor.
   Interfaces();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Read from the json file.
   void doRead();

   // Write to the json file.
   void doWrite();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Get the current network settings.
   void doGetNetSettings();

   // Get the current network settings. Sub functions.
   void doGetNetSettingsEth0Address();
   void doGetNetSettingsWlan0Address();
   void doGetNetSettingsEth0Gateway();
   void doGetNetSettingsWlan0Gateway();
   void doGetNetSettingsEth0DhcpFlag();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Set the network settings for eth0 from the member variables.
   void doSetNetSettingsEth0();

   // Set the network settings for eth0 from the member variables.
   // Sub functions.
   void doSetNetSettingsEth0_dhcp();
   void doSetNetSettingsEth0_static();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Set a member variable. Return true if successful.
   bool setEth0WantAddress(std::string aString);
   bool setEth0WantMask(std::string aString);
   bool setEth0WantGateway(std::string aString);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Support.
   void show(int aPrintFilter = 0);
   void showEth0(int aPrintFilter = 0);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

