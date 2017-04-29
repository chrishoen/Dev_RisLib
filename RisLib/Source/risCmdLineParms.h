#ifndef _RISCMDLINEPARMS_H_
#define _RISCMDLINEPARMS_H_
/*==============================================================================

Base class for parameters classes whose values are read from a command file. 

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCmdLineFile.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a base class for classes that contains parameter member variables.
// The values of the parameters are set by reading a text file that contains 
// command lines. Each command line is of the form "command argument1
// argument2 ...".
// 
// The command files are partitioned into different sections and only one
// section can be read at a time to set member variables that are specified
// in it.
//
// The command files are managed by a CmdLineFile object. This opens the 
// file, reads each line in it, parses the line into a CmdLineCmd command 
// object, passes the command object to an inheritoring object for command
// execution, and then closes the file. 
//
// This class inherits from BaseCmdLineExec. An inheriting class must provide
// an overload for the execute(cmd) method that is called by the CmdLineFile
// object for each command in the file. It then sets a member variable,
// according to the command.
// 
// Inheriting classes can contain member variables that also inherit from
// BaseCmdLineExec. This provides for command files that have a nested
// structure. If so, then the inheriting class is the root.
// 

class BaseCmdLineParms : public Ris::BaseCmdLineExec
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // File name max size.
   static const int cMaxStringSize = 400;

   // File name for files that contain default paramters.
   char mDefaultFileName[cMaxStringSize];

   // File name for files that contain override paramters.
   char mOverrideFileName[cMaxStringSize];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // If true then use sections.
   bool mUseSections;

   // The target section, the section to be read from.
   char mTargetSection[200];

   // True if the current command is in the target section.
   bool mTargetSectionFlag;

   // Return true if the command is in the target section.
   bool isTargetSection(Ris::CmdLineCmd* aCmd);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infrastucture.

   // Constructor,
   typedef Ris::BaseCmdLineExec BaseClass;
   BaseCmdLineParms();
   void reset();

   // Base class override: Execute a command from the command file to set a 
   // member variable. This is called by the associated command file object
   // for each command in the file.
   void execute(Ris::CmdLineCmd* aCmd)=0;

   // Calculate expanded member variables. This is called after the entire
   // command file has been processed.
   virtual void expand() {}

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Read a section of the command file and set member variables accordingly.
   // Create a command file object, open the file, pass this object to the file
   // object to read the file and apply this object's execution method to each
   // command in the file, and then close the file. This only reads variables
   // for a specific section in the file. If the input section is null or 
   // empty then section logic is ignored and the entire file is read.
   bool readSection(char* aFilepath,char* aSection);
   
   // This is the same as above, but with a null filepath. The filepath is
   // searched amoung default filepaths.
   bool readSection(char* aSection);

   // This is the same as the above, but with a null filepth and a null
   // section. The filepath is searched amoung default filepaths.    
   bool readFile();

};

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace
#endif

