#ifndef _SYSTEM_SETTINGS_H_
#define _SYSTEM_SETTINGS_H_

/*==============================================================================

Global settings file, CSC specific section.

==============================================================================*/
#include "risCmdLineExec.h"

//******************************************************************************
// Settings for the Network CSC

class System_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding section of
   // the settings file

   // Integrity checks
   int   mCode1;
   int   mCode2;

   // Product settings
   bool mIsKepler;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   System_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute (Ris::CmdLineCmd* aCmd);
};

#endif

