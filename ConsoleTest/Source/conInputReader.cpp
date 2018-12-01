/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <conio.h>
#include <ctype.h>

#define  _CONSTRINGREADER_CPP_
#include "conInputReader.h"

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

InputReader::InputReader()
{
   resetVariables();
}

void InputReader::resetVariables()
{
   mInputLength = 0;
   mInputString[0] = 0;
   mOutputString[0] = 0;
}

void InputReader::initialize()
{
   Prn::print(Prn::View01, "InputReader::initialize*****************************");
   Prn::print(Prn::View11, "InputReader::initialize*****************************");

   resetVariables();
   mInputHandle = GetStdHandle(STD_INPUT_HANDLE);
}

void InputReader::finalize()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a string from the console input and copy it to the input
// argument.

void InputReader::doReadString(char* aInputString)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Locals.

   DWORD tNumRead = 0;
   DWORD tNumWritten = 0;

   //************************************************************************
   //************************************************************************
   //************************************************************************
   // Read the first key.

   // Flush the input and loop to read console input key records for the
   // first key input. Store the key records in the input buffer.
   // If the first key is a control then process it and exit the loop.
   mInputCount = 0;
   FlushConsoleInputBuffer(mInputHandle);
   while (true)
   {
      // Read a console input record.
      ReadConsoleInput(
         mInputHandle,
         &mInputRecord,
         1,
         &tNumRead);

      // Process the console input record for key events.
      if (mInputRecord.EventType == KEY_EVENT)
      {
         //******************************************************************
         //******************************************************************
         //******************************************************************

         // Store the record in the input buffer and increment the
         // input buffer count.
         mInputBuffer[mInputCount++] = mInputRecord;

         //******************************************************************
         //******************************************************************
         //******************************************************************
         // If the input record is for a printable char
         // then exit the loop.

         if (mInputRecord.Event.KeyEvent.bKeyDown &&
            isprint(mInputRecord.Event.KeyEvent.uChar.AsciiChar))
         {
            break;
         }

         //******************************************************************
         //******************************************************************
         //******************************************************************
         // If the input record is for an up or down arrow
         // then exit the loop.

         if (mInputRecord.Event.KeyEvent.bKeyDown &&
            mInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_UP)
         {
            break;
         }

         if (mInputRecord.Event.KeyEvent.bKeyDown &&
            mInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
         {
            break;
         }

         //******************************************************************
         //******************************************************************
         //******************************************************************
         // If the input record is for a control char
         // then reset the input count and exit the loop.

         if (mInputRecord.Event.KeyEvent.bKeyDown &&
            mInputRecord.Event.KeyEvent.uChar.AsciiChar != 0 &&
            iscntrl(mInputRecord.Event.KeyEvent.uChar.AsciiChar))
         {
            int tChar = mInputRecord.Event.KeyEvent.uChar.AsciiChar;
            int tState = mInputRecord.Event.KeyEvent.dwControlKeyState;

            if (tChar == 27)
            {
               strcpy(aInputString, "escape");
               return;
            }
            else if (tChar == 13)
            {
               if ((tState & SHIFT_PRESSED))
               {
                  strcpy(aInputString, "shift_enter");
                  return;
               }
               else if ((tState & LEFT_CTRL_PRESSED) || (tState & RIGHT_CTRL_PRESSED))
               {
                  strcpy(aInputString, "control_enter");
                  return;
               }
               else if ((tState & LEFT_ALT_PRESSED) || (tState & RIGHT_ALT_PRESSED))
               {
                  strcpy(aInputString, "alt_enter");
                  return;
               }
               else
               {
                  strcpy(aInputString, "enter");
                  return;
               }
            }
            else
            {
               if ((tState & SHIFT_PRESSED) == 0)
               {
                  sprintf(aInputString,"control_%c", 96 + tChar);
                  return;
               }
               else
               {
                  sprintf(aInputString, "control_shift_%c", 96 + tChar);
                  return;
               }
            }
         }

         //******************************************************************
         //******************************************************************
         //******************************************************************
         // If the input record is for a function key
         // then reset the input count and exit the loop.

         if (mInputRecord.Event.KeyEvent.bKeyDown)
         {
            int tCode = mInputRecord.Event.KeyEvent.wVirtualKeyCode;
            if (VK_F1 <= tCode && tCode <= VK_F12)
            {
               int tFunctionNum = tCode - VK_F1 + 1;
               sprintf(aInputString, "function_%d", tFunctionNum);
               return;
            }
         }
      }
   }

   //************************************************************************
   //************************************************************************
   //************************************************************************
   // If the first key is not a control then read the input string.

   // Write the input buffer from the first key back to the console input
   // buffer.
   WriteConsoleInput(
      mInputHandle,
      mInputBuffer,
      mInputCount,
      &tNumWritten);

   // Read the input string from the console input. This is in buffered 
   // mode and will allow string editing.
   ReadConsole(
      mInputHandle,
      aInputString,
      200,
      &tNumRead,
      NULL);
   aInputString[tNumRead] = 0;
   if (tNumRead > 1) aInputString[tNumRead - 2] = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void InputReader::doTestLoop1()
{
   int tCount = 0;
   char  tString[200];

   while (true)
   {
      doReadString(tString);
      Prn::print(Prn::View11, "%4d $ %s",++tCount,tString);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

