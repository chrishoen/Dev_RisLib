/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <conio.h>
#include <ctype.h>

#define  _CONKEYREADER_CPP_
#include "conKeyReader.h"

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

KeyRecord::KeyRecord()
{
   reset();
}

void KeyRecord::reset()
{
   mCode = 0;
   mChar = 0;
   mIsPrintable = false;
   mIsEndOfRead = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

KeyReader::KeyReader()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a single console keyboard input. Return it in the input record.

void KeyReader::readKey(KeyRecord* aRecord)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Clear the input record.
   aRecord->reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Local variables.

   int tKeyIn = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read and test.

   // Read the input key.
   tKeyIn = _getch();

   // Test the input for end of read.
   if (tKeyIn == 'z')
   {
      aRecord->mIsEndOfRead = true;
      aRecord->mCode = cKey_EndOfRead;
   }

   // Test the input for enter.
   if (tKeyIn == 13)
   {
      aRecord->mCode = cKey_Enter;
      return;
   }

   // Test the input for enter.
   if (tKeyIn == 's')
   {
      aRecord->mCode = cKey_Special;
      return;
   }

   // Test the input for back space.
   if (tKeyIn == 8)
   {
      aRecord->mCode = cKey_BackSpace;
      return;
   }

   // Test the input for not special.
   if (tKeyIn != 224)
   {
      // Test the input for printable.
      if (isprint(tKeyIn))
      {
         aRecord->mIsPrintable = true;
         aRecord->mCode = cKey_Printable;
         aRecord->mChar = tKeyIn;
         return;
      }
      // Not special and not printable.
      else
      {
         aRecord->mCode = cKey_Ignore;
         return;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read again and test.

   // Read the key input.
   tKeyIn = _getch();

   // Test the input for special keys.
   switch (tKeyIn)
   {
   case  83: aRecord->mCode = cKey_Delete; break;
   case  75: aRecord->mCode = cKey_LeftArrow; break;
   case  77: aRecord->mCode = cKey_RightArrow; break;
   case  72: aRecord->mCode = cKey_UpArrow; break;
   case  80: aRecord->mCode = cKey_DownArrow; break;
   case  71: aRecord->mCode = cKey_Home; break;
   case  79: aRecord->mCode = cKey_End; break;
   default:  aRecord->mCode = cKey_Ignore; break;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write one char.

void KeyReader::writeOne(char aChar)
{
   _putch(aChar);
}

void KeyReader::writeNewLine()
{
   _putch('\n');
}

// Write a string to the console.
void KeyReader::writeString(char* aString)
{
   _cputs(aString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
