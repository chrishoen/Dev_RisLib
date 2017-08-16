#include "stdafx.h"

#include "risThreadsProcess.h"
#include "protoserialSettings.h"

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
   Prn::setFilter(Prn::SocketInit1,true);
   Prn::setFilter(Prn::SocketInit2,true);
   Prn::setFilter(Prn::SocketRun1, false);
   Prn::setFilter(Prn::SocketRun2, false);
   Prn::setFilter(Prn::SocketRun3, false);

   Prn::setFilter(Prn::SocketRun4, false);

   Prn::setFilter(Prn::SerialInit1,false);
   Prn::setFilter(Prn::SerialInit2,true);
   Prn::setFilter(Prn::SerialRun1, true);
   Prn::setFilter(Prn::SerialRun2, true);
   Prn::setFilter(Prn::SerialRun3, true);
   Prn::setFilter(Prn::SerialRun4, true);

   Prn::setFilter(Prn::ThreadRun1, true);
   Prn::setFilter(Prn::ThreadRun2, false);
   Prn::setFilter(Prn::ThreadRun3, false);
   Prn::setFilter(Prn::ThreadRun4, false);

   Prn::print(0,"ProtoSerialS*******************************************BEGIN %d %s",argc,argv[1]);

   //---------------------------------------------------------------------------
   // Initialize Settings

   if (argc == 2)
   {
      ProtoSerial::gSettings.readSection(argv[1]);
   }
   else
   {
      ProtoSerial::gSettings.readSection("SerialPeer1");
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize

void main_finalize()
{
   Prn::print(0,"ProtoSerialU*******************************************END");

   // Close print
   Prn::finalizePrint();

   // Exit process
   Ris::Threads::exitProcess();

}

