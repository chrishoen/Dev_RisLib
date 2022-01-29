#include "stdafx.h"

#include "risThreadsProcess.h"

#include "someTimerThread.h"
using namespace Some;

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize

void main_initialize(int argc,char** argv)
{
   // Enter process
   Ris::Threads::enterProcessHigh();

   // Initialize print facility.
   Prn::resetPrint("127.0.0.1");
   Prn::resetPrint("192.168.1.134");
   Prn::useConsole(1);
   Prn::useConsole(2);
   Prn::initializePrint();

   // Initialize print filters
   Prn::setFilter(Prn::View01, true);
   Prn::setFilter(Prn::View02, true);
   Prn::setFilter(Prn::View03, true);
   Prn::setFilter(Prn::View04, true);

   Prn::setFilter(Prn::View11, true, 1);
   Prn::setFilter(Prn::View12, true, 1);
   Prn::setFilter(Prn::View13, true, 1);
   Prn::setFilter(Prn::View14, true, 1);

   Prn::setFilter(Prn::View21, true, 2);
   Prn::setFilter(Prn::View22, true, 2);
   Prn::setFilter(Prn::View23, true, 2);
   Prn::setFilter(Prn::View24, true, 2);


   Log::reset();
   Log::openFile(0,"LogPrintTest.txt");

   Prn::print(0,"Program PrintTest***********************************BEGIN");
   Log::write(0,"Program PrintTest***********************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize

void main_finalize()
{
   Prn::print(0,"Program PrintTest***********************************END");
   Log::write(0,"Program PrintTest***********************************END");
   Log::closeAllFiles();

   // Close print
   Prn::finalizePrint();

   // Exit process
   Ris::Threads::exitProcess();

}

