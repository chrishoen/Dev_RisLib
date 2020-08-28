#include "stdafx.h"

#include "tsThreadServices.h"
#include "risThreadsProcess.h"
#include "procoTcpSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize

void main_initialize(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Enter process.

   Ris::Threads::enterProcessHigh();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   // Initialize print.
   Prn::resetPrint();
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::ThreadInit1,     true);
   Prn::setFilter(Prn::ThreadInit1,     true);
   Prn::setFilter(Prn::ThreadRun1,      true);
   Prn::setFilter(Prn::ThreadRun2,      false);
   Prn::setFilter(Prn::ThreadRun3,      false);
   Prn::setFilter(Prn::ThreadRun4,      false);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read parameters files.

   if (argc == 2)
   {
      ProtoComm::gTcpSettings.reset();
      ProtoComm::gTcpSettings.readSection("default");
      ProtoComm::gTcpSettings.readSection(argv[1]);
      ProtoComm::gTcpSettings.show();
   }
   else
   {
      ProtoComm::gTcpSettings.reset();
      ProtoComm::gTcpSettings.readSection("default");
      ProtoComm::gTcpSettings.readSection("TcpClient1");
      ProtoComm::gTcpSettings.show();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize thread services.

   char tProgramName[100];
   sprintf(tProgramName, "TcpClient%d", ProtoComm::gTcpSettings.mMyAppNumber);

   TS::reset();
   TS::setProgramName(tProgramName);
   TS::setProgramPrintLevel(2);
   TS::initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   Prn::print(0, "ProtoTcpClient***********************************BEGIN %s", argv[1]);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize

void main_finalize()
{
   Prn::print(0, "ProtoTcpClient***********************************END");

   // Close print.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();

   // Finalize thread services.
   TS::finalize();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
