#pragma once
/*==============================================================================

This file contains a set of filters used by prnPrint.h
The filters are initialized from the command line file FilterTable.txt

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Prn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// FilterTable class, inherits from BaseCmdLineExec to process
// command lines from a command line file. Each application reads its
// own print settings from a common settings file. 

class PrintFilterTable
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Filter table size.
   static const int cFilterTableSize = 300;

   // Max number of print view consoles.
   static const int cMaxNumConsoles = 5;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Array of print filters. If true then print is enabled.
   bool mEnable[cFilterTableSize];

   // Array of print consoles numbers for each filter. If zero then stdout is
   // used for the console to print to. If nonzero then it is the print view
   // console number.
   int  mConsole[cFilterTableSize];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   PrintFilterTable();
   void reset();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Set a filter in the filter table.
   //
   // aFilter is the index of the filter.
   // aEnablePrint is the value stored in the table at the filter index.
   // aConsole is the console index stored in the table at the filter index.
   void setFilter(int aFilter, bool aEnablePrint, int aConsole = 0);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _PRNPRINTFILTERTABLE_CPP_
        PrintFilterTable gPrintFilterTable;
#else
        extern PrintFilterTable gPrintFilterTable;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

