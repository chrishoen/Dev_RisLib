#include "stdafx.h"


#include "risThreadsProcess.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize

void main_initialize(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Process configuration.

   // Set the program current working directory up one level from the 
   // program bin directory.
   Ris::portableChdirUpFromBin();

   // Set the process priority class.
   Ris::Threads::enterProcessHigh();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   Prn::resetPrint();
   Prn::initializePrint();

   // Initialize print filters
   Prn::setFilter(Prn::Show1, true);
   Prn::setFilter(Prn::Show2, false);
   Prn::setFilter(Prn::Show3, false);
   Prn::setFilter(Prn::Show4, false);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize trace facility.

   Trc::reset();
   Trc::create_buffer(1, 3, "test1");
   Trc::create_buffer(4, 6, "test4");
   Trc::create_log(1, 4, "log/trace1.log");
   Trc::initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************

   Prn::print(0, "Trace *******************************************BEGIN");
   Prn::print(0, "current dir %s", Ris::portableGetCurrentDir());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize

void main_finalize()
{
   Prn::print(0, "Trace *******************************************END");

   // Close print.
   Prn::finalizePrint();

   // Close trace.
   Trc::finalize();

   // Exit process
   Ris::Threads::exitProcess();
}

