#ifndef _DIAG_GSETTINGS_H_
#define _DIAG_GSETTINGS_H_

/*==============================================================================

Global settings file, CSC specific section.

==============================================================================*/
#include "Mount_Settings.h"
#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Settings for the DIAG CSC

class DIAG_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding CSC section of
   // the settings file

   // Integrity checks
   int   mCode1;
   int   mCode2;

   // Flags
   bool mEnable_PLT;
   bool mEnable_GGI;

   // Network share mounts

   Mount_Settings   mMount_diagnostic;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   DIAG_Settings();
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

