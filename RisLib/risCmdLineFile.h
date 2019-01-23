#pragma once

/*==============================================================================
Command line file processor

This processes text files that contain a sequence of command lines.
It enters a loop that reads each line in the file and applies a given command
line executive to it.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <stdio.h>

#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is a command line processor for text files that contain a sequence
// of command lines. It opens a command line file and applies an executive
// to process all of the commands in the file, one at a time, as if they were
// commands typed into a console command line interface. If an "EXIT" command
// is encountered then no further commands are processed.

namespace Ris
{

class CmdLineFile
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

   // Nested anchor. This is used to manage files that contain nested records,
   // which are processed by nested executives.
   CmdLineExecNestedAnchor mNestedAnchor;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. 

   // Open the command line file.
   bool open (char* aFilename);

   // Apply a command line executive to each of the command lines in the file.
   void execute (BaseCmdLineExec* aExec);

   // Close the command line file/
   void close ();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

