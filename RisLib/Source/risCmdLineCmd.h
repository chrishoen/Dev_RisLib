#ifndef _RIS_CMDLINECMD_H_
#define _RIS_CMDLINECMD_H_

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
// This provides parsing of the command line and methods to extract the command
// and the arguments.

class CmdLineExecNestedAnchor;

class  CmdLineCmd
{
public:

   //---------------------------------------------------------------------------
   // Constructor

   CmdLineCmd();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Access methods

   //---------------------------------------------------------------------------
   // Parse a command line string into the command object

   void    parseCmdLine  (char* aCommandLine);

   //---------------------------------------------------------------------------
   // Return arguments from the command line.
   // These are given the argument index, such that argInt(1) returns the
   // first argument as an integer and argInt(2) returns the second argument.

   int      argInt        (int aArgIndex);
   unsigned argUnsigned   (int aArgIndex);
   bool     argBool       (int aArgIndex);
   double   argDouble     (int aArgIndex);
   double   argRadian     (int aArgIndex);
   char*    argString     (int aArgIndex);

   // Tests argument aArgIndex against a string.
   // It compares the argument with the string.
   bool    isArgString   (int aArgIndex,const char* aString);

   // Copies an argument into a string
   void    copyArgString (int aArgIndex,char* aString,int aMaxSize);

   //---------------------------------------------------------------------------
   // Set defaults for arguments from the command line.
   // These are used as defaults if no arguments are entered.

   void setArgDefault         (int argIndex, int      aValue);
   void setArgDefaultUnsigned (int argIndex, unsigned aValue);
   void setArgDefault         (int argIndex, bool     aValue);
   void setArgDefault         (int argIndex, double   aValue);
   void setArgDefault         (int argIndex, char*    aString);

   //---------------------------------------------------------------------------
   // Command extraction

   // Returns the number of arguments
   int     numArg   ();

   // Returns true if aString is equal to the command
   bool    isCmd    (const char* aString);

   // Returns true if no calls to isCmd returned true
   bool    isBadCmd ();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Member variables

   enum {MaxNumOfArgs =  20};
   enum {MaxArgSize   = 100};

   // Argument delimiters
   char mDelimiters[10];

   // Command line string argument pointer
   // mArgPtr[0] is the command, mArgPtr[1] is the first argument ...
   char*  mArgPtr   [MaxNumOfArgs+1];
   // Command line string argument storage
   // mArgPtr[n] points to mArgStore[n]
   char   mArgStore [MaxNumOfArgs+1][MaxArgSize];
   // True if corresponding argument exists
   bool   mArgFlag  [MaxNumOfArgs+1];
   // Number of arguments
   int    mArgNum;
   // if number of arguments is zero then use defaults
   bool   mDefaultEnable;
   // True if an isCmd returned true   
   bool   mGoodCmd;
   // True if struct is valid
   bool   mValidFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Command response string. This can be used to store a response to a 
   // command.

   enum {ResponseStringSize=200};
   char  mResponseString[ResponseStringSize];
   bool  mResponseValid;

   void  resetResponse ();
   void  setResponse   (char* aString);
   char* getResponse   ();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Nested anchor, used for processing nested records. This is piggybacked 
   // onto a command by a command line processor so that it can be used by a 
   // command line executive.

   CmdLineExecNestedAnchor* mNestedAnchor;
};

//******************************************************************************
}//namespace
#endif

