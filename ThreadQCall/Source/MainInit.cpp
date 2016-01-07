#include <windows.h>

#include "prnPrint.h"
#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

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
   Prn::initializePrint();

   // Initialize print filters
   Prn::setFilter(Prn::SocketInit1, false);
   Prn::setFilter(Prn::SocketInit2,  true);
   Prn::setFilter(Prn::SocketRun1,  false);
   Prn::setFilter(Prn::SocketRun2,  false);
   Prn::setFilter(Prn::SocketRun3,  false);
   Prn::setFilter(Prn::SocketRun4,  false);

   Prn::setFilter(Prn::ThreadRun1,   true);
   Prn::setFilter(Prn::ThreadRun2,  false);
   Prn::setFilter(Prn::ThreadRun3,   true);
   Prn::setFilter(Prn::ThreadRun4,  false);

   Prn::setFilter(Prn::ProcRun1,     true);
   Prn::setFilter(Prn::ProcRun2,     true);
   Prn::setFilter(Prn::ProcRun3,    false);
   Prn::setFilter(Prn::ProcRun4,     true);

   Prn::setFilter(Prn::QCallInit1,   true);
   Prn::setFilter(Prn::QCallInit2,   true);
   Prn::setFilter(Prn::QCallRun1,   false);
   Prn::setFilter(Prn::QCallRun2,   false);
   Prn::setFilter(Prn::QCallRun3,   false);
   Prn::setFilter(Prn::QCallRun4,   false);

   Prn::print(0,"ThreadTest*******************************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize

void main_finalize()
{
   Prn::print(0,"ThreadTest*******************************************END");

   // Close print
   Prn::finalizePrint();

   // Exit process
   Ris::Threads::exitProcess();
}
