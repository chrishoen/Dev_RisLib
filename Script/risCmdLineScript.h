#pragma once

/*==============================================================================
Command line script file processor.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <stdio.h>

#include "risCmdLineCmd.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class can be used to process script text files that contain a
// sequence of command lines. It provides a functions that reads each
// line in the file one at a time, parses the line into a command line
// command and returns it.

namespace Ris
{

class CmdLineScript
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members. 

   // File handle
   FILE*  mFile;

   // Current line number.
   int  mLineNumber;

   // File offset that a loop starts at.
   int mLoopStartOffset;

   // Loop count down to zero.
   int mLoopCountZero;
   
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. 

   // Constructor.
   CmdLineScript();
   virtual ~CmdLineScript(){}

   // Open the command line script file.
   bool doOpen(char* aFilename);

   // Close the command line script file.
   void doClose();

   // Read the next line from the file, parse it into a command line
   // command, and return it in the function argument. If end of file is
   // reached then return "EXIT". Return true if the line contained a
   // valid command, return false if it did not (for example: a commented
   // line).
   bool doRead(CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

