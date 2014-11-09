#ifndef _PRNPRINT_H_
#define _PRNPRINT_H_

/*==============================================================================
This unit provides a function that is similar to printf which can
be filtered. The way that it is used is that there can be many prints
located throughout an application, with all them being called, but only
those ones that are enabled actually printing, the ones that are
filtered just immediately return.
The prints are sent to a single low priority background task for
output. The filtering uses a filter table that is set at program
initialization or modified on the fly. 

When a print is executed it is passed a filter topic and subtopic.
If the value of the filter table at the specified topic and
subtopic is enabled then the print is executed.

==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"
#include "prnPrintSettings.h"

namespace Prn
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initializes the print service task and the filter table

enum PrintMode
{
   PrintModeLocal,          // This prints to stdout directly
   PrintModeLocalThreaded,  // This prints to stdout via the print thread
   PrintModeRemote          // This prints via the remote server
};

void initializePrint(
   PrintMode aPrintMode=PrintModeLocalThreaded);

// Initializes the print service task and the filter table
// from an already initialized print settings global object
// Print control initializes the settings object and then
// calls this

// Initialize.
// aSettingsFileName is the settings filename. aSection is the settings
// file section that each application extracts its settings from. 
// aSectionMode specifies the section mode. Each section begins with a 
// command line, such as "Begin CmdFile Default", where "Begin" is the 
// command ,"CmdFile" is  first argument and denotes the section and
// "Default" is the second argument and denotes the section mode.

void initializePrintFromSettings(
   char* aSettingsFileName,
   char* aSection,
   PrintMode aPrintMode=PrintModeLocalThreaded);

// This reads the settings from the settings file from 
// the settings folder, and "Default",1
void  initializePrintFromSettings(
   PrintMode aPrintMode=PrintModeLocalThreaded);

// Closes the print facility.

void  closePrint();

//******************************************************************************
//******************************************************************************
//******************************************************************************
// supress any prints that do not have a filter,subfilter of 0,0

void  suppressPrint();
void  unsuppressPrint();
void  toggleSuppressPrint();

//******************************************************************************
//******************************************************************************
//******************************************************************************
//
// Filtering:
//
//    1) If not enabled then the print is not executed.
//
//    2) ElseIf Topic and SubTopic are enabled then the print is executed.
//

void  print   (int aTopic,int aSubTopic,const char* aFormat, ...);

//******************************************************************************
//******************************************************************************
//******************************************************************************
// access the filter table

// sets the filter table for a topic and subtopic.

void  setFilter(int aTopic, int aSubTopic, bool aEnablePrint);

}//namespace
#endif

