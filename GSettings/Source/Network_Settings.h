#ifndef _NETWORK_SETTINGS_H_
#define _NETWORK_SETTINGS_H_

/*==============================================================================

Global settings file, CSC specific section.

==============================================================================*/
#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Settings for the Network, IPAddresses

class Network_IPAddress_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding section of
   // the settings file

   enum {MaxIPAddressSize=20};

   char mCOMM   [MaxIPAddressSize];
   char mPLT    [MaxIPAddressSize];
   char mDIAG   [MaxIPAddressSize];
   char mSCP    [MaxIPAddressSize];
   char mRTC    [MaxIPAddressSize];
   char mGGI    [MaxIPAddressSize];

   char mGUI    [MaxIPAddressSize];
   char mSCC1   [MaxIPAddressSize];
   char mSCC2   [MaxIPAddressSize]; 
   char mPMC    [MaxIPAddressSize];

   bool isValid_COMM();
   bool isValid_PLT();
   bool isValid_DIAG();
   bool isValid_SCP();
   bool isValid_RTC();
   bool isValid_GGI();

   bool isValid_GUI();
   bool isValid_SCC1();
   bool isValid_SCC2();
   bool isValid_PMC();

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   Network_IPAddress_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute(Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Settings for the Network, NetParameters

class Network_NetParameters_Settings : public Ris::BaseCmdLineExec
{
public:
  //---------------------------------------------------------------------------
  // Settings members. These are read from the corresponding section of
  // the settings file

  enum {MaxIPAddressSize=20};

  char mNetmask    [MaxIPAddressSize];
  char mGateway    [MaxIPAddressSize];

  bool isValid_Netmask();
  bool isValid_Gateway();

  //---------------------------------------------------------------------------
  // Infrastucture. Constructor and such

  typedef Ris::BaseCmdLineExec BaseClass;
  Network_NetParameters_Settings();
  void show();

  //---------------------------------------------------------------------------
  // Execute. This is called for each line in the corresponding CSC section of
  // the settings file. It parses the command line to set the settings
  // variables defined above.

  void execute(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Settings for the Network, Ports

class Network_Port_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding section of
   // the settings file

   int  mCOMM_TO_SCP;
   int  mPLT_TO_SCP;
   int  mDIAG_TO_SCP;
   int  mRTC_TO_SCP;
   int  mGGI_TO_SCP;

   int  mSCP_TO_COMM;
   int  mSCP_TO_PLT;
   int  mSCP_TO_DIAG;
   int  mSCP_TO_RTC;
   int  mSCP_TO_GGI;

   int  mSCP_TO_GUI;
   int  mGUI_TO_SCP;

   int  mSCP_TO_SCC1;
   int  mSCC1_TO_SCP;

   int  mSCP_TO_SCC2;
   int  mSCC2_TO_SCP;

   int  mSCP_TO_PMC;
   int  mPMC_TO_SCP;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   Network_Port_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute(Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
// Settings for the Network CSC

class Network_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding section of
   // the settings file

   // Integrity checks
   int   mCode1;
   int   mCode2;

   // Network settings
   Network_IPAddress_Settings         mIPAddress;
   Network_NetParameters_Settings     mNetParameters;
   Network_Port_Settings              mPort;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   Network_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute (Ris::CmdLineCmd* aCmd);

   // 
   void executeOnBegin(Ris::CmdLineCmd* aCmd);
};

#endif

