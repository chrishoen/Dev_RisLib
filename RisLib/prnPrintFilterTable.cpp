//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#define  _PRNPRINTFILTERTABLE_CPP_
#include "prnPrintFilterTable.h"

namespace Prn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// No Constructor.

void PrintFilterTable::initialize()
{
   // All disabled, except entry zero.
   for (int i = 0; i < cFilterTableSize; i++)
   {
      mEnable  [i] = false;
      mConsole [i] = 0;
   }
   // Enable entry zero.
   mEnable[0] = true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set a filter in the filter table.
//
// aFilter is the index of the filter.
// aEnable is the value stored in the table at the filter index.
// aConsole is the console index stored in the table at the filter index.

void PrintFilterTable::setFilter(int aFilter, bool aEnable, int aConsole)
{
   mEnable  [aFilter] = aEnable;
   mConsole [aFilter] = aConsole;
}   	

//******************************************************************************
//******************************************************************************
//******************************************************************************
// aFilter is the index of the filter.
// aEnable is the value stored in the table at the filter index.

void PrintFilterTable::enableFilter(int aFilter, bool aEnable)
{
   mEnable[aFilter] = aEnable;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Get a filter in the filter table.
//
// aFilter is the index of the filter.
// Return true if the filter is enabled.

bool PrintFilterTable::getFilter(int aFilter)
{
   return mEnable[aFilter];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace



