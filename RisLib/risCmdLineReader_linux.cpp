/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <readline/readline.h>
#include <readline/history.h>

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
   // Initialize readline history.
   using_history();
}

CmdLineReader::~CmdLineReader()
{
   // Clear readline history.
   clear_history();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a string of user console input. Copy it into the input argument
// string. Return a pointer to the string.

char* CmdLineReader::readLine(char* aString, int aMaxSize)
{
   // Read a line from console input.
   char* tString = readline("");

   // Guard.
   if (tString == 0) return 0;

   // Copy the line into the input argument.
   strncpy(aString, tString, aMaxSize - 1);

   // Add the line to the readline history.
   add_history(tString);

   // The line was allocated by the readline call. free it.
   free(tString);

   // Return the string.
   return aString;
}
   

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

