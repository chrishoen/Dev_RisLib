#include "stdafx.h"

#include "risThreadsProcess.h"
#include "smShare.h"


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program resources.

void main_initialize(int argc,char** argv)
{
   printf("SharedMemory Program********************************************BEGIN\n");
   printf("SharedMemory Program********************************************BEGIN\n");
   printf("SharedMemory Program********************************************BEGIN\n\n");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   // Initialize print.
   Prn::resetPrint();
   Prn::useConsole(1);
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::View11, true,  1);
   Prn::setFilter(Prn::View12, false, 1);
   Prn::setFilter(Prn::View13, false, 1);
   Prn::setFilter(Prn::View14, false, 1);

   Prn::setFilter(Prn::Show1, true);
   Prn::setFilter(Prn::Show2, true);
   Prn::setFilter(Prn::Show3, false);
   Prn::setFilter(Prn::Show4, false);
   Prn::setFilter(Prn::Show5, false);
   Prn::setFilter(Prn::Show6, false);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // initialize.

   // Initialize shared memory.
   SM::initializeShare();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize program resourcs.

void main_finalize()
{
   // Finalize shared memory.
   SM::finalizeShare();

   // Finalize print facility.
   Prn::finalizePrint();

   // Done.
   printf("\n");
   printf("SharedMemory Program********************************************END\n\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
