/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "prnPrint.h"
#include "risContainers.h"

#include "risCmdLineFile.h"


namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

bool CmdLineFile::open(char* aFilename)
{            
   // Open the file
   mFile = fopen(aFilename,"r");

   if (mFile==0)
   {
      return false;
   }

   mLineNumber=0;

   return true;
}

//******************************************************************************

void CmdLineFile::close()
{       
   // Close the file
   if (mFile)
   {
      fclose(mFile);
   }
}

//******************************************************************************

void CmdLineFile::execute(BaseCmdLineExec* aExec)
{
   //---------------------------------------------------------------------------
   // Guard
            
   if (mFile==0)
   {
      return;
   }

   //---------------------------------------------------------------------------
   // Locals
            
   CmdLineCmd       tCmd;
   BaseCmdLineExec* tExec      = aExec;

   Ris::Containers::Stack<BaseCmdLineExec*,10> tExecStack;

   char tCommandLine[200];
   int  tCommandLineLen=0;
   bool tCommentFlag=false;

   //---------------------------------------------------------------------------
   // Loop to read each command line in the file
            
   bool tGoing=true;
   while(tGoing)
   {
      // Read command line
      if (fgets(tCommandLine,200,mFile)==0) break;
      mLineNumber++;

      // Test for comment line    
      tCommentFlag=false;
      if (tCommandLine[0]=='#') tCommentFlag=true;
      if (tCommandLine[0]=='/') tCommentFlag=true;

      // Command line length
      tCommandLineLen=(int)strlen(tCommandLine);

      // Remove cr/lf at end of line
      if (tCommandLine[tCommandLineLen-1]==0xa)
      {
         tCommandLine[tCommandLineLen-1]=0;

         if (tCommandLine[tCommandLineLen-2]==0xd)
         {
            tCommandLine[tCommandLineLen-2]=0;
         }
      }

      // If the command line is not empty and not a comment
      // then process it, else go on to the next line
      if(tCommandLineLen>2 && !tCommentFlag)
      {
         // Put command line string to command line command
         // for parsing
         tCmd.parseCmdLine(tCommandLine);
   
         // Process the parsed command line
         if(tCmd.isCmd("EXIT"))
         {
            // If exit then exit the loop
            tGoing=false; 
         }
         else if(tCmd.isCmd("BEGIN"))
         {
            // Call executive to process the command
            tExecStack.push(tExec);
            tExec = tExec->executeForBegin(&tCmd);
            if (tExec==0) tGoing=false;
         }
         else if(tCmd.isCmd("END"))
         {
            tExecStack.pop(tExec);
         }
         else
         {
            // Call executive to process the command
            tExec->execute(&tCmd);

            if(tExec->mExitFlag)
            {
               // If exit then exit the loop
               tGoing=false; 
            }
         }
      }
   }
}

}//namespace

