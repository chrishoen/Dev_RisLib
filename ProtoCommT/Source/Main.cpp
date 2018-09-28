#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "procoSettings.h"
#include "procoClientThread.h"
#include "procoServerThread.h"

#include "MainInit.h"

using namespace ProtoComm;



//***********************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Begin program

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Launch threads

   switch (ProtoComm::gSettings.mMyAppRole)
   {
      case ProtoComm::Settings::cTcpServer:
         ProtoComm::gServerThread = new ProtoComm::ServerThread;
         ProtoComm::gServerThread->launchThread();
         break;
      case ProtoComm::Settings::cTcpClient:
         ProtoComm::gClientThread = new ProtoComm::ClientThread;
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
      case ProtoComm::Settings::cTcpServer:
         ProtoComm::gServerThread->shutdownThread();
         delete ProtoComm::gServerThread;
         break;
      case ProtoComm::Settings::cTcpClient:
         ProtoComm::gClientThread->shutdownThread();
         delete ProtoComm::gClientThread;
         break;
   }
      
   main_finalize();

   return 0;
}
