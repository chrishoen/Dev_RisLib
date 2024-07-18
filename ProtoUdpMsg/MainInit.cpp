
#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risBaseDir.h"
#include "procoUdpParms.h"
#include "MainArgs.h"
#include "MainInit.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize.

void main_initialize()
{
   printf("ProtoUdpMsg Program**********************************************BEGIN\n");

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
   ////Ris::Threads::enterProcessHigh();

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
   // Initialize trace facility.

   Trc::reset();
   Trc::create_buffer(1,  3, "message");
   Trc::create_buffer(11, 3, "udp");
   Trc::set_default_trace_index(11);
   Trc::create_log(11, 4, "log/ProtoUdpMsg_trace11.log");
   Trc::initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read parameters files.

   if (gMainArgs.mPeerIdent == 1)
   {
      ProtoComm::gUdpParms.reset();
      ProtoComm::gUdpParms.readSection("default");
      ProtoComm::gUdpParms.readSection("UdpPeer1");
   }
   else if (gMainArgs.mPeerIdent == 2)
   {
      ProtoComm::gUdpParms.reset();
      ProtoComm::gUdpParms.readSection("default");
      ProtoComm::gUdpParms.readSection("UdpPeer2");
   }
   else
   {
      printf("MAIN ARGS ERROR\n");
      exit(-1);
   }
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
   ////Ris::Threads::exitProcess();

   printf("ProtoUdpMsg Program**********************************************BEGIN\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
