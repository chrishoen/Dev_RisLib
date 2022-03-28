//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"
#include "risSerialSettings.h"

#define  _EXPERIMENT_CPP_
#include "Experiment.h"


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Experiment::Experiment()
{
   mMode = 1;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test .

void Experiment::doRun1()
{
   printf("doRun1*****************************************BEGIN\n");

   while (true)
   {
      // Read from console input.
      char tString[cMaxStringSize];
      if (fgets(tString, cMaxStringSize, stdin) == 0)
      {
         printf("ERROR 1");
         return;
      }

      // Remove cr/lf at end of line.
      my_trimCRLF(tString);

      if (strcmp(tString, "e") == 0) break;

      printf("CMD %s\n", tString);
   }

   printf("doRun1*****************************************END\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
