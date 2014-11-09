#include "prnPrint.h"
#include "risPortableCalls.h"
#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "risRemoteClientThread.h"
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
   // Launch remote client thread

   CmdLineExec* tExec = new CmdLineExec;
   Ris::Remote::gClientThread = new Ris::Remote::RemoteClientThread;
   Ris::Remote::gClientThread->configure("127.0.0.1",55001,tExec);
   Ris::Remote::gClientThread->launchThread();

   //--------------------------------------------------------------------
   // Initialize

   amain_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Launch threads

   gThread1 = new Thread1;
   gThread1->launchThread();

   gThread2 = new Thread2;
   gThread2->launchThread();

   gTimerThread = new TimerThread;
   gTimerThread->launchThread();

   //--------------------------------------------------------------------
   // Wait for keypressed
   Ris::portableWaitForKbhit();

   //--------------------------------------------------------------------
   // Shutdown threads

   gThread2->shutdownThread();
   delete gThread2;

   gThread1->shutdownThread();
   delete gThread1;

   gTimerThread->shutdownThread();
   delete gTimerThread;

   Ris::Remote::gClientThread->shutdownThread();
   delete Ris::Remote::gClientThread;

   delete tExec;

   //--------------------------------------------------------------------
   // Exit
   
   amain_finalize();

   return 0;
}

//******************************************************************************
// Initialize

void amain_initialize(int argc,char** argv)
{

   printf("ThreadTest*******************************************BEGIN\n");

   // Enter process
   Ris::Threads::enterProcessHigh();

   // Initialize print filters
   Prn::initializePrint(Prn::PrintModeRemote);

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
}

//******************************************************************************
// Main finalize

void amain_finalize()
{
   printf("ThreadTest*******************************************END\n");

   // Close print
   Prn::closePrint();

   // Exit process
   Ris::Threads::exitProcess();

}

