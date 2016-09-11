#ifndef _GGI_GSETTINGS_H_
#define _GGI_GSETTINGS_H_

/*==============================================================================

Global settings file, CSC specific section.

==============================================================================*/
#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Settings for the GGI CSC

class GGI_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding CSC section of
   // the settings file

   // Integrity checks
   int   mCode1;
   int   mCode2;

   // GGI variables
   bool  mGGIStackPresent;
   bool  mEnableGrooming;

   int   mNumberOfGGIs;

   enum {MaxNumberOfGGIs=3};
   bool  mGGIPresent[MaxNumberOfGGIs];

;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   GGI_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute (Ris::CmdLineCmd* aCmd);

   // Execute specific commands
   void executeOnNumberOfGGIs  (Ris::CmdLineCmd* aCmd);

};

//******************************************************************************
#endif

