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
   bool tFound = false;
   int tCount = 0;
   Prn::print(Prn::View01, "InputReader::doTestLoop1****************************");
   Prn::print(Prn::View11, "InputReader::doTestLoop1****************************");

   while (true)
   {
      // Read console input.
      char  tCommandLine[200];

      int tChar = _getch();
      ungetch(tChar);
      Prn::print(Prn::View11, "Char   %4d", tChar);
//      fgets(tCommandLine, 200, stdin);
      DWORD tNumRead = 0;
      tRet = ReadConsoleInput(
         mInputHandle,      // input buffer handle 
         mInputBuffer,      // buffer to read into 
         128,               // size of read buffer 
         &tNumRead);        // number of records read 
      Prn::print(Prn::View11, "String %s", tCommandLine);
   }
};

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




DWORD tNumRead = 0;
ReadConsole(
   mInputHandle,
   tCommandLine,
   200,
   &tNumRead,
   NULL);
#endif


