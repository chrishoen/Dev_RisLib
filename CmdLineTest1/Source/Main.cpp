#include "stdafx.h"
#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "GSettings.h"

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

   // Initialize print facility
   Prn::resetPrint();
   Prn::initializePrint();

   // Initialize global settings
   gGSettings.initialize();

   Prn::print(0,"CmdLineTest*******************************************BEGIN");
}

//******************************************************************************
// Main finalize

void amain_finalize()
{
   Prn::print(0,"CmdLineTest*******************************************END");

   // Exit process
   Ris::Threads::exitProcess();

   // Close print
   Prn::finalizePrint();
}

