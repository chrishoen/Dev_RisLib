#ifndef _TRANSFORMS_SETTINGS_H_
#define _TRANSFORMS_SETTINGS_H_

/*==============================================================================

Global settings file, PLT specific section.

==============================================================================*/
#include "risCmdLineTables.h"
#include "risCmdLineExec.h"

//******************************************************************************
// Settings for transformation matrices

class Transforms_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding CSC section of
   // the settings file

   // Integrity checks
   int   mCode1;
   int   mCode2;

   // Default selector
   int   mDefaultSelect;

   // Transformation matrices
   Ris::CmdLineDoubleTable2D  mMatrix_PG;
   Ris::CmdLineDoubleTable2D  mMatrix_GP;
   Ris::CmdLineDoubleTable2D  mMatrix_PA;
   Ris::CmdLineDoubleTable2D  mMatrix_AP;
   Ris::CmdLineDoubleTable2D  mMatrix_PGGI_1;
   Ris::CmdLineDoubleTable2D  mMatrix_PGGI_2;
   Ris::CmdLineDoubleTable2D  mMatrix_PGGI_3;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   Transforms_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute (Ris::CmdLineCmd* aCmd);
};

#endif

#ifndef _TRANSFORMS_SETTINGS_H_
#define _TRANSFORMS_SETTINGS_H_

/*==============================================================================

Global settings file, PLT specific section.

==============================================================================*/
#include "risRTable.h"
#include "risCmdLineExec.h"

//******************************************************************************
// Settings for transformation matrices

class Transforms_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding CSC section of
   // the settings file

   // Integrity checks
   int   mCode1;
   int   mCode2;

   // Transformation matrices
   Ris::RTable2D  mMatrix_PG;
   Ris::RTable2D  mMatrix_GP;
   Ris::RTable2D  mMatrix_PA;
   Ris::RTable2D  mMatrix_AP;
   Ris::RTable2D  mMatrix_PGGI_1;
   Ris::RTable2D  mMatrix_PGGI_2;
   Ris::RTable2D  mMatrix_PGGI_3;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   Transforms_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute (Ris::CmdLineCmd* aCmd);
};

#endif

