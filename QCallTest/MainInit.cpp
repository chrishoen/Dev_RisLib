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
   // Enter process

   Ris::Threads::enterProcessHigh();

   // Initialize print facility
   Prn::resetPrint();
   Prn::useConsole(1);
   Prn::initializePrint();

   // Initialize print filters
   Prn::setFilter(Prn::View11, true, 1);
   Prn::setFilter(Prn::View12, false, 1);
   Prn::setFilter(Prn::View13, false, 1);
   Prn::setFilter(Prn::View14, false, 1);

   Prn::print(0, "QCallTest*******************************************BEGIN");
   Prn::print(Prn::View11, "QCallTest*******************************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize

void main_finalize()
{
   Prn::print(0, "QCallTest*******************************************END");
   Prn::print(Prn::View11, "QCallTest*******************************************END");

   // Close print
   Prn::finalizePrint();

   // Exit process
   Ris::Threads::exitProcess();
}

