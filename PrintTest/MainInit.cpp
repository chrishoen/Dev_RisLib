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

   // Initialize print facility
   Prn::resetPrint();
   Prn::useConsole(1);
   Prn::useConsole(2);
   Prn::initializePrint();

   // Initialize print filters
   Prn::setFilter(Prn::SocketInitS1,    true);
   Prn::setFilter(Prn::SocketInitS2,    false);
   Prn::setFilter(Prn::SocketRxRunS1,   false);
   Prn::setFilter(Prn::SocketRxRunS2,   false);
   Prn::setFilter(Prn::SocketTxRunS1,   false);
   Prn::setFilter(Prn::SocketTxRunS2,   false);
   Prn::setFilter(Prn::SocketErrorS1,   true);
   Prn::setFilter(Prn::SocketErrorS2,   false);

   Prn::setFilter(Prn::SocketInitT1,    true);
   Prn::setFilter(Prn::SocketInitT2,    false);
   Prn::setFilter(Prn::SocketRxRunT1,   false);
   Prn::setFilter(Prn::SocketRxRunT2,   false);
   Prn::setFilter(Prn::SocketTxRunT1,   false);
   Prn::setFilter(Prn::SocketTxRunT2,   false);
   Prn::setFilter(Prn::SocketErrorT1,   true);
   Prn::setFilter(Prn::SocketErrorT2,   true);

   Prn::setFilter(Prn::ThreadRun1,      true);
   Prn::setFilter(Prn::ThreadRun2,      false);
   Prn::setFilter(Prn::ThreadRun3,      true);
   Prn::setFilter(Prn::ThreadRun4,      true);

   Prn::setFilter(Prn::ProcRun1,        true);
   Prn::setFilter(Prn::ProcRun2,        true   ,1);
   Prn::setFilter(Prn::ProcRun3,        false);
   Prn::setFilter(Prn::ProcRun4,        true);

   Prn::setFilter(Prn::ViewRun1,        true   ,1);
   Prn::setFilter(Prn::ViewRun2,        true   ,1);
   Prn::setFilter(Prn::ViewRun3,        false  ,1);
   Prn::setFilter(Prn::ViewRun4,        true   ,1);

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

