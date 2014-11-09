#include "prnPrint.h"
#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "someThread1.h"
#include "someThread2.h"
#include "someTimerThread.h"
using namespace Some;

void amain_initialize(int argc,char** argv);
void amain_finalize();

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Initialize

   amain_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Launch threads

   gThread1 = new Thread1;
// gThread1->launchThread();

   gThread2 = new Thread2;
// gThread2->launchThread();

   gTimerThread = new TimerThread;
   gTimerThread->launchThread();

   //--------------------------------------------------------------------
   // Start user command line executive,
   // It returns when user exits

   CmdLineExec* tExec = new CmdLineExec;
   Ris::executeCmdLineConsole(tExec);
   delete tExec;

   //--------------------------------------------------------------------
   // Shutdown threads

   gThread2->shutdownThread();
   delete gThread2;

   gThread1->shutdownThread();
   delete gThread1;

   gTimerThread->shutdownThread();
   delete gTimerThread;

   //--------------------------------------------------------------------
   // Exit
   
   amain_finalize();

   return 0;
}

//******************************************************************************
// Initialize

void amain_initialize(int argc,char** argv)
{
   // Enter process
   Ris::Threads::enterProcessHigh();

   // Initialize print filters
   Prn::initializePrintFromSettings();

   Prn::print(0,0,"ThreadTest*******************************************BEGIN");
}

//******************************************************************************
// Main finalize

void amain_finalize()
{
   Prn::print(0,0,"ThreadTest*******************************************END");

   // Exit process
   Ris::Threads::exitProcess();

   // Close print
   Prn::closePrint();
}

