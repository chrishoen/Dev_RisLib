#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "procoSettings.h"
#include "procoClientThread.h"
#include "procoServerThread.h"

#include "MainInit.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc, char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Begin program.

   main_initialize(argc, argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

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
      
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Execute user command line executive, wait for user to exit.

   CmdLineExec* tExec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(tExec);
   delete tExec;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Shutdown program threads.

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
      
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();

   return 0;
}
