#include "prnPrint.h"
#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "risRemoteServerThread.h"

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

   Ris::Remote::gServerThread = new Ris::Remote::ServerThread;
   Ris::Remote::gServerThread->configure(55001);
   Ris::Remote::gServerThread->launchThread();

   //--------------------------------------------------------------------
   // Run user command line executive,
   // It returns when user exits

   CmdLineExec* tExec = new CmdLineExec;
   Ris::executeCmdLineConsole(tExec);
   delete tExec;

   //--------------------------------------------------------------------
   // Shutdown threads

   Ris::Remote::gServerThread->shutdownThread();
   delete Ris::Remote::gServerThread;

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
   Prn::initializePrint(Prn::PrintModeLocalThreaded);
   Prn::setFilter(Prn::SocketInit,Prn::Init1,false);
   Prn::setFilter(Prn::SocketInit,Prn::Init2,true);
   Prn::setFilter(Prn::SocketRun,Prn::Run1,false);
   Prn::setFilter(Prn::SocketRun,Prn::Run2,false);
   Prn::setFilter(Prn::SocketRun,Prn::Run3,false);
   Prn::setFilter(Prn::SocketRun,Prn::Run4,false);

   Prn::setFilter(Prn::ThreadInit,Prn::Any,true);
   Prn::setFilter(Prn::ThreadRun,Prn::Run1,true);
   Prn::setFilter(Prn::ThreadRun,Prn::Run2,false);
   Prn::setFilter(Prn::ThreadRun,Prn::Run3,false);
   Prn::setFilter(Prn::ThreadRun,Prn::Run4,false);

   Prn::setFilter(Prn::ProcRun,Prn::Run1,true);
   Prn::setFilter(Prn::ProcRun,Prn::Run2,true);
   Prn::setFilter(Prn::ProcRun,Prn::Run3,false);
   Prn::setFilter(Prn::ProcRun,Prn::Run4,false);
   Prn::setFilter(Prn::ProcPer,Prn::Per1,true);

   Prn::print(0,0,"RemoteServer*****************************************BEGIN");
}

//******************************************************************************
// Main finalize

void amain_finalize()
{
   Prn::print(0,0,"RemoteServer*****************************************END");

   // Exit process
   Ris::Threads::exitProcess();

   // Close print
   Prn::closePrint();
}

