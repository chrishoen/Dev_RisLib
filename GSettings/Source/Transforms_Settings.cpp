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

#include "Transforms_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

Transforms_Settings::Transforms_Settings()
{
   mCode1           = 0;
   mCode2           = 0;

   mDefaultSelect   = 0;

   mMatrix_PG.initialize(3,3);
   mMatrix_GP.initialize(3,3);
   mMatrix_PA.initialize(3,3);
   mMatrix_AP.initialize(3,3);
   mMatrix_PGGI_1.initialize(3,3);
   mMatrix_PGGI_2.initialize(3,3);
   mMatrix_PGGI_3.initialize(3,3);
}

void Transforms_Settings::show()
{
   printf("\n");
   printf("Transforms_Settings*********************************************************\n");
   printf("\n");

   printf("Code1               %d\n", mCode1);
   printf("Code2               %d\n", mCode2);
   printf("\n");

   printf("DefaultSelect       %d\n", mDefaultSelect);
   printf("\n");

   if (mMatrix_PG.mValidFlag)
   {
      mMatrix_PG.show("Matrix_PG");
      printf("\n");
   }

   if (mMatrix_GP.mValidFlag)
   {
      mMatrix_GP.show("Matrix_GP");
      printf("\n");
   }

   if (mMatrix_PA.mValidFlag)
   {
      mMatrix_PA.show("Matrix_PA");
      printf("\n");
   }

   if (mMatrix_AP.mValidFlag)
   {
      mMatrix_AP.show("Matrix_AP");
      printf("\n");
   }

   if (mMatrix_PGGI_1.mValidFlag)
   {
      mMatrix_PGGI_1.show("Matrix_PGGI_1");
      printf("\n");
   }

   if (mMatrix_PGGI_2.mValidFlag)
   {
      mMatrix_PGGI_2.show("Matrix_PGGI_2");
      printf("\n");
   }

   if (mMatrix_PGGI_3.mValidFlag)
   {
      mMatrix_PGGI_3.show("Matrix_PGGI_3");
      printf("\n");
   }
}

//******************************************************************************
// Base class overload, called for each line in the file

void Transforms_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))                 nestedPop(aCmd);

   // Read variables
   if (aCmd->isCmd("Code1"))               mCode1       = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))               mCode2       = aCmd->argInt(1);

   if (aCmd->isCmd("DefaultSelect"))       mDefaultSelect = aCmd->argInt(1);

   if (aCmd->isCmd("Matrix_PG"))           mMatrix_PG.execute(aCmd);
   if (aCmd->isCmd("Matrix_GP"))           mMatrix_GP.execute(aCmd);
   if (aCmd->isCmd("Matrix_PA"))           mMatrix_PA.execute(aCmd);
   if (aCmd->isCmd("Matrix_AP"))           mMatrix_AP.execute(aCmd);
   if (aCmd->isCmd("Matrix_PGGI_1"))       mMatrix_PGGI_1.execute(aCmd);
   if (aCmd->isCmd("Matrix_PGGI_2"))       mMatrix_PGGI_2.execute(aCmd);
   if (aCmd->isCmd("Matrix_PGGI_3"))       mMatrix_PGGI_3.execute(aCmd);
}

