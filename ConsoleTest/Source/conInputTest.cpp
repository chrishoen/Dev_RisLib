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
#include "conInputTest.h"

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

InputTest::InputTest()
{
   resetVariables();
}

void InputTest::resetVariables()
{
   mInputLength = 0;
   mInputString[0] = 0;
   mOutputString[0] = 0;
}

void InputTest::initialize()
{
   Prn::print(Prn::View01, "InputTest::initialize*****************************");
   Prn::print(Prn::View11, "InputTest::initialize*****************************");

   resetVariables();
   mInputHandle = GetStdHandle(STD_INPUT_HANDLE);

//  gInputThread = new InputThread;
//  gInputThread->launchThread();
}

void InputTest::finalize()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void InputTest::doTestLoop1()
{
   Prn::print(Prn::View01, "InputTest::doTestLoop1****************************");
   Prn::print(Prn::View11, "InputTest::doTestLoop1****************************");
   Prn::print(Prn::View21, "InputTest::doTestLoop1****************************");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************

   DWORD tRet = 0;

   char  tString[200];
   DWORD tNumRead = 0;
   DWORD tNumWritten = 0;
   int   tCount = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the user input.

   while (true)
   {
      //************************************************************************
      //************************************************************************
      //************************************************************************
      // Read the first key.

      mInputCount = 0;
      Prn::print(Prn::View11, "ReadConsoleInput****************************");

      // Flush the input and loop to read console input key records for the
      // first key input. Store the key records in the input buffer.
      // If the first key is a control then process it and exit the loop.
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

            Prn::print(Prn::View11, "ReadConsoleInput  %4d $ %4d %4d $ %4X %4X",
               mInputCount,
               mInputRecord.Event.KeyEvent.bKeyDown,
               mInputRecord.Event.KeyEvent.uChar.AsciiChar,
               mInputRecord.Event.KeyEvent.dwControlKeyState,
               mInputRecord.Event.KeyEvent.wVirtualKeyCode);

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
               Prn::print(Prn::View11, "ReadConsoleInput  UpArrow");
               break;
            }

            if (mInputRecord.Event.KeyEvent.bKeyDown &&
               mInputRecord.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)
            {
               Prn::print(Prn::View11, "ReadConsoleInput  DownArrow");
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
                  Prn::print(Prn::View21, "escape");
               }
               else if (tChar == 13)
               {
                  if ((tState & SHIFT_PRESSED))
                  {
                     Prn::print(Prn::View21, "shift_enter");
                  }
                  else if ((tState & LEFT_CTRL_PRESSED)|| (tState & RIGHT_CTRL_PRESSED))
                  {
                     Prn::print(Prn::View21, "control_enter");
                  }
                  else if ((tState & LEFT_ALT_PRESSED) || (tState & RIGHT_ALT_PRESSED))
                  {
                     Prn::print(Prn::View21, "alt_enter");
                  }
                  else
                  {
                     Prn::print(Prn::View21, "enter");
                  }
               }
               else
               {
                  if ((tState & SHIFT_PRESSED) == 0)
                  {
                     Prn::print(Prn::View21, "control_%c", 96 + tChar);
                  }
                  else
                  {
                     Prn::print(Prn::View21, "control_shift_%c", 96 + tChar);
                  }
               }

               mInputCount = 0;
               break;
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
                  Prn::print(Prn::View21, "function_%d",tFunctionNum);
               }

               mInputCount = 0;
               break;
            }
         }
      }

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // If the first key is a control then do not read the input string.

      // Continue the main loop.
      if (mInputCount == 0) continue;

      //************************************************************************
      //************************************************************************
      //************************************************************************
      // If the first key is not a control then do read the input string.

      // Write the input buffer from the first key back to the console input
      // buffer.
      WriteConsoleInput(
         mInputHandle,
         mInputBuffer,
         mInputCount,
         &tNumWritten);
      Prn::print(Prn::View11, "WriteConsoleInput %4d", tNumWritten);

      // Read the input string from the console input. This is in buffered 
      // mode and will allow string editing.
      ReadConsole(
         mInputHandle,
         tString,
         200,
         &tNumRead,
         NULL);
      tString[tNumRead] = 0;
      if (tNumRead > 1) tString[tNumRead - 2] = 0;
      Prn::print(Prn::View21, "ReadConsole %4d %4d $ %s", tNumRead,strlen(tString),tString);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void InputTest::doTestLoop2()
{
   DWORD tNumRead = 0;
   int   tCount = 0;

   Prn::print(Prn::View01, "InputTest::doTestLoop2****************************");
   Prn::print(Prn::View11, "InputTest::doTestLoop2****************************");
   Prn::print(Prn::View21, "InputTest::doTestLoop2****************************");

   while (true)
   {
      ReadConsoleInput(
         mInputHandle,
         &mInputRecord,
         1,
         &tNumRead);
      mInputCount++;

      if (mInputRecord.EventType == KEY_EVENT)
      {
         Prn::print(Prn::View11, "ReadConsoleInput  %4d $ %4d %4d $ %4X %4X",
            mInputCount,
            mInputRecord.Event.KeyEvent.bKeyDown,
            mInputRecord.Event.KeyEvent.uChar.AsciiChar,
            mInputRecord.Event.KeyEvent.dwControlKeyState,
            mInputRecord.Event.KeyEvent.wVirtualKeyCode);
      }

      if (mInputRecord.EventType == KEY_EVENT &&
          mInputRecord.Event.KeyEvent.bKeyDown &&
          isprint(mInputRecord.Event.KeyEvent.uChar.AsciiChar)) 
      {
               Prn::print(Prn::View21, "Printable  %4d $ %4d %4d $ %4X %4X",
                  mInputCount,
                  mInputRecord.Event.KeyEvent.bKeyDown,
                  mInputRecord.Event.KeyEvent.uChar.AsciiChar,
                  mInputRecord.Event.KeyEvent.dwControlKeyState,
                  mInputRecord.Event.KeyEvent.wVirtualKeyCode);
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void InputTest::doTestLoop3()
{
   char  tString[200];
   DWORD tNumRead = 0;
   int   tCount = 0;

   Prn::print(Prn::View01, "InputTest::doTestLoop3****************************");
   Prn::print(Prn::View11, "InputTest::doTestLoop3****************************");
   Prn::print(Prn::View21, "InputTest::doTestLoop3****************************");

   while (true)
   {
      // Read console input.
      ReadConsole(
         mInputHandle,
         tString,
         200,
         &tNumRead,
         NULL);
      tString[tNumRead - 2] = 0;

      Prn::print(Prn::View21, "ReadConsole %4d $ %4d %s",
         tNumRead, 
         strlen(tString),
         tString);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

#if 0
Prn::print(Prn::View01, "ReadConsoleInput BEGIN");
DWORD tNumRead = 0;
tRet = ReadConsoleInput(
   mInputHandle,      // input buffer handle 
   mInputBuffer,      // buffer to read into 
   128,               // size of read buffer 
   &tNumRead);        // number of records read 

Prn::print(Prn::View01, "ReadConsoleInput END %4d %4d", tRet, tNumRead);


//      fgets(tCommandLine, 200, stdin);

DWORD tNumRead = 0;
ReadConsole(
   mInputHandle,
   tString,
   200,
   &tNumRead,
   NULL);
tString[tNumRead] = 0;
if (tNumRead > 1) tString[tNumRead - 2] = 0;
Prn::print(Prn::View11, "String %4d %4d $ %s", tNumRead, tString);

#endif


