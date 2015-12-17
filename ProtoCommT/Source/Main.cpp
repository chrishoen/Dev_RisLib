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

   Prn::setFilter(Prn::SocketInit1,false);
   Prn::setFilter(Prn::SocketInit2,true);
   Prn::setFilter(Prn::SocketRun1,false);
   Prn::setFilter(Prn::SocketRun2,false);
   Prn::setFilter(Prn::SocketRun3,false);
   Prn::setFilter(Prn::SocketRun4,false);

   Prn::setFilter(Prn::ThreadRun1,true);
   Prn::setFilter(Prn::ThreadRun2,false);
   Prn::setFilter(Prn::ThreadRun3,false);
   Prn::setFilter(Prn::ThreadRun4,false);

   Prn::setFilter(Prn::ProcRun1,true);
   Prn::setFilter(Prn::ProcRun2,true);
   Prn::setFilter(Prn::ProcRun3,false);
   Prn::setFilter(Prn::ProcRun4,true);
}


