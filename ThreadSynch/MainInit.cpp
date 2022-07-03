#include "stdafx.h"

#include "risThreadsProcess.h"

#include "someTimerThread.h"
using namespace Some;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize.

void main_initialize(int argc,char** argv)
{
   // Enter process.
   Ris::Threads::enterProcessHigh();

   // Initialize print facility.
   Prn::resetPrint();
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::ThreadRun1,  true);
   Prn::setFilter(Prn::ThreadRun2,  false);
   Prn::setFilter(Prn::ThreadRun3,  true);
   Prn::setFilter(Prn::ThreadRun4,  true);

   Prn::print(0,"ThreadSynch*******************************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize.

void main_finalize()
{
   Prn::print(0,"ThreadSynch*******************************************END");

   // Close print.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
