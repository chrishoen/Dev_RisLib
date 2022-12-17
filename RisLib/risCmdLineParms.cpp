//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risBaseDir.h"
#include "risAlphaDir.h"
#include "risPortableCalls.h"
#include "risCmdLineFile.h"

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
   mFilePath[0] = 0;
   reset();
}

void BaseCmdLineParms::reset()
{
   mUseSections = true;
   mTargetSection[0] = 0;
   mTargetSectionSave[0] = 0;
   mTargetSectionFlag = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// File path.

void BaseCmdLineParms::setFilePath(const char* aFilePath)
{
   strncpy(mFilePath, aFilePath,cMaxStringSize - 1);
}

void BaseCmdLineParms::setFilePath(std::unique_ptr<std::string> aFilePath)
{
   strncpy(mFilePath, aFilePath->c_str(), cMaxStringSize - 1);
}

void BaseCmdLineParms::setFilePath_RelativeToBaseDir(const char* aRelativeFilePath)
{
   char tBuffer[cMaxStringSize];
   strncpy(mFilePath, Ris::getBaseFullFilePath(tBuffer, aRelativeFilePath), cMaxStringSize - 1);
}

void BaseCmdLineParms::setFileName_RelAlphaFiles(const char* aFileName)
{
   char tBuffer[cMaxStringSize];
   strncpy(mFilePath, Ris::getAlphaFilePath_Files(tBuffer, aFileName), cMaxStringSize - 1);
}
void BaseCmdLineParms::setFileName_RelAlphaSettings(const char* aFileName)
{
   char tBuffer[cMaxStringSize];
   strncpy(mFilePath, Ris::getAlphaFilePath_Settings(tBuffer, aFileName), cMaxStringSize - 1);
}
bool BaseCmdLineParms::fileExists()
{
   return portableFilePathExists(mFilePath);
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
// Alias for readSection("default");

bool BaseCmdLineParms::readFile()
{
   return readSection("default");
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

bool BaseCmdLineParms::readSection(const char* aSection)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open file.

   // Command line executive file object.   
   Ris::CmdLineFile tCmdLineFile;

   // Open the file.
   if (!tCmdLineFile.open(mFilePath))
   {
      // Exit the program if the open failed.
      printf("BaseCmdLineParms::file open FAIL001 %s\n", mFilePath);
      Ris::portableHalt();
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
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
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read a section of the file and apply a separate executive
// to it.

class CmdLineExecProxy : public Ris::BaseCmdLineExec
{
public:
   bool mTargetSectionFlag;
   char* mTargetSection;
   Ris::BaseCmdLineExec* mExec;
 
   CmdLineExecProxy(char* aTargetSection, Ris::BaseCmdLineExec* aExec)
   {
      mTargetSectionFlag = false;
      mTargetSection = aTargetSection;
      mExec = aExec;
   }

   bool isTargetSection(Ris::CmdLineCmd* aCmd)
   {
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

   void execute(Ris::CmdLineCmd* aCmd)
   {
      if (!isTargetSection(aCmd)) return;
      mExec->execute(aCmd);
   }
};

bool BaseCmdLineParms::readSection(const char* aSection, Ris::BaseCmdLineExec* aExec)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Section variables.   

   // Save the target section and then set it to the new section.
   strcpy(mTargetSectionSave, mTargetSection);
   strcpy(mTargetSection, aSection);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open file.

   // Command line executive file object.   
   Ris::CmdLineFile tCmdLineFile;

   // Open the file.
   if (!tCmdLineFile.open(mFilePath))
   {
      // Exit the program if the open failed.
      printf("BaseCmdLineParms::file open FAIL201 %s\n", mFilePath);
      Ris::portableHalt();
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Execute the commands in the file.

   // Pass this object to the file object to read the file and apply this
   // object's execution method to each command in the file.
   CmdLineExecProxy tExecProxy(&mTargetSection[0], aExec);
   tCmdLineFile.execute(&tExecProxy);

   // Close the file.
   tCmdLineFile.close();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   // Restore the target section.
   strcpy(mTargetSection, mTargetSectionSave);
   // Done.
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


