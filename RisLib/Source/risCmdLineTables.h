#ifndef _RISRTABLE_H_
#define _RISRTABLE_H_
/*==============================================================================

These are some real valued table classes that can be used by command line file
classes. They encapsulate 1 or 2 dimensional tables of doubles.

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
// Table, 1 dimensional, indexed by 0..R-1
  
class CmdLineDoubleTable1D : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------

   // Vector components
   double* mValues;
   int     mRows;

   // Constructor
   CmdLineDoubleTable1D();
  ~CmdLineDoubleTable1D();
   void reset();

   // Initialize and allocate memory for the table
   void initialize(int aRows);

   // Access components
   double& e(int aRow);
   double& operator()(int aRow);
   void show(char* aLabel=0);

   //---------------------------------------------------------------------------
   // Execute, overload used to read from a command line file. This is called
   // for each line in the corresponding table section of the file. It parses 
   // the file command line to read table values

   void execute(Ris::CmdLineCmd* aCmd);

   // Execute again. reinitializes and calls execute.
   void reexecute(Ris::CmdLineCmd* aCmd);

   // True if initialized
   bool mInitialized;
   // True if this is the first command line
   bool mFirstCmd;
   // Number of doubles allocated in mValues
   int  mAlloc;
   // Current index into mValues
   int  mIndex;
   // True if all values freqd in correctly
   bool mValidFlag;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Table, 2 dimensional, indexed by 0..R-1,0..C-1

class CmdLineDoubleTable2D : public Ris::BaseCmdLineExec
{
public:
   // Matrix components
   double* mValues;
   int     mRows;
   int     mCols;

   // Constructor
   CmdLineDoubleTable2D();
  ~CmdLineDoubleTable2D();
   void reset();

   // Initialize and allocate memory for the table
   void initialize(int aRows,int aCols);

   // Access components
   double& e(int aRow,int aCol);
   double& operator()(int aRow,int aCol);
   void show(char* aLabel=0);

   //---------------------------------------------------------------------------
   // Execute, overload used to read from a command line file. This is called
   // for each line in the corresponding table section of the file. It parses 
   // the file command line to read table values

   // Execute.
   void execute(Ris::CmdLineCmd* aCmd);

   // Execute again. reinitializes and calls execute.
   void reexecute(Ris::CmdLineCmd* aCmd);

   // True if initialized
   bool mInitialized;
   // True if this is the first command line
   bool mFirstCmd;
   // Number of doubles allocated in mValues
   int  mAlloc;
   // Current index into mValues
   int  mIndex;
   // True if all values freqd in correctly
   bool mValidFlag;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Table, 1 dimensional, indexed by 0..R-1

class CmdLineStringTable : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------

   // String table components
   char*  mStringStore;
   char** mStringPtr;
   int    mRows;
   int    mStringSize;

   // Constructor
   CmdLineStringTable();
  ~CmdLineStringTable();

   // Initialize and allocate memory for the table
   void initialize(
      int     aRows,
      int     aStringSize);

   // Access components
   char* e(int aRow);
   char* operator()(int aRow);
   void show(char* aLabel=0);

   //---------------------------------------------------------------------------
   // Execute, overload used to read from a command line file. This is called
   // for each line in the corresponding table section of the file. It parses 
   // the file command line to read table values

   void execute(Ris::CmdLineCmd* aCmd);

   // True if initialized
   bool mInitialized;
   // True if this is the first command line
   bool mFirstCmd;
   // Number of doubles allocated in mValues
   int  mAlloc;
   // Current index into mValues
   int  mIndex;
   // True if all values freqd in correctly
   bool mValidFlag;
};

//******************************************************************************

}//namespace
#endif



