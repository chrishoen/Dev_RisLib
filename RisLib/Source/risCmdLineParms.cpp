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
   mTargetSectionFlag = true;
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
// command in the file, and then close the file. This only reads variables
// for a specific section in the file. If the input section is null or 
// empty then section logic is ignored and the entire file is read.

bool BaseCmdLineParms::readSection(char* aSection)
{
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Initial.   

   // If the input section is valid then use sections.
   mUseSections = (aSection!=0) && (strlen(aSection)!=0);

   // Store arguments.
   if (mUseSections)
   {
      strcpy(mTargetSection, aSection);
   }
   else
   {
      strcpy(mTargetSection, "NO_SECTIONS");
   }


   // Command line executive file object.   
   Ris::CmdLineFile tCmdLineFile;

   // Filepath.
   char tFilePath[200];

   // Filepath found.
   bool tFileFound = false;

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Open file.

   // Try open file.
   strcpy(tFilePath, Ris::portableGetCurrentWorkingDir());
   strcat(tFilePath, mFileName);

   if (tCmdLineFile.open(tFilePath))
   {
      // File found.
      tFileFound = true;
   }

   // If file not found then try open another file.
   if (!tFileFound)
   {
      strcpy(tFilePath, Ris::portableGetCurrentWorkingDir());
      strcat(tFilePath, "..\\..\\Files\\");
      strcat(tFilePath, mFileName);

      if (tCmdLineFile.open(tFilePath))
      {
         // File found.
         tFileFound = true;
      }
   }

   // If file not found then try open another file.
   if (!tFileFound)
   {
      strcpy(tFilePath, Ris::portableGetSettingsDir());
      strcat(tFilePath, mFileName);

      if (tCmdLineFile.open(tFilePath))
      {
         // File found.
         tFileFound = true;
      }
   }

   // Exit if file not found.
   if (!tFileFound)
   {
      printf("BaseCmdLineParms::file open FAIL %s\n", tFilePath);
      return false;
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Execute commands in file.

   // Pass this object to the file object to read the file and apply this
   // object's execution method to each command in the file.
   tCmdLineFile.execute(this);

   // Close the file.
   tCmdLineFile.close();

   // Expand extra member variables.
   expand();

   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the same as the above, but with an null section.    
// Read the entire command file and set member variables accordingly.
// Create a command file object, open the file, pass this object to the file
// object to read the file and apply this object's execution method to each
// command in the file, and then close the file.

bool BaseCmdLineParms::readFile()
{
   // Read with a null section, this ignores sections and processes the entire
   // file.
   return readSection(0);
}


}//namespace


