/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <conio.h>
#include <ctype.h>

#include "conReadWrite.h"

#define  _CONCONSOLE_CPP_
#include "conConsole.h"

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Console::Console()
{
}

Console::~Console()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void Console::doTestLoop1()
{
   Prn::print(Prn::View11, "doTestLoop1****************************");
   while (true)
   {
      int tKeyIn = 0;
      tKeyIn = readOne();

      if (isEndOfRead())
      {
         Prn::print(Prn::View11, "end of read");
         writeNewLine();
         break;
      }

      if (isPrintable())
      {
         writeOne(tKeyIn);
      }

      Prn::print(Prn::View11, "tKeyIn %d", tKeyIn);

      switch (tKeyIn)
      {
      case cKey_Enter: onKey_Enter(); break;
      case cKey_BackSpace: onKey_BackSpace(); break;
      case cKey_Delete: onKey_Delete(); break;
      case cKey_LeftArrow: onKey_LeftArrow(); break;
      case cKey_RightArrow: onKey_RightArrow(); break;
      case cKey_UpArrow: onKey_UpArrow(); break;
      case cKey_DownArrow: onKey_DownArrow(); break;
      case cKey_Home: onKey_Home(); break;
      case cKey_End: onKey_End(); break;
      default:onKey_Default();
      }
   }

};

void Console::onKey_Enter()      { Prn::print(Prn::View11, "Keyin Enter"); }
void Console::onKey_BackSpace()  { Prn::print(Prn::View11, "Keyin BackSpace"); }
void Console::onKey_Delete()     { Prn::print(Prn::View11, "Keyin Delete"); }
void Console::onKey_LeftArrow()  { Prn::print(Prn::View11, "Keyin LeftArrow"); }
void Console::onKey_RightArrow() { Prn::print(Prn::View11, "Keyin RightArrow"); }
void Console::onKey_UpArrow()    { Prn::print(Prn::View11, "Keyin UpArrow"); }
void Console::onKey_DownArrow()  { Prn::print(Prn::View11, "Keyin DownArrow"); }
void Console::onKey_Home()       { Prn::print(Prn::View11, "Keyin Home"); }
void Console::onKey_End()        { Prn::print(Prn::View11, "Keyin End"); }
void Console::onKey_Default()    { Prn::print(Prn::View11, "Keyin Default"); }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void Console::doTestLoop2()
{
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
