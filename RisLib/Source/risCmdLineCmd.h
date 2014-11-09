#ifndef _RIS_CMDLINECMD_H_
#define _RIS_CMDLINECMD_H_

#include "risPortableTypes.h"

/*==============================================================================

Command line command.

==============================================================================*/

//******************************************************************************
namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This encapsulates a single command line command.
// It is given a string of the form "command argument1 argumant2.." where
// the arguments can hold values for primitive data types such as int,bool,char*,
// double. An example command line command is the string "transmit true 101 1.1".
// The provides parsing of the command line and methods to extract the command
// and the arguments.

class  CmdLineCmd
{
public:

   CmdLineCmd();

   enum {MaxNumOfArgs=20};
   enum {MaxArgSize=100};

   char mDelimiters[10];

   //---------------------------------------------------------------------------
   // Parse a command line string into the command

   void    parseCmdLine  (char* aCommandLine);

   //---------------------------------------------------------------------------
   // Return arguments from the command line.
   // These are given the argument index, such that argInt(1) returns the
   // first argument as an integer and argInt(2) returns the second argument.

   int     argInt        (int aArgIndex);
   bool    argBool       (int aArgIndex);
   double  argDouble     (int aArgIndex);
   char*   argString     (int aArgIndex);

   // Tests argument aArgIndex against a string
   bool    isArgString   (int aArgIndex,char* aString);

   // Copies an argument into a string
   void    copyArgString (int aArgIndex,char* aString);

   //---------------------------------------------------------------------------
   // Set defaults for arguments from the command line.
   // Used if no arguments are entered.

   void setArgDefault(int argIndex, int    aValue);
   void setArgDefault(int argIndex, bool   aValue);
   void setArgDefault(int argIndex, double aValue);
   void setArgDefault(int argIndex, char*  aString);

   //---------------------------------------------------------------------------
   // Command extraction

   // Returns the number of arguments
   int     numArg   ();

   // Returns true if aString is equal to the command
   bool    isCmd    (char* aString);

   // Returns true if no calls to isCmd returned true
   bool    isBadCmd ();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Member variables

   // command line string argument pointer
   // mArgPtr[0] is the command, mArgPtr[1] is the first argument ...
   char*  mArgPtr   [MaxNumOfArgs+1];      
   // command line string argument storage
   // mArgPtr[n] points to mArgStore[n]
   char  mArgStore   [MaxNumOfArgs+1][MaxArgSize];      
   // true if corresponding argument exists
   bool   mArgFlag  [MaxNumOfArgs+1];  
   // number of arguments
   int    mArgNum;                         
   // if number of arguments is zero then use defaults
   bool   mDefaultEnable;                         
   // true if an isCmd returned true   
   bool   mGoodCmd;                        
   // true if struct is valid
   bool   mValidFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // command response string

   enum {ResponseStringSize=200};
   char  mResponseString[ResponseStringSize];
   bool  mResponseValid;

   void  resetResponse ();
   void  setResponse   (char* aString);
   char* getResponse   ();

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Command Line Executive base class.
// This is a base class for classes that process command line arguments
// Pointers to the base class are passed to command line executers (instances
// of CmdLineProc or CmdLineFile) to process input from a console command line
// or input from a command line file, which is a text file that contains a
// sequence of command lines.

class  BaseCmdLineExec
{
public:
   // Constructor
   BaseCmdLineExec();
   virtual void reset (){};

   // This is supplied by inheriting classes to execute single command lines.
   virtual void execute(CmdLineCmd* aCmd)=0;

   // This is supplied by inheriting classes to execute a command line 
   // that starts with "Begin". It is used to begin a new section with a 
   // new executive.
   virtual BaseCmdLineExec* executeForBegin(CmdLineCmd* aCmd) {return 0;}

   // This can be called to indicate to the executer to exit a sequence
   // of commands, as if "EXIT" was the command.
   void   exit();
   bool   mExitFlag;
};

//******************************************************************************
}//namespace
#endif

