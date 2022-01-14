/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"

#define  _RISCMDLINEREADER_CPP_
#include "risCmdLineReader.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

CmdLineReader::CmdLineReader()
{
}

CmdLineReader::~CmdLineReader()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a string of user console input. Copy it into the input argument
// string. Return a pointer to the string.

char* CmdLineReader::readLine(char* aString, int aMaxSize)
{
   // Read from console input.
   char* tString = fgets(aString, aMaxSize, stdin);

   // Guard.
   if (tString == 0) return 0;

   // Remove cr/lf at end of line.
   my_trimCRLF(tString);

   // Return the string.
   return tString;
}
   

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
