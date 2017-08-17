#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "protoserialSettings.h"
#include "protoserialSerialThread.h"

#include "MainInit.h"

using namespace ProtoSerial;

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Begin program

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Launch threads

   gSerialThread = new SerialThread;
   gSerialThread->launchThread();

   //--------------------------------------------------------------------
   // Start user command line executive,
   // Wait for user to exit

   CmdLineExec* exec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(exec);
   delete exec;

   //--------------------------------------------------------------------
   // End program

   gSerialThread->shutdownThread();
   delete gSerialThread;

   main_finalize();

   return 0;
}
