#pragma once

/*==============================================================================
This unit provides a function that is similar to printf which can be filtered. 
The way that it is used is that there can be many prints located throughout an
application, with all them being called, but only those ones that are enabled 
actually printing, the ones that are filtered just immediately return.

When a print is executed it is passed a filter index. If the value of the 
filter table at the specified index is enabled then the print is executed.

This unit supports multiple consoles, with the default being one console, the
console application console, with a console index of zero. If more than one
consoles are specified then PrintView console applications are launched and
prints are sent to them vai udp sockets. Console indices for a specific
filter index are stored in the filter table.
==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "prnPrintFilters.h"
#include "prnPrintFilterTable.h"

namespace Prn
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initializes the print facility.

// Reset settings to defaults.
void resetPrint();

// Reset settings to defaults.
// Supply the print view udp ip address. This will override that which is
// specified in the default print settings file.
void resetPrint(const char* aPrintViewIPAddress, int aPrintViewIPPort = 58000);

// Specifie a print view console to use. If it is greater than zero 
// then an additional PrintView console application launched
// and prints are redirected to them via udp sockets.
void useConsole(int   aConsole);

// Use an application specific filter table. This is used by programs
// that use shared memory.
void usePrintFilterTable(PrintFilterTable* aTable);

// Initialize prints using the effects of the above functions.
void initializePrint();

// Close the udp sockets.
void finalizePrint();

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Suppress any prints that do not have a filter of zero.

void suppressPrint       ();
void unsuppressPrint     ();
void toggleSuppressPrint ();

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Filtered print, if the corresponding entry in the filter table is true
// then the print is executed.

void print (int aFilter, const char* aFormat, ...);

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Filters.

// Set a filter in the filter table.
//
// aFilter is the index of the filter.
// aEnable is the value stored in the table at the filter index.
// aConsole is the console index stored in the table at the filter index.
void  setFilter(int aFilter, bool aEnable, int aConsole = 0);

// Enable a filter in the filter table.
//
// aFilter is the index of the filter.
// aEnable is the value stored in the table at the filter index.
void enableFilter(int aFilter, bool aEnable);

// Get a filter in the filter table.
//
// aFilter is the index of the filter.
// Return true if the filter is enabled.
bool getFilter(int aFilter);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

