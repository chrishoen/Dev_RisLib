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

#include "Mount_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

Mount_Settings::Mount_Settings()
{
   mDevice  [0]=0;
   mDir     [0]=0;
   mOptions [0]=0;

   strcpy(mName, "NONAME");
}

//******************************************************************************

void Mount_Settings::setName(char* aName)
{
   my_strncpy(mName, aName, MaxStringSize);
}

//******************************************************************************

void Mount_Settings::show()
{
   if (isValid())
   {
      printf("%-10s Device     %s\n", mName, mDevice);
      printf("%-10s Dir        %s\n", mName, mDir);
      printf("%-10s Options    %s\n", mName, mOptions);
      printf("\n");
   }
   else
   {
      printf("%-10s INVALID\n", mName);
      printf("\n");
   }

}

//******************************************************************************

bool Mount_Settings::isValid()
{
   bool tValidFlag = true;

   if (mDevice  [0]==0) tValidFlag=false;
   if (mDir     [0]==0) tValidFlag=false;

   return tValidFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class overload, called for each line in the file

void Mount_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))        nestedPop(aCmd);

   if (aCmd->isCmd("Device"))     aCmd->copyArgString(1,mDevice,  MaxStringSize);
   if (aCmd->isCmd("Dir"))        aCmd->copyArgString(1,mDir,     MaxStringSize);
   if (aCmd->isCmd("Options"))    aCmd->copyArgString(1,mOptions, MaxStringSize);
}

