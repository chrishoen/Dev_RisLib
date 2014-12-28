#ifndef _PROCOSETTINGS_H_
#define _PROCOSETTINGS_H_
/*==============================================================================

This file contains settings for protocomm. 

==============================================================================*/

//**********************************************************************
//**********************************************************************
//**********************************************************************

#include "risCmdLineFile.h"

namespace ProtoComm
{

//**********************************************************************
//**********************************************************************
//**********************************************************************
// ProtoCommSettings class, inherits from BaseCmdLineExec to process
// command lines from a command line file.
// Each application reads its own print settings from a common settings
// file. 

class  ProtoCommSettings : public Ris::BaseCmdLineExec
{
public:

   //---------------------------------------------------------------------------
   // Settings members.

   enum {MaxStringSize=30};

   int  mMyAppNumber;
   int  mMyAppRole;

   char mTcpServerIPAddress[MaxStringSize];
   int  mTcpServerPort;

   char mMyUdpIPAddress[MaxStringSize];
   int  mMyUdpPort;

   char mOtherUdpIPAddress[MaxStringSize];
   int  mOtherUdpPort;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   ProtoCommSettings();
   void show();

   // Baseclass override, executes for each line in the settings
   // command line file

   void execute(Ris::CmdLineCmd* aCmd);

   // Specific execute
   void executeOnTcpServer (Ris::CmdLineCmd* aCmd);
   void executeOnMyUdp     (Ris::CmdLineCmd* aCmd);
   void executeOnOtherUdp  (Ris::CmdLineCmd* aCmd);

   // Initialize.
   // aSettingsFileName is the settings filename. aSection is the settings
   // file section that each application extracts its settings from. 
   // aSectionMode specifies the section mode. Each section begins with a 
   // command line, such as "Begin CmdFile Default", where "Begin" is the 
   // command ,"CmdFile" is  first argument and denotes the section and
   // "Default" is the second argument and denotes the section mode.

   bool initialize(char* aSection);

   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   // Section

   char mSection[200];
   char mSectionMode[200];

   bool isMySection(Ris::CmdLineCmd* aCmd);
   bool mSectionFlag;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _PROCOSETTINGS_CPP_
        ProtoCommSettings gSettings;
#else
        extern ProtoCommSettings gSettings;
#endif

}//namespace
#endif

