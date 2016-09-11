
#ifndef DEVIO_SETTINGS_H_
#define DEVIO_SETTINGS_H_

/*==============================================================================

Global settings file, CSC specific section.

==============================================================================*/
#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Settings for the DevIO HWDefs

class DevIO_HWDefs_Settings : public Ris::BaseCmdLineExec
{
public:
  //---------------------------------------------------------------------------
  // Settings members. These are read from the corresponding section of
  // the settings file

  enum { MaxHWDefsSize=30 };

  char mSlotDef    [MaxHWDefsSize];
  char mCPUDef     [MaxHWDefsSize];

  bool isValid_SlotDef();
  bool isValid_CPUDef();

  //---------------------------------------------------------------------------
  // Infrastucture. Constructor and such

  typedef Ris::BaseCmdLineExec BaseClass;
  DevIO_HWDefs_Settings();
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
// Settings for the DevIO ProgramName

class DevIO_ProgramName_Settings : public Ris::BaseCmdLineExec
{
public:
  //---------------------------------------------------------------------------
  // Settings members. These are read from the corresponding section of
  // the settings file
  enum {MaxProgramNameSize=60};

  char mSlot1A  [MaxProgramNameSize];
  char mSlot1B  [MaxProgramNameSize];
  char mSlot2A  [MaxProgramNameSize];
  char mSlot2B  [MaxProgramNameSize];
  char mSlot3A  [MaxProgramNameSize];
  char mSlot3B  [MaxProgramNameSize];
  char mSlot4A  [MaxProgramNameSize];
  char mSlot4B  [MaxProgramNameSize];

  bool isValid_Slot1A();
  bool isValid_Slot1B();
  bool isValid_Slot2A();
  bool isValid_Slot2B();
  bool isValid_Slot3A();
  bool isValid_Slot3B();
  bool isValid_Slot4A();
  bool isValid_Slot4B();

  //---------------------------------------------------------------------------
  // Infrastucture. Constructor and such

  typedef Ris::BaseCmdLineExec BaseClass;
  DevIO_ProgramName_Settings();
  void show();

  //---------------------------------------------------------------------------
  // Execute. This is called for each line in the corresponding CSC section of
  // the settings file. It parses the command line to set the settings
  // variables defined above.

  void execute(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
// Settings for the Network CSC

class DevIO_Settings : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Settings members. These are read from the corresponding section of
   // the settings file

   // Integrity checks
   int   mCode1;
   int   mCode2;

   // DevIO settings
   DevIO_HWDefs_Settings        mHWDefs;
   DevIO_ProgramName_Settings   mProgramName;

   //---------------------------------------------------------------------------
   // Infrastucture. Constructor and such

   typedef Ris::BaseCmdLineExec BaseClass;
   DevIO_Settings();
   void show();

   //---------------------------------------------------------------------------
   // Execute. This is called for each line in the corresponding CSC section of
   // the settings file. It parses the command line to set the settings
   // variables defined above.

   void execute (Ris::CmdLineCmd* aCmd);

   //
   void executeOnBegin(Ris::CmdLineCmd* aCmd);

};

#endif /* DEVIO_SETTINGS_H_ */
