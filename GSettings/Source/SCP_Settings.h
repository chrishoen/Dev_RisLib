#ifndef _SCP_GSETTINGS_H_
#define _SCP_GSETTINGS_H_

/*==============================================================================

Global settings file, CSC specific section.

==============================================================================*/
#include "Mount_Settings.h"
#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Settings for the SCP CSC

class SCP_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding CSC section of
   // the settings file

   // Integrity checks
   int   mCode1;
   int   mCode2;

   // Network share mounts

   Mount_Settings   mMount_disk1;
   Mount_Settings   mMount_disk2;
   Mount_Settings   mMount_disk3;
   Mount_Settings   mMount_disk4;

   // Flags
   bool mEnable_StripChart1;
   bool mEnable_StripChart2;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   SCP_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute (Ris::CmdLineCmd* aCmd);

   // Execute specific commands
   void executeOnBegin           (Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
#endif

