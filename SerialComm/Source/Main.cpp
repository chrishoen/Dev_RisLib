#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "someSerialRxThread.h"
#include "someSerialTxThread.h"
#include "MainInit.h"

using namespace Some;

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Initialize

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Launch threads

   gSerialRxThread = new SerialRxThread();
   gSerialRxThread->configure(5,"38400,N,8,1",4000);
   gSerialRxThread->launchThread();

   gSerialTxThread = new SerialTxThread();
   gSerialTxThread->configure(6,"38400,N,8,1",0);
   gSerialTxThread->launchThread();

   //--------------------------------------------------------------------
   // Start user command line executive,
   // It returns when user exits

   CmdLineExec* tExec = new CmdLineExec;
   Ris::executeCmdLineConsole(tExec);
   delete tExec;

   //--------------------------------------------------------------------
   // Shutdown threads

   gSerialTxThread->shutdownThread();
   gSerialRxThread->shutdownThread();

   //--------------------------------------------------------------------
   // Exit
   
   main_finalize();

   return 0;
}

