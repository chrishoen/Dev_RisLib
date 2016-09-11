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

#include "Fpga_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

Fpga_Settings::Fpga_Settings()
{
   mCode1 = 0;
   mCode2 = 0;
   mTestCode1 = 0;
   mTestCode2 = 0;
   mTestCode3 = 0;
   mTestCode4 = 0;
   mTestCode5 = 0;
   mTestCode6 = 0;
   mTestCode7 = 0;
   mUseLowerCoverData = false;
   mUseSIOGmpa = true;
}

void Fpga_Settings::show()
{
   printf("\n");
   printf("Fpga_Settings************************************************************\n");
   printf("\n");

   printf("Code1               %d\n", mCode1);
   printf("Code2               %d\n", mCode2);
   printf("\n");
   printf("UseLowerCoverData                     %8s\n",     my_string_from_bool(mUseLowerCoverData));
   printf("UseSIOGmpa                            %8s\n",     my_string_from_bool(mUseSIOGmpa));
   printf("\n");
   printf("TestCode1                             %8d\n",     mTestCode1);
   printf("TestCode2                             %8d\n",     mTestCode2);
   printf("\n");
}

//******************************************************************************
// Base class overload, called for each line in the file

void Fpga_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))        nestedPop(aCmd);

   // Read variables
   if (aCmd->isCmd("Code1"))             mCode1 = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))             mCode2 = aCmd->argInt(1);

   if (aCmd->isCmd("UseLowerCoverData")) mUseLowerCoverData = aCmd->argBool(1);
   if (aCmd->isCmd("UseSIOGmpa"))        mUseSIOGmpa = aCmd->argBool(1);

   if (aCmd->isCmd("TestCode1"))         mTestCode1 = aCmd->argInt(1);
   if (aCmd->isCmd("TestCode2"))         mTestCode2 = aCmd->argInt(1);
   if (aCmd->isCmd("TestCode3"))         mTestCode3 = aCmd->argInt(1);
   if (aCmd->isCmd("TestCode4"))         mTestCode4 = aCmd->argInt(1);
   if (aCmd->isCmd("TestCode5"))         mTestCode5 = aCmd->argInt(1);
   if (aCmd->isCmd("TestCode6"))         mTestCode6 = aCmd->argInt(1);
   if (aCmd->isCmd("TestCode7"))         mTestCode7 = aCmd->argInt(1);
}

