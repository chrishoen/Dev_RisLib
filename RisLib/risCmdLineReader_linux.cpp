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
// Read a line of user console input. Copy it into the input argument.
// Return a pointer to the line.

char* CmdLineReader::readLine(char* aCmdLine, int aMaxSize)
{
   // Read a line from console input.
   char* tLine = readline("");

   // Guard.
   if (tLine == 0) return 0;

   // Copy the line into the input argument.
   strncpy(aCmdLine, tLine, aMaxSize - 1);

   // Add the line to the readline history.
   add_history(tLine);

   // The line was allocated by the readline call. free it.
   free(tLine);

   // Return the line.
   return tLine;
}
   

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

