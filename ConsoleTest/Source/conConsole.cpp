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

      doTouchCursor();

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

void Console::doTouchCursor()
{
   if (mInputLength < 2) return;
   if (mCursor == 0)
   {
      writeOne(mInputString[0]);
      writeOne(8);
   }
   else if (mCursor == mInputLength)
   {
      writeOne(8);
      writeOne(mInputString[mCursor - 1]);
   }
   else
   {
      writeOne(8);
      writeOne(mInputString[mCursor - 1]);
   }

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Console::onKey_Ignore()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the cursor fro the beginning.

void Console::onKey_Enter()
{
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Move the cursor left by one (backspace) and delete the character there. 

void Console::onKey_BackSpace()
{ 
   if (mCursor == 0) return;
   writeOne(8);
   mCursor--;
   onKey_Delete();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shift the input string left by one, starting at the cursor. This overwrites
// the character at the cursor with the one after it and reduces the length of
// the input string by one.

void Console::onKey_Delete()
{ 
   // If at the end then exit.
   if (mCursor == mInputLength) return;

   // Shift the input string from the cursor to the end left by one
   // and write it out. This over writes the character at the cursor.
   for (int i = mCursor; i < mInputLength; i++)
   {
      mInputString[i] = mInputString[i + 1];
      writeOne(mInputString[i]);
   }
   mInputString[mInputLength - 1] = 0;
   mInputLength--;
   // Erase the last character.
   writeOne(' ');

   // Go back from the end to the cursor.
   int tCount = mInputLength - mCursor + 2;
   for (int i = 0; i < tCount; i++)
   {
      writeOne(8);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write one backspace. This moves the cursor left by one.

void Console::onKey_LeftArrow()
{
   if (mCursor == 0) return;
   writeOne(8);
   mCursor--;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write the input string character at the cursor. This moves the cursor to
// the right by one.

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
// Write a line feed to move the cursor to the beginning and set the cursor
// to zero.

void Console::onKey_Home()
{ 
   if (mCursor == 0) return;
   writeOne(13);
   mCursor = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Move the cursor to the end by writing the input string from the cursor
// to the end.

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
// Insert the input key into the input string at the cursor. Write the
// new string, including the new character, from the cursor to the end.
// Go back from the end to the new cursor.

void Console::onKey_Default()
{ 
   // Shift right by one all keys at and to the right of the cursor.
   mInputLength = (int)strlen(mInputString);
   for (int i = mInputLength; i > mCursor; i--)
   {
      mInputString[i] = mInputString[i - 1];
   }
   mInputString[mInputLength + 1] = 0;
   mInputLength++;

   // Set the input key at the cursor.
   mInputString[mCursor] = mKeyIn;

   // Write the new string from the cursor to the end.
   for (int i = mCursor; i < mInputLength; i++)
   {
      writeOne(mInputString[i]);
   }

   // Set the new cursor and go back to it.
   mCursor++;
   for (int i = mCursor; i < mInputLength; i++)
   {
      writeOne(8);
   }
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
