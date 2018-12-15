/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <conio.h>
#include <ctype.h>

#include "prnPrint.h"

#define  _CONSTRINGREADER_CPP_
#include "conInputReader_win.h"

namespace Con
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

InputReader::InputReader()
{
   reset();
}

void InputReader::reset()
{
}

void InputReader::initialize()
{
// Prn::print(Prn::View01, "InputReader::initialize*****************************");
// Prn::print(Prn::View11, "InputReader::initialize*****************************");
   reset();
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

   HANDLE tInputHandle = GetStdHandle(STD_INPUT_HANDLE);
   HANDLE tOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   DWORD tNumRead = 0;
   DWORD tNumWritten = 0;

   //************************************************************************
   //************************************************************************
   //************************************************************************
   // Read the first key.

   // Flush the input and loop to read console input key records for the
   // first key input. Store the key records in the input buffer. If the 
   // first key is a control then write substitutions to the argument input 
   // string and and return. If the first key is not a control then exit
   // the loop. In this case, the input records will be written back to the
   // console input buffer and the argument input string will be read 
   // from buffered console input so that editing can occur.

   mInputCount = 0;
   FlushConsoleInputBuffer(tInputHandle);
   while (true)
   {
      // Read a console input record.
      ReadConsoleInput(
         tInputHandle,
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

         mInputBuffer[mInputCount] = mInputRecord;
         if (++mInputCount == cMaxBufferSize) mInputCount = cMaxBufferSize - 1;

         //******************************************************************
         //******************************************************************
         //******************************************************************
         // Extract local state variables from the key record.

         int tChar   = mInputRecord.Event.KeyEvent.uChar.AsciiChar;
         int tState  = mInputRecord.Event.KeyEvent.dwControlKeyState;
         int tCode   = mInputRecord.Event.KeyEvent.wVirtualKeyCode;

         bool tDown  = (mInputRecord.Event.KeyEvent.bKeyDown);
         bool tShift = (tState & SHIFT_PRESSED);
         bool tCntl  = (tState & LEFT_CTRL_PRESSED) || (tState & RIGHT_CTRL_PRESSED);
         bool tAlt   = (tState & LEFT_ALT_PRESSED) || (tState & RIGHT_ALT_PRESSED);

         //******************************************************************
         //******************************************************************
         //******************************************************************
         // If the input record is for a printable character then exit the
         // loop.

         if (tDown && isprint(tChar) && !tAlt)
         {
            break;
         }

         //******************************************************************
         //******************************************************************
         //******************************************************************
         // If the input record is for a printable character with an alt 
         // then write substitutions to the argument input string and
         // return.

         if (tDown && isprint(tChar) && tAlt && !tShift)
         {
            sprintf(aInputString, "alt_%c", tChar);
            return;
         }

         if (tDown && isprint(tChar) && tAlt && tShift)
         {
            sprintf(aInputString, "alt_shift_%c", tChar - ('A' - 'a'));
            return;
         }

         //******************************************************************
         //******************************************************************
         //******************************************************************
         // If the input record is an up or down arrow then exit the loop.

         if (tDown && ((tCode == VK_UP) || (tCode == VK_DOWN)))
         {
            break;
         }

         //******************************************************************
         //******************************************************************
         //******************************************************************
         // If the input record is a control character then write 
         // substitutions to the argument input string and return.

         if (tDown && (tChar != 0) && iscntrl(tChar))
         {
            if (tChar == 27)
            {
               strcpy(aInputString, "escape");
               return;
            }
            else if (tChar == 13)
            {
               if (tShift)
               {
                  strcpy(aInputString, "shift_enter");
                  return;
               }
               else if (tCntl)
               {
                  strcpy(aInputString, "cntl_enter");
                  return;
               }
               else if (tAlt)
               {
                  strcpy(aInputString, "alt_enter");
                  return;
               }
               else
               {
                  strcpy(aInputString, "enter");
                  WriteConsole(tOutputHandle, "\n", 1, &tNumWritten, 0);
                  return;
               }
            }
            else
            {
               if (tShift)
               {
                  sprintf(aInputString, "cntl_shift_%c", 96 + tChar);
                  return;
               }
               else
               {
                  sprintf(aInputString, "cntl_%c", 96 + tChar);
                  return;
               }
            }
         }

         //******************************************************************
         //******************************************************************
         //******************************************************************
         // If the input record is a function key then write 
         // substitutions to the argument input string and return.

         if (tDown && (VK_F1 <= tCode && tCode <= VK_F12))
         {
            int tFunctionNum = tCode - VK_F1 + 1;

            if (!tCntl && !tAlt && !tShift)  sprintf(aInputString, "fn_%d", tFunctionNum);
            if (!tCntl && !tAlt &&  tShift)  sprintf(aInputString, "shift_fn_%d", tFunctionNum);
            if (!tCntl &&  tAlt && !tShift)  sprintf(aInputString, "alt_fn_%d", tFunctionNum);
            if (!tCntl &&  tAlt &&  tShift)  sprintf(aInputString, "alt_shift_fn_%d", tFunctionNum);
            if ( tCntl && !tAlt && !tShift)  sprintf(aInputString, "cntl_fn_%d", tFunctionNum);
            if ( tCntl && !tAlt &&  tShift)  sprintf(aInputString, "cntl_shift_fn_%d", tFunctionNum);
            if ( tCntl &&  tAlt && !tShift)  sprintf(aInputString, "cntl_alt_fn_%d", tFunctionNum);
            if ( tCntl &&  tAlt &&  tShift)  sprintf(aInputString, "cntl_alt_shift_fn_%d", tFunctionNum);

            return;
         }
      }
   }

   //************************************************************************
   //************************************************************************
   //************************************************************************
   // If the first key is not a control then read the input string into
   // the argument input string.

   // Write the input buffer from the first key back to the console input
   // buffer.
   WriteConsoleInput(
      tInputHandle,
      mInputBuffer,
      mInputCount,
      &tNumWritten);

   // Read from the console input into the argument input string. This is
   // in buffered  mode and will allow string editing.
   ReadConsole(
      tInputHandle,
      aInputString,
      cMaxStringSize,
      &tNumRead,
      NULL);

   // Ignore the end of string.
   aInputString[tNumRead] = 0;
   if (tNumRead > 1) aInputString[tNumRead - 2] = 0;

   // Done.
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Wait for a press enter at the console input.

void InputReader::doWaitForPressEnter()
{
   char tString[10];
   DWORD tNumRead = 0;
   HANDLE tInputHandle = GetStdHandle(STD_INPUT_HANDLE);

   ReadConsole(
      tInputHandle,
      tString,
      10,
      &tNumRead,
      NULL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run a test loop.

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

