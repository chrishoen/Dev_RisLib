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
   mLoopStartOffset = 0;
   mLoopCountZero = 0;
}
  
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Open.

// Open the command line script file. Return true if successful.
bool CmdLineScript::doOpen(const char* aFilePath)
{            
   mFile = fopen(aFilePath,"r");

   if (mFile==0) return false;

   mLineNumber=0;
   mLoopStartOffset = 0;
   mLoopCountZero = 0;

   return true;
}

// Open the command line script file. Return true if successful.
bool CmdLineScript::doOpen(std::unique_ptr<std::string> aFilePath)
{
   bool tPass = doOpen(aFilePath->c_str());
   return tPass;
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
// command, and return it in the function argument. If end of file
// is reached then return "EXIT". If the command is "LOOP n" then 
// a loop will be started. If the command is "LOOPEND" then the next
// read will be at the next command after the "LOOP n" command. This
// will be repeated n times.
// 
// Return true if the line contained a valid command, return false
// if it did not (for example: a commented line, an exit, a loop, or
// an end loop).

bool CmdLineScript::doRead(CmdLineCmd* aCmd)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Guard.
   aCmd->reset();
   aCmd->parseCmdLine("EXIT");
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

   // If the command line is empty or a comment then ignore it.
   if (tCommandLineLen <= 2 || tCommentFlag) return false;

   // Parse the command line into the command object.
   aCmd->parseCmdLine(tCommandLine);

   // Test for a loop command
   if (aCmd->isCmd("LOOP"))
   {
      // Save the current file position, which would point to the start of 
      // the next instruction in the file. This will be used at the end of
      // the loop to restart at the first instruction that follows this one.
      mLoopStartOffset = ftell(mFile);
      mLoopCountZero = aCmd->argInt(1);
      if (mLoopCountZero == 0) mLoopCountZero = 1000000000;
      return false;
   }

   // Test for a loop end command
   if (aCmd->isCmd("ENDLOOP"))
   {
      // Test if still in the loop.
      if (mLoopCountZero > 0)
      {
         // Test if still in the loop.
         if (--mLoopCountZero == 0)
         {
            // End of loop, continue.
         }
         else
         {
            // Still in the loop, set the current file position to the
            // first instruction in the loop. The next read will then 
            // read that instruction.
            fseek(mFile, mLoopStartOffset, SEEK_SET);
         }
      }
      return false;
   }
   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// If currently in a loop, then exit it.

void CmdLineScript::doExitLoop()
{
   mLoopCountZero = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

