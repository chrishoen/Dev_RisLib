/*==============================================================================

==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "my_functions.h"
#include "risCmdLineFile.h"
#include "risPortableCalls.h"

#include "DevIO_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

DevIO_HWDefs_Settings::DevIO_HWDefs_Settings()
{
  mSlotDef  [0] = 0;
  mCPUDef   [0] = 0;
}

//******************************************************************************

bool DevIO_HWDefs_Settings::isValid_SlotDef() {return mSlotDef[0] != 0;}
bool DevIO_HWDefs_Settings::isValid_CPUDef()  {return mCPUDef[0]  != 0;}

//******************************************************************************

void DevIO_HWDefs_Settings::show()
{
    printf("DevIO  SlotDef      %16s\n", mSlotDef);
    printf("DevIO  CPUDef       %16s\n", mCPUDef);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class overload, called for each line in the file
void DevIO_HWDefs_Settings::execute(Ris::CmdLineCmd* aCmd)
{
  if (aCmd->isCmd("END"))         nestedPop(aCmd);

  // Read variables
  if (aCmd->isCmd("SlotDef"))     aCmd->copyArgString(1, mSlotDef,  MaxHWDefsSize);
  if (aCmd->isCmd("CPUDef"))      aCmd->copyArgString(1, mCPUDef,   MaxHWDefsSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

DevIO_ProgramName_Settings::DevIO_ProgramName_Settings()
{
   mSlot1A   [0]=0;
   mSlot1B   [0]=0;
   mSlot2A   [0]=0;
   mSlot2B   [0]=0;
   mSlot3A   [0]=0;
   mSlot3B   [0]=0;
   mSlot4A   [0]=0;
   mSlot4B   [0]=0;
}

//******************************************************************************

bool DevIO_ProgramName_Settings::isValid_Slot1A() {return mSlot1A[0] != 0;}
bool DevIO_ProgramName_Settings::isValid_Slot1B() {return mSlot1B[0] != 0;}
bool DevIO_ProgramName_Settings::isValid_Slot2A() {return mSlot2A[0] != 0;}
bool DevIO_ProgramName_Settings::isValid_Slot2B() {return mSlot2B[0] != 0;}
bool DevIO_ProgramName_Settings::isValid_Slot3A() {return mSlot3A[0] != 0;}
bool DevIO_ProgramName_Settings::isValid_Slot3B() {return mSlot3B[0] != 0;}
bool DevIO_ProgramName_Settings::isValid_Slot4A() {return mSlot4A[0] != 0;}
bool DevIO_ProgramName_Settings::isValid_Slot4B() {return mSlot4B[0] != 0;}

//******************************************************************************

void DevIO_ProgramName_Settings::show()
{
  printf("ProgramName Slot1A  %32s\n", mSlot1A);
  printf("ProgramName Slot1B  %32s\n", mSlot1B);
  printf("ProgramName Slot2A  %32s\n", mSlot2A);
  printf("ProgramName Slot2B  %32s\n", mSlot2B);
  printf("ProgramName Slot3A  %32s\n", mSlot3A);
  printf("ProgramName Slot3B  %32s\n", mSlot3B);
  printf("ProgramName Slot4A  %32s\n", mSlot4A);
  printf("ProgramName Slot4B  %32s\n", mSlot4B);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class overload, called for each line in the file

void DevIO_ProgramName_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))  nestedPop(aCmd);

   // Read variables
   if (aCmd->isCmd("Slot1A"))    aCmd->copyArgString(1, mSlot1A,  MaxProgramNameSize);
   if (aCmd->isCmd("Slot1B"))    aCmd->copyArgString(1, mSlot1B,  MaxProgramNameSize);
   if (aCmd->isCmd("Slot2A"))    aCmd->copyArgString(1, mSlot2A,  MaxProgramNameSize);
   if (aCmd->isCmd("Slot2B"))    aCmd->copyArgString(1, mSlot2B,  MaxProgramNameSize);
   if (aCmd->isCmd("Slot3A"))    aCmd->copyArgString(1, mSlot3A,  MaxProgramNameSize);
   if (aCmd->isCmd("Slot3B"))    aCmd->copyArgString(1, mSlot3B,  MaxProgramNameSize);
   if (aCmd->isCmd("Slot4A"))    aCmd->copyArgString(1, mSlot4A,  MaxProgramNameSize);
   if (aCmd->isCmd("Slot4B"))    aCmd->copyArgString(1, mSlot4B,  MaxProgramNameSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************


DevIO_Settings::DevIO_Settings()
{
  mCode1 = 0;
  mCode2 = 0;
}

//******************************************************************************


void DevIO_Settings::show()
{
  printf("\n");
  printf("DevIO_Settings ************************************************\n");

  printf("Code1                      %d\n", mCode1);
  printf("Code2                      %d\n", mCode2);
  printf("\n");
  mHWDefs.show();
  printf("\n");
  mProgramName.show();
}

//******************************************************************************
// Base class overload, called for each line in the file

void DevIO_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))        nestedPop(aCmd);

   // Read variables
   if (aCmd->isCmd("Code1"))      mCode1 = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))      mCode2 = aCmd->argInt(1);
   if (aCmd->isCmd("Begin"))      executeOnBegin(aCmd);
}

//******************************************************************************
// Execute specific command

void DevIO_Settings::executeOnBegin(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))  nestedPop(aCmd);

   // Read variables
   if (aCmd->isArgString(1,"HWDefs"))
   {
     nestedPush(aCmd, &mHWDefs);
   }
   if (aCmd->isArgString(1,"ProgramName"))
   {
     nestedPush(aCmd, &mProgramName);
   }
}

