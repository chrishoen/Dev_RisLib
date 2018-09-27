
#include "stdafx.h"

#include "risThreadsProcess.h"

#include "fcomSettings.h"


//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize.

void main_initialize(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Enter process.

   Ris::Threads::enterProcessHigh();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   Prn::resetPrint();
   Prn::useConsole(1);
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::SerialInit1, true);
   Prn::setFilter(Prn::SerialInit2, true);
   Prn::setFilter(Prn::SerialRun1,  true);
   Prn::setFilter(Prn::SerialRun2,  true);
   Prn::setFilter(Prn::SerialRun3,  false);
   Prn::setFilter(Prn::SerialRun4,  false);

   Prn::setFilter(Prn::ThreadInit1, true);
   Prn::setFilter(Prn::ThreadInit1, true);
   Prn::setFilter(Prn::ThreadRun1,  true);
   Prn::setFilter(Prn::ThreadRun2,  true);
   Prn::setFilter(Prn::ThreadRun3,  true);
   Prn::setFilter(Prn::ThreadRun4,  true);

   Prn::setFilter(Prn::ProcRun1,    true);
   Prn::setFilter(Prn::ProcRun2,    true);
   Prn::setFilter(Prn::ProcRun3,    false);
   Prn::setFilter(Prn::ProcRun4,    true);

   Prn::setFilter(Prn::ViewRun1,    true, 1);
   Prn::setFilter(Prn::ViewRun2,    true, 1);
   Prn::setFilter(Prn::ViewRun3,    false,1);
   Prn::setFilter(Prn::ViewRun4,    true, 1);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read parameters files.

   FCom::gSettings.reset();
   FCom::gSettings.readSection("default");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   Prn::print(0,"FComm Program**********************************************BEGIN");

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize.

void main_finalize()
{
   Prn::print(0,"FComm Program**********************************************END");

   // Close print
   Prn::finalizePrint();

   // Exit process
   Ris::Threads::exitProcess();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
