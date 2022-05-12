/*==============================================================================
==============================================================================*/

#include "stdafx.h"

#include "my_functions.h"
#include "risContainers.h"

#include "risCmdLineScript.h"


namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

CmdLineScript::CmdLineScript()
{
   mFile = 0;
   mLineNumber = 0;
}
  
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Open the command line script file.

bool CmdLineScript::doOpen(char* aFilename)
{            
   mFile = fopen(aFilename,"r");

   if (mFile==0) return false;

   mLineNumber=0;

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the file.

void CmdLineScript::doClose()
{       
   // Close the file
   if (mFile) fclose(mFile);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read the next line from the file, parse it into a command line
// command, and return it in the function argument. If end of file is
// reached then return "EXIT". Return true if the line contained a
// valid command, return false if it did not (for example: a commented
// line).

bool CmdLineScript::doRead(CmdLineCmd* aCmd)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Guard.
   aCmd->reset();
   if (mFile==0) return false;

   // Command line variables.
   char tCommandLine[200];
   int  tCommandLineLen=0;
   bool tCommentFlag=false;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read a command line from the file. If it contains a valid
   // command then parse it into the command line command.
            
   // Read command line from the file.
   if (fgets(tCommandLine, 200, mFile) == 0)
   {
      // If this is eof then done.
      aCmd->parseCmdLine("EXIT");
      return true;
   }
   mLineNumber++;

   // Test for comment line.
   tCommentFlag=false;
   if (tCommandLine[0]=='#') tCommentFlag=true;
   if (tCommandLine[0]=='/') tCommentFlag=true;

   // Remove cr/lf at end of line.
   my_trimCRLF(tCommandLine);

   // Command line length.
   tCommandLineLen=(int)strlen(tCommandLine);

   // If the command line is not empty and not a comment then process
   // it, else continue the loop and go on to the next line.
   if(tCommandLineLen>2 && !tCommentFlag)
   {
      // Parse the command line into the command object.
      aCmd->parseCmdLine(tCommandLine);
      return true;
   }
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

