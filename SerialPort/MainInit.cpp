#include "stdafx.h"

#include "risThreadsProcess.h"
#include "someSerialParms.h"

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
   // Read parameters files.

   if (argc == 2)
   {
      Some::gSerialParms.reset();
      Some::gSerialParms.readSection("default");
      Some::gSerialParms.readSection(argv[1]);
      Some::gSerialParms.show();
   }
   else
   {
      Some::gSerialParms.reset();
      Some::gSerialParms.readSection("default");
      Some::gSerialParms.readSection("Peer1");
      Some::gSerialParms.show();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize print facility.

   if (Some::gSerialParms.mMyAppNumber == 1)
   {
      // Initialize print.
      Prn::resetPrint();
      Prn::useConsole(1);
      Prn::initializePrint();

      // Initialize print filters.
      Prn::setFilter(Prn::View11, true, 1);
      Prn::setFilter(Prn::View12, false, 1);
   }
   else
   {
      // Initialize print.
      Prn::resetPrint();
      Prn::useConsole(2);
      Prn::initializePrint();

      // Initialize print filters.
      Prn::setFilter(Prn::View11, true, 2);
      Prn::setFilter(Prn::View12, false, 2);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   Prn::print(0, "SerialPort Program************************************BEGIN %s", argv[1]);
   Prn::print(0, "SerialPort Program************************************BEGIN %s", argv[1]);
   Prn::print(0, "SerialPort Program************************************BEGIN %s", argv[1]);

   Prn::print(Prn::View11, "SerialPort Program************************************BEGIN %s", argv[1]);
   Prn::print(Prn::View11, "SerialPort Program************************************BEGIN %s", argv[1]);
   Prn::print(Prn::View11, "SerialPort Program************************************BEGIN %s", argv[1]);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Finalize.

void main_finalize()
{
   Prn::print(0, "SerialPort Program*************************************END");
   Prn::print(Prn::View11, "SerialPort Program*************************************END");

   // Finalize print facility.
   Prn::finalizePrint();

   // Exit process.
   Ris::Threads::exitProcess();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
