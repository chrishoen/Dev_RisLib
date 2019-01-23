#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "someTestQCallThread.h"
#include "someRandomTimerThread.h"

#include "MainInit.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Begin program.

   main_initialize(argc,argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   Some::gTestQCallThread = new Some::TestQCallThread;
   Some::gTestQCallThread->launchThread();

   Some::gRandomTimerThread1 = new Some::RandomTimerThread(1);
   Some::gRandomTimerThread1->launchThread();

   Some::gRandomTimerThread2 = new Some::RandomTimerThread(2);
   Some::gRandomTimerThread2->launchThread();

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

   Some::gRandomTimerThread1->shutdownThread();
   Some::gRandomTimerThread2->shutdownThread();
   Some::gTestQCallThread->shutdownThread();

   delete Some::gRandomTimerThread1;
   delete Some::gRandomTimerThread2;
   delete Some::gTestQCallThread;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
 //return 0;

   printf("press enter\n");
   getchar();
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

