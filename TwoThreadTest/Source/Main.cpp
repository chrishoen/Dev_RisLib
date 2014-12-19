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

   //---------------------------------------------------------------------------
   // Banner

   Prn::print(0,0,"ThreadTest***************************************************");
}

