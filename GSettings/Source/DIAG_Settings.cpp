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

#include "DIAG_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

DIAG_Settings::DIAG_Settings()
{
   mCode1           = 0;
   mCode2           = 0;

   mEnable_PLT      = true;
   mEnable_GGI      = true;

   mMount_diagnostic.setName("Mount_diagnostic");
}

void DIAG_Settings::show()
{
   printf("\n");
   printf("DIAG_Settings****************************************************************\n");
   printf("\n");

   printf("Code1               %d\n", mCode1);
   printf("Code2               %d\n", mCode2);
   printf("\n");

   printf("Enable_PLT         %16s\n", my_string_from_bool(mEnable_PLT));
   printf("Enable_GGI         %16s\n", my_string_from_bool(mEnable_GGI));
   printf("\n");

   mMount_diagnostic.show();
}

//******************************************************************************
// Base class overload, called for each line in the file

void DIAG_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))              nestedPop(aCmd);

   if (aCmd->isCmd("Begin"))            executeOnBegin(aCmd);

   // Read variables
   if (aCmd->isCmd("Code1"))            mCode1      = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))            mCode2      = aCmd->argInt(1);

   if(aCmd->isCmd("Enable_PLT"     ))   mEnable_PLT = aCmd->argBool(1);
   if(aCmd->isCmd("Enable_GGI"     ))   mEnable_GGI = aCmd->argBool(1);
}

//******************************************************************************
// Execute specific command

void DIAG_Settings::executeOnBegin(Ris::CmdLineCmd* aCmd)
{
   //------------------------------------------------------
   if      (aCmd->isArgString(1, "Mount_diagnostic"))
   {
      nestedPush(aCmd,&mMount_diagnostic);
   }
}

