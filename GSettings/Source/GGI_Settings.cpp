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

#include "GGI_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

GGI_Settings::GGI_Settings()
{
   mCode1           = 0;
   mCode2           = 0;
   mGGIStackPresent = true;
   mEnableGrooming  = false;
   mNumberOfGGIs    = 0;

   for (int i = 0; i < MaxNumberOfGGIs; i++)
   {
      mGGIPresent[i] = false;
   }
}

void GGI_Settings::show()
{
   printf("\n");
   printf("GGI_Settings****************************************************************\n");
   printf("\n");

   printf("Code1               %5d\n", mCode1);
   printf("Code2               %5d\n", mCode2);
   printf("\n");
   printf("GGIStackPresent                       %8s\n",     my_string_from_bool(mGGIStackPresent));
   printf("EnableGrooming                        %8s\n",     my_string_from_bool(mEnableGrooming));
   printf("mNumberOfGGIs                         %8d\n",     mNumberOfGGIs);


}

//******************************************************************************
// Base class overload, called for each line in the file

void GGI_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))        nestedPop(aCmd);

   // Read variables
   if (aCmd->isCmd("Code1"))                 mCode1           = aCmd->argInt  (1);
   if (aCmd->isCmd("Code2"))                 mCode2           = aCmd->argInt  (1);

   if (aCmd->isCmd("GGIStackPresent"))       mGGIStackPresent = aCmd->argBool (1);
   if (aCmd->isCmd("EnableGrooming"))        mEnableGrooming  = aCmd->argBool (1);
   if (aCmd->isCmd("NumberOfGGIs"))          executeOnNumberOfGGIs(aCmd);
}

//******************************************************************************
// Execute specific command

void GGI_Settings::executeOnNumberOfGGIs(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("NumberOfGGIs")) mNumberOfGGIs = aCmd->argInt(1);

   for (int i = 0; i < MaxNumberOfGGIs; i++)
   {
      mGGIPresent[i] = (i < mNumberOfGGIs);
   }

}