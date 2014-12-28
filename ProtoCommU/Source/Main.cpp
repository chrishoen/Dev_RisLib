#include <stdlib.h>
#include "prnPrint.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "procoSettings.h"
#include "procoDefs.h"
#include "procoNetworkThread.h"

void main_initialize(int argc,char** argv);

using namespace ProtoComm;

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Begin program

   main_initialize(argc,argv);


   if (argc > 1)
   {
      ProtoComm::gSettings.initialize(argv[1]);
      ProtoComm::gSettings.show();
   }
   return 0;
      
   //--------------------------------------------------------------------
   // Launch threads

   gNetworkThread = new NetworkThread;
   gNetworkThread->configure();
   gNetworkThread->launchThread();

   //--------------------------------------------------------------------
   // Start user command line executive,
   // Wait for user to exit

   CmdLineExec* exec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(exec);
   delete exec;

   //--------------------------------------------------------------------
   // End program

   gNetworkThread->shutdownThread();
   delete gNetworkThread;

   return 0;
}
//******************************************************************************
// main initialize

void main_initialize(int argc,char** argv)
{
   //---------------------------------------------------------------------------
   // Initialize print filters

   Prn::initializePrint();

   Prn::setFilter(Prn::SocketInit,Prn::Init1,false);
   Prn::setFilter(Prn::SocketInit,Prn::Init2,true);
   Prn::setFilter(Prn::SocketRun,Prn::Run1,false);
   Prn::setFilter(Prn::SocketRun,Prn::Run2,false);
   Prn::setFilter(Prn::SocketRun,Prn::Run3,false);
   Prn::setFilter(Prn::SocketRun,Prn::Run4,false);

   Prn::setFilter(Prn::ThreadRun,Prn::Run1,true);
   Prn::setFilter(Prn::ThreadRun,Prn::Run2,false);
   Prn::setFilter(Prn::ThreadRun,Prn::Run3,false);
   Prn::setFilter(Prn::ThreadRun,Prn::Run4,false);
}


