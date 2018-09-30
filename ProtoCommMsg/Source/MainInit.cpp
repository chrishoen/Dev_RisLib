#include "stdafx.h"


#include "risThreadsProcess.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize.

void main_initialize(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   // Initialize print.
   Prn::resetPrint();
   Prn::initializePrint();

   // Initialize print filters.
   Prn::setFilter(Prn::SocketInit1, false);
   Prn::setFilter(Prn::SocketInit2, true);
   Prn::setFilter(Prn::SocketRun1, false);
   Prn::setFilter(Prn::SocketRun2, false);
   Prn::setFilter(Prn::SocketRun3, false);
   Prn::setFilter(Prn::SocketRun4, false);

   Prn::setFilter(Prn::ThreadRun1, true);
   Prn::setFilter(Prn::ThreadRun2, false);
   Prn::setFilter(Prn::ThreadRun3, false);
   Prn::setFilter(Prn::ThreadRun4, false);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   Prn::print(0,"ExampleMsg Program*******************************************BEGIN");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize.

void main_finalize()
{
   Prn::print(0,"ExampleMsg Program*******************************************END");

   // Close print facility.
   Prn::finalizePrint();
}

