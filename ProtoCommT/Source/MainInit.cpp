#include "stdafx.h"

#include "risThreadsProcess.h"
#include "procoSettings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize

void main_initialize(int argc,char** argv)
{
   //---------------------------------------------------------------------------
   // Enter process

   Ris::Threads::enterProcessHigh();

   //---------------------------------------------------------------------------
   // Initialize print facility

   Prn::resetPrint();
   Prn::initializePrint();

   // Initialize print filters
   Prn::setFilter(Prn::SocketInit1,false);
   Prn::setFilter(Prn::SocketInit2,true);
   Prn::setFilter(Prn::SocketRun1,true);
   Prn::setFilter(Prn::SocketRun2,true);
   Prn::setFilter(Prn::SocketRun3,true);
   Prn::setFilter(Prn::SocketRun4,true);

   Prn::setFilter(Prn::ThreadRun1,true);
   Prn::setFilter(Prn::ThreadRun2,true);
   Prn::setFilter(Prn::ThreadRun3,false);
   Prn::setFilter(Prn::ThreadRun4,false);

   Prn::print(0,"ProtoCommT*******************************************BEGIN");

   //---------------------------------------------------------------------------
   // Initialize Settings

   if (argc == 1)
   {
      ProtoComm::gSettings.readSection("default");
      ProtoComm::gSettings.readSection(argv[1]);
      ProtoComm::gSettings.show();
   }
   else
   {
      ProtoComm::gSettings.readSection("default");
      ProtoComm::gSettings.readSection("TcpClient1");
      ProtoComm::gSettings.show();
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize

void main_finalize()
{
   Prn::print(0,"ProtoCommT*******************************************END");

   // Close print
   Prn::finalizePrint();

   // Exit process
   Ris::Threads::exitProcess();

}

