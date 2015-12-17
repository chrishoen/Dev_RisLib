#include "prnPrint.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

void amainInit(int argc,char** argv);

#include "someControllerThread.h"
#include "somePlantThread.h"
using namespace Some;

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Begin program

   amainInit(argc,argv);

   //--------------------------------------------------------------------
   // Launch Threads

   gPlantThread = new PlantThread;
   gPlantThread->launchThread();

   gControllerThread = new ControllerThread;
   gControllerThread->launchThreads();
   //--------------------------------------------------------------------
   // Start user command line executive,
   // Wait for user to exit

   CmdLineExec* exec = new CmdLineExec;
   Ris::executeCmdLineConsole(exec);
   delete exec;

   gControllerThread->shutdownThreads();
   delete gControllerThread;

   //gPlantThread->shutdownThread();
   gPlantThread->forceTerminateThread();
   delete gPlantThread;


   return 0;
}

//******************************************************************************
// program.exe configFilename entityID

void amainInit(int argc,char** argv)
{
   //---------------------------------------------------------------------------
   // Initialize print facility

   Prn::initializePrint();

   Prn::setFilter(Prn::SocketInit1,false);
   Prn::setFilter(Prn::SocketInit2,true);
   Prn::setFilter(Prn::SocketRun1,false);
   Prn::setFilter(Prn::SocketRun2,false);
   Prn::setFilter(Prn::SocketRun3,false);
   Prn::setFilter(Prn::SocketRun4,false);

   Prn::setFilter(Prn::ThreadInit1,true);
   Prn::setFilter(Prn::ThreadRun1,true);
   Prn::setFilter(Prn::ThreadRun2,false);
   Prn::setFilter(Prn::ThreadRun3,false);
   Prn::setFilter(Prn::ThreadRun4,false);

   Prn::setFilter(Prn::ProcRun1,true);
   Prn::setFilter(Prn::ProcRun2,true);
   Prn::setFilter(Prn::ProcRun3,false);
   Prn::setFilter(Prn::ProcRun4,true);

   //---------------------------------------------------------------------------
   // Banner

   Prn::print(0,"ThreadTest***************************************************");
}

