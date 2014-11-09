#ifndef _RISCMDLINEFILE_H_
#define _RISCMDLINEFILE_H_
/*==============================================================================

Executer for text files that contain a sequence of command lines.

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <stdio.h>

#include "risPortableTypes.h"
#include "risCmdLineCmd.h"

//******************************************************************************
// This class is a command line executer for text files that contain a sequence
// of command lines. It opens a command line file and applies an executive
// to process all of the commands in the file, one at a time, as if they were
// commands typed into a console command line interface. If an "EXIT" command
// is encountered then no further commands are processed.

namespace Ris
{

class  CmdLineFile
{
public:

   // Open the command line file
   bool open      (char* aFilename);

   // Apply a command line executive to process all of the command lines
   // in the file
   void execute   (BaseCmdLineExec* aExec);

   // Close the command line file
   void close     ();

   // Members
   FILE*  mFile;
   int  mLineNumber;
};

//******************************************************************************
}//namespace
#endif

