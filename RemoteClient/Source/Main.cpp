#include <stdio.h>
#include "prnPrint.h"
#include "risPortableCalls.h"
#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "risRemoteClientThread.h"
#include "CmdLineExec.h"

void main_initialize(int argc,char** argv);
void main_finalize();

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Launch remote client thread

   Ris::portableSleep(2000);

   CmdLineExec* tExec = new CmdLineExec;
   Ris::Remote::gClientThread = new Ris::Remote::RemoteClientThread;
   Ris::Remote::gClientThread->configure("127.0.0.1",55001,tExec);
   Ris::Remote::gClientThread->launchThread();

   //--------------------------------------------------------------------
   // Initialize

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Wait for keypressed
   getchar();

   //--------------------------------------------------------------------
   // Shutdown threads

   Ris::Remote::gClientThread->shutdownThread();
   delete Ris::Remote::gClientThread;

   delete tExec;

   //--------------------------------------------------------------------
   // Exit
   
   main_finalize();

   return 0;
}

//******************************************************************************
// Initialize

void main_initialize(int argc,char** argv)
{
   // Enter process
   Ris::Threads::enterProcessHigh();

   // Initialize print facility
   Prn::resetPrint();
   Prn::usePrintThread(true);

   // Initialize print filters
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

   Prn::initializePrint();

   Prn::print(0,0,"RemoteClient*****************************************BEGIN\n");

}

//******************************************************************************
// Main finalize

void main_finalize()
{
   printf("RemoteClient*****************************************END\n");

   // Close print
   Prn::finalizePrint();

   // Exit process
   Ris::Threads::exitProcess();

}

