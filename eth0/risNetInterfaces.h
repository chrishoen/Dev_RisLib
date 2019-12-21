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

   // Network interface variables for wlan0.
   bool mWlan0ValidFlag;
   bool mWlan0DhcpFlag;
   std::string mWlan0Address;
   std::string mWlan0Mask;
   std::string mWlan0Gateway;

   // Network interface variables to show.
   // Decide between eth0 and wlan0.

   bool mShowValidFlag;
   bool mShowDhcpFlag;
   std::string mShowAddress;
   std::string mShowMask;
   std::string mShowGateway;

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
   void doGetNetsettings();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Get the current network settings. Sub functions.
   void doGetNetsettingsEth0Address();
   void doGetNetsettingsWlan0Address();
   void doGetNetsettingsEth0Gateway();
   void doGetNetsettingsWlan0Gateway();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Support.
   void show(int aPrintFilter = 0);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

