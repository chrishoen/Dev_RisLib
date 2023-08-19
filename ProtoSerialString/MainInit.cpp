
#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risBaseDir.h"

#include "procoSerialParms.h"
#include "MainArgs.h"
#include "MainInit.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize.

void main_initialize()
{
   printf("ProtoSerialString Program****************************************BEGIN\n");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Process configuration.

   // Set the program current working directory up one level from the 
   // program bin directory.
   Ris::portableChdirUpFromBin();

   // Set the base directory to the current working directory.
   Ris::setBaseDirectoryToCurrent();

   // Set the process priority class.
   Ris::Threads::enterProcessHigh();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   // Initialize print.
   Prn::resetPrint();
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::Show1, true);
   Prn::setFilter(Prn::Show2, false);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read parameters files.

   if (gMainArgs.mPeerIdent == 1)
   {
      ProtoComm::gSerialParms.reset();
      ProtoComm::gSerialParms.readSection("default");
      Prn::print(0, "PARMS PEER1 DEFAULT %s", ProtoComm::gSerialParms.mSerialPortDevice);
   }
   else if (gMainArgs.mPeerIdent == 2)
   {
      ProtoComm::gSerialParms.reset();
      ProtoComm::gSerialParms.readSection("Peer2");
      Prn::print(0, "PARMS PEER2 %s", ProtoComm::gSerialParms.mSerialPortDevice);
   }
   else
   {
      printf("MAIN ARGS ERROR\n");
      exit(-1);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize trace facility.

   Trc::reset();
   Trc::create_buffer(1,  3, "string");
   Trc::create_buffer(11, 3, "serial");
   Trc::set_default_trace_index(11);

   if (ProtoComm::gSerialParms.mTraceLogEnable)
   {
      Trc::create_log(11, 4, "log/trace-ProtoSerialString-serial.log");
   }
   Trc::initialize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize.

void main_finalize()
{
   // Finalize print facility.
   Prn::finalizePrint();

   // Finalize trace facility.
   Trc::finalize();

   // Exit process
   Ris::Threads::exitProcess();

   printf("ProtoSerialString Program****************************************END\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
