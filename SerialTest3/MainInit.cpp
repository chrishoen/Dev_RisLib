#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risBaseDir.h"

#include "someSerialParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize program resources.

void main_initialize(int argc,char** argv)
{
   printf("SerialTest3 Program*******************************************BEGIN\n");
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Enter process.

   // Set program process for high priority.
   Ris::Threads::enterProcessHigh();

   // Set the base directory to the current directory.
   Ris::setBaseDirectoryToCurrent();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize program resources.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   // Initialize print.
   Prn::resetPrint();
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::Show1, true);
   Prn::setFilter(Prn::Show2, false);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read parameters files.

   Some::gSerialParms.reset();
   Some::gSerialParms.readSection("default");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize program resources.

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize program resourcs.

void main_finalize()
{
   Prn::print(Prn::Show1, "SerialTest3 Program******************************************END");
   printf("\n");

   // Finalize print facility.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
