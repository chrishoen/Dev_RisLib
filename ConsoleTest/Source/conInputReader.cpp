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

//  gInputThread = new InputThread;
//  gInputThread->launchThread();
}

void InputReader::finalize()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Run test loop.

void InputReader::doTestLoop1()
{
   DWORD tRet = 0;

   char  tString[200];
   DWORD tNumRead = 0;
   DWORD tNumWritten = 0;
   int   tCount = 0;

   Prn::print(Prn::View01, "InputReader::doTestLoop1****************************");
   Prn::print(Prn::View11, "InputReader::doTestLoop1****************************");
   Prn::print(Prn::View21, "InputReader::doTestLoop1****************************");

   while (true)
   {
      mInputCount = 0;
      Prn::print(Prn::View11, "ReadConsoleInput****************************");

      FlushConsoleInputBuffer(mInputHandle);
      while (true)
      {
         ReadConsoleInput(
            mInputHandle,
            &mInputRecord,
            1,
            &tNumRead);

         if (mInputRecord.EventType == KEY_EVENT)
         {
            mInputBuffer[mInputCount++] = mInputRecord;

            Prn::print(Prn::View11, "ReadConsoleInput  %4d $ %4d %4d $ %4X %4X",
               mInputCount,
               mInputRecord.Event.KeyEvent.bKeyDown,
               mInputRecord.Event.KeyEvent.uChar.AsciiChar,
               mInputRecord.Event.KeyEvent.dwControlKeyState,
               mInputRecord.Event.KeyEvent.wVirtualKeyCode);

            if (mInputRecord.Event.KeyEvent.bKeyDown == 0) break;
         }
      }

      FlushConsoleInputBuffer(mInputHandle);
      WriteConsoleInput(
         mInputHandle,
         mInputBuffer,
         mInputCount,
         &tNumWritten);
      Prn::print(Prn::View11, "WriteConsoleInput %4d", tNumWritten);

      // Read console input.
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

void InputReader::doTestLoop2()
{
   DWORD tNumRead = 0;
   int   tCount = 0;

   Prn::print(Prn::View01, "InputReader::doTestLoop2****************************");
   Prn::print(Prn::View11, "InputReader::doTestLoop2****************************");
   Prn::print(Prn::View21, "InputReader::doTestLoop2****************************");

   while (true)
   {
      ReadConsoleInput(
         mInputHandle,
         &mInputRecord,
         1,
         &tNumRead);
      tCount++;

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

void InputReader::doTestLoop3()
{
   char  tString[200];
   DWORD tNumRead = 0;
   int   tCount = 0;

   Prn::print(Prn::View01, "InputReader::doTestLoop3****************************");
   Prn::print(Prn::View11, "InputReader::doTestLoop3****************************");
   Prn::print(Prn::View21, "InputReader::doTestLoop3****************************");

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


