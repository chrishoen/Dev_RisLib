#ifndef _FPGA_SETTINGS_H_
#define _FPGA_SETTINGS_H_

/*==============================================================================

Global settings file, CSC specific section.

==============================================================================*/
#include "risCmdLineExec.h"

//******************************************************************************
// Settings for the Network CSC

class Fpga_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding section of
   // the settings file

   // Integrity checks
   int   mCode1;
   int   mCode2;

   // Fpga settings
   bool mUseLowerCoverData;
   bool mUseSIOGmpa;

   // Debug test settings
   int  mTestCode1;
   int  mTestCode2;
   int  mTestCode3;
   int  mTestCode4;
   int  mTestCode5;
   int  mTestCode6;
   int  mTestCode7;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   Fpga_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute (Ris::CmdLineCmd* aCmd);
};

#endif

