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

class RTable1D : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Table, 1 dimensional, indexed by 0..R-1

   // Vector components
   double* mValues;
   int mRows;

   // Constructor
   RTable1D();
   ~RTable1D();

   // Initialize and allocate memory for the table
   void initialize(
      int     aRows);

   // Access components
   double& e(int aRow);
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
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

class RTable2D : public Ris::BaseCmdLineExec
{
public:
   //---------------------------------------------------------------------------
   // Table, 2 dimensional, indexed by 0..R-1,0..C-1

   // Matrix components
   double* mValues;
   int mRows;
   int mCols;

   // Constructor
   RTable2D();
  ~RTable2D();

   // Initialize and allocate memory for the table
   void initialize(
      int     aRows,
      int     aCols);

   // Access components
   double& e(int aRow,int aCol);
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
};

//******************************************************************************

}//namespace
#endif



