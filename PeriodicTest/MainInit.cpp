#include "stdafx.h"

#include <sys/mman.h>

#include "risThreadsProcess.h"
#include "risBaseDir.h"
#include "somePeriodicParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program resources.

void main_initialize(int argc,char** argv)
{
   printf("PeriodicTest2 Program****************************************BEGIN\n");
   printf("PeriodicTest2 Program****************************************BEGIN\n");
   printf("PeriodicTest2 Program****************************************BEGIN\n\n");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Lock memory.

   mlockall(MCL_CURRENT | MCL_FUTURE);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Enter process.

   Ris::Threads::enterProcessHigh();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Set the base directory to the current directory.

   Ris::setBaseDirectoryToCurrent();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   // Initialize print.
   Prn::resetPrint();
   Prn::useConsole(1);
   Prn::useConsole(2);
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::ThreadInit1,     true);
   Prn::setFilter(Prn::ThreadRun1,      true);
   Prn::setFilter(Prn::ThreadRun2,      false);
   Prn::setFilter(Prn::ThreadRun3,      false);
   Prn::setFilter(Prn::ThreadRun4,      false);

   Prn::setFilter(Prn::View11, true, 1);
   Prn::setFilter(Prn::View12, false, 1);
   Prn::setFilter(Prn::View21, true, 2);
   Prn::setFilter(Prn::View22, false, 2);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read parameters files.

   // Read parameters files.
   Some::gPeriodicParms.reset();
   Some::gPeriodicParms.readSection("default");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize program resources.

void main_finalize()
{
   // Finalize print facility.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();

   // Done.
   printf("\n");
   printf("PeriodicTest2 Program****************************************END\n\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
