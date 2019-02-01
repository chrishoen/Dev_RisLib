#pragma once
/*==============================================================================

Table classes that can be used by the command line file classes.
One or two dimensional tables classes of int,double,string.
One dimensional talble of string.

==============================================================================*/

#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Table, 1 dimensional, indexed by 0..R-1
  
class CmdLineIntTable1D : public Ris::BaseCmdLineExec
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Vector components.
   int* mValues;
   int     mRows;

   // True if initialized.
   bool mInitialized;
   // True if this is the first command line.
   bool mFirstCmd;
   // Number of ints allocated in mValues.
   int  mAlloc;
   // Current index into mValues.
   int  mIndex;
   // True if all values were read in correctly.
   bool mValidFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Intrastructure.

   // Constructor.
   CmdLineIntTable1D();
  ~CmdLineIntTable1D();
   void reset();

   // Initialize and allocate memory for the table.
   void initialize(int aRows);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Access components.
   int& e(int aRow);
   int& operator()(int aRow);
   void show(char* aLabel=0);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Execute, overload used to read from a command line file. This is called
   // for each line in the corresponding table section of the file. It parses 
   // the file command line to read table values
   void execute(Ris::CmdLineCmd* aCmd);

   // Execute again. reinitializes and calls execute.
   void reexecute(Ris::CmdLineCmd* aCmd);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace




