#include "prnPrint.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "procoSettings.h"
#include "procoDefs.h"
#include "procoClientThread.h"
#include "procoServerThread.h"

void main_initialize(int argc,char** argv);

//***********************************************************************
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
      
   //--------------------------------------------------------------------
   // Launch threads

   switch (ProtoComm::gSettings.mMyAppRole)
   {
      case ProtoComm::AR_TcpServer:
         ProtoComm::gServerThread = new ProtoComm::ServerThread;
         ProtoComm::gServerThread->configure();
         ProtoComm::gServerThread->launchThread();
         break;
      case ProtoComm::AR_TcpClient:
         ProtoComm::gClientThread = new ProtoComm::ClientThread;
         ProtoComm::gClientThread->configure();
         ProtoComm::gClientThread->launchThread();
         break;
   }
      
   //--------------------------------------------------------------------
   // Start user command line executive,
   // Wait for user to exit

   CmdLineExec* exec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(exec);
   delete exec;

   //--------------------------------------------------------------------
   // End program

   switch (ProtoComm::gSettings.mMyAppRole)
   {
      case ProtoComm::AR_TcpServer:
         ProtoComm::gServerThread->shutdownThread();
         delete ProtoComm::gServerThread;
         break;
      case ProtoComm::AR_TcpClient:
         ProtoComm::gClientThread->shutdownThread();
         delete ProtoComm::gClientThread;
         break;
   }
      
   return 0;
}
//******************************************************************************
// amain configfile role

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

   Prn::setFilter(Prn::ProcRun,Prn::Run1,true);
   Prn::setFilter(Prn::ProcRun,Prn::Run2,true);
   Prn::setFilter(Prn::ProcRun,Prn::Run3,false);
   Prn::setFilter(Prn::ProcRun,Prn::Run4,false);
   Prn::setFilter(Prn::ProcPer,Prn::Per1,true);
}


