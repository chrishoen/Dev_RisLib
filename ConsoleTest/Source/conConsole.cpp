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
   mCursor = 0;
   mInputLength = 0;
   mInputString[0] = 0;

   while (true)
   {
      mKeyIn = readOne();

      if (isEndOfRead())
      {
         Prn::print(Prn::View11, "end of read");
         writeNewLine();
         break;
      }

      switch (mKeyIn)
      {
      case cKey_Ignore: onKey_Enter(); break;
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

      Prn::print(Prn::View11, "mInput %3d $ %4d $  %3d %s", 
         mCursor, 
         mKeyIn, 
         mInputLength,
         mInputString);

   }

};

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_Ignore()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_Enter()
{
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_BackSpace()
{ 
   if (mCursor == 0) return;
   writeOne(8);
   mCursor--;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_Delete()
{ 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_LeftArrow()
{
   if (mCursor == 0) return;
   writeOne(8);
   mCursor--;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_RightArrow()
{ 
   if (mCursor < mInputLength)
   {
      writeOne(mInputString[mCursor]);
      mCursor++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_UpArrow()
{ 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_DownArrow()
{ 
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_Home()
{ 
   if (mCursor == 0) return;
   writeOne(13);
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_End()
{ 
   while (mCursor < mInputLength)
   {
      writeOne(mInputString[mCursor]);
      mCursor++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_Default()
{ 
   Prn::print(Prn::View14, "Keyin Default");

   writeOne(mKeyIn);

   mInputString[mCursor] = mKeyIn;
   mInputString[mCursor + 1] = 0;
   mInputLength = (int)strlen(mInputString);

   mCursor++;
}

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
