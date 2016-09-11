#ifndef _MOUNT_SETTINGS_H_
#define _MOUNT_SETTINGS_H_

/*==============================================================================

Global settings file, CSC specific section.

==============================================================================*/
#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Network share mount settings

class Mount_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members.

   enum {MaxStringSize    = 200};

   char mDevice  [MaxStringSize];
   char mDir     [MaxStringSize];
   char mOptions [MaxStringSize];

   //---------------------------------------------------------------------------
   // Name

   void setName(char* aName);
   char mName    [MaxStringSize];

   //---------------------------------------------------------------------------
   // isValid

   bool isValid();

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   Mount_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute(Ris::CmdLineCmd* aCmd);

};

#endif

