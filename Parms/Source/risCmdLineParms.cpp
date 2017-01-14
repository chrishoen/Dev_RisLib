//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "my_functions.h"
#include "risCmdLineFile.h"
#include "risPortableCalls.h"

#define  _RISCMDLINEPARMS_CPP_
#include "risCmdLineParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

BaseCmdLineParms::BaseCmdLineParms()
{
   reset();
}

void BaseCmdLineParms::reset()
{
   // File member variables.
   mUseSections = true;
   mTargetSection[0] = 0;
   mTargetSectionFlag = false;
   strcpy(mFileName,"YOU_FORGOT_THE_FILENAME");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return true if the command is in the target section.

bool BaseCmdLineParms::isTargetSection(Ris::CmdLineCmd* aCmd)
{
   // If not using sections then always in target section.
   if (!mUseSections) return true;

   // If the command is the beginning of a section. 
   if (aCmd->isCmd("SectionBegin"))
   {
      mTargetSectionFlag = false;

      if (aCmd->numArg() == 1)
      {
         // And the section is the target section then the 
         // command is in the target section.
         if (aCmd->isArgString(1, mTargetSection))
         {
            mTargetSectionFlag = true;
         }
      }
   }

   // If the command is at the end of any section
   if (aCmd->isCmd("SectionEnd"))
   {
      // Then the command is not in the target section.
      mTargetSectionFlag = false;
   }

   return mTargetSectionFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a section of the command file and set member variables accordingly.
// Create a command file object, open the file, pass this object to the file
// object to read the file and apply this object's execution method to each
// command in the file, and then close the file. This only reads variables for
// a specific section in the file.

bool BaseCmdLineParms::readSection(char* aSection)
{
   // Store arguments.
   strcpy(mTargetSection, aSection);

   // File path.
   char tFilePath[200];

   strcpy(tFilePath, Ris::portableGetCurrentWorkingDir());
   strcat(tFilePath, "..\\..\\Files\\");
   strcat(tFilePath, mFileName);

   // Temporary command line file object.   
   Ris::CmdLineFile tCmdLineFile;

   // Open the file.
   if (tCmdLineFile.open(tFilePath))
   {
      // Pass this object to the file object to read the file and apply this
      // object's execution method to each command in the file.
      tCmdLineFile.execute(this);

      // Close the file.
      tCmdLineFile.close();

      // Expand extra member variables.
      expand();

      //printf("BaseCmdLineParms::file open PASS %s\n", tFilePath);
      return true;
   }
   else
   {
      printf("BaseCmdLineParms::file open FAIL %s\n", tFilePath);
      return false;
   }
}

}//namespace


