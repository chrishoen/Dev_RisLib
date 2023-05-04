#pragma once

/*==============================================================================
Command line script file processor.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <stdio.h>
#include <string>
#include <memory>

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

   // Open the command line script file. Return true if successful.
   bool doOpen(const char* aFilePath);

   // Open the command line script file. Return true if successful.
   bool doOpen(std::unique_ptr<std::string> aFilePath);

   // Close the command line script file.
   void doClose();

   // Read the next line from the file, parse it into a command line
   // command, and return it in the function argument. If end of file
   // is reached then return "EXIT". If the command is "LOOP n" then 
   // a loop will be started. If the command is "LOOPEND" then the next
   // read will be at the next command after the "LOOP n" command. This
   // will be repeated n times.
   // 
   // Return true if the line contained a valid command, return false
   // if it did not (for example: a commented line, an exit, a loop, or
   // an end loop).
   bool doRead(CmdLineCmd* aCmd);

   // If currently in a loop, then exit it.
   void doExitLoop();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

