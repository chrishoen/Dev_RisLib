//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "prnPrint.h"
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
   strcpy(mDefaultFileName,"YOU_FORGOT_THE_DEFAULT_FILENAME");
   strcpy(mOverrideFileName,"YOU_FORGOT_THE_OVERRIDE_FILENAME");
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

bool BaseCmdLineParms::readSection(char* aFilePath, char* aSection)
{
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Section variables.   

   // If the input section is invalid then don't use sections.
   if (aSection == 0)
   {
      mUseSections = false;
   }
   else if (strlen(aSection) == 0)
   {
      mUseSections = false;
   }

   // Store arguments.
   if (mUseSections)
   {
      strcpy(mTargetSection, aSection);
   }
   else
   {
      strcpy(mTargetSection, "NO_SECTIONS");
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Open file.

   // Command line executive file object.   
   Ris::CmdLineFile tCmdLineFile;

   if (!tCmdLineFile.open(aFilePath))
   {
      printf("BaseCmdLineParms::file open FAIL %s\n", aFilePath);
      return false;
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Execute the commands in the file.

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
// This is the same as above, but with a null filepath. The filepath is
// searched amoung default filepaths.

bool BaseCmdLineParms::readSection(char* aSection)
{
   char tFilePath[200];

   // Filepath found.
   bool tFileFound = false;

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetProgramDir());
   strcat(tFilePath, mDefaultFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetCurrentDir());
   strcat(tFilePath, mDefaultFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetCurrentDir());
   strcat(tFilePath, "..\\Files\\");
   strcat(tFilePath, mDefaultFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetProgramDir());
   strcat(tFilePath, "..\\..\\Files\\");
   strcat(tFilePath, mDefaultFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // File not found.

   printf("BaseCmdLineParms::file open FAIL %s\n", tFilePath);
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the same as above, but it uses the override file name, instead
// of the default file name.

bool BaseCmdLineParms::readOverride(char* aSection)
{
   char tFilePath[200];

   // Filepath found.
   bool tFileFound = false;

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetCurrentDir());
   strcat(tFilePath, mOverrideFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetCurrentDir());
   strcat(tFilePath, "..\\Files\\");
   strcat(tFilePath, mOverrideFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Search for file. If found then read it.

   strcpy(tFilePath, Ris::portableGetProgramDir());
   strcat(tFilePath, "..\\..\\Files\\");
   strcat(tFilePath, mOverrideFileName);

   if (portableFilePathExists(tFilePath))
   {
      return readSection(tFilePath,aSection);
   }

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // File not found.

   printf("BaseCmdLineParms::file open FAIL %s\n", tFilePath);
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the same as the above, but with a null filepth and a null
// section. The filepath is searched amoung default filepaths.    

bool BaseCmdLineParms::readFile()
{
   // Read with a null section, this ignores sections and processes the entire
   // file.
   return readSection(0);
}


}//namespace


