#ifndef _PRNPRINTSETTINGS_H_
#define _PRNPRINTSETTINGS_H_
/*==============================================================================

This file contains a set of filters used by prnPrint.h
The filters are initialized from the command line file PrintSettings.txt

==============================================================================*/

//**********************************************************************
//**********************************************************************
//**********************************************************************

#include "risCmdLineFile.h"

namespace Prn
{

//**********************************************************************
//**********************************************************************
//**********************************************************************
// PrintSettings class, inherits from BaseCmdLineExec to process
// command lines from a command line file. Each application reads its
// own print settings from a common settings file. 

class PrintSettings : public Ris::BaseCmdLineExec
{
public:

   PrintSettings();

   void reset();

   // Baseclass override, executes for each line in the settings
   // command line file

   void execute(Ris::CmdLineCmd* aCmd);

   // Initialize.
   // aSettingsFileName is the settings filename. aSection is the settings
   // file section that each application extracts its settings from. 
   // aSectionMode specifies the section mode. Each section begins with a 
   // command line, such as "Begin CmdFile Default", where "Begin" is the 
   // command ,"CmdFile" is  first argument and denotes the section and
   // "Default" is the second argument and denotes the section mode.

   bool initialize(
      char* aSettingsFileName,
      char* aSection);

   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   //--------------------------------------------------------------------------
   // Filter table
 
   static const int cFilterTableSize = 200;

   bool mFilterTable  [cFilterTableSize];
   int  mConsoleTable [cFilterTableSize];

   void setFilter(int aFilter, bool aEnablePrint, int aConsole = 0);

   //--------------------------------------------------------------------------
   // Section

   char mSection[200];
   char mSectionMode[200];

   bool isMySection(Ris::CmdLineCmd* aCmd);
   bool mSectionFlag;

   int  mAppNumber;

   void tryFilterStart    ();
   void tryFilterString   (Ris::CmdLineCmd* aCmd,char* aFilterString,int aFilter);
   void tryFilterSet      (Ris::CmdLineCmd* aCmd);

   int mFilterTry;

   //--------------------------------------------------------------------------
   // Print control

   // If this flag is true then initialize has been called
   bool  mInitFlag;

   // If this flag is true then any print that does not have a
   // filter,subfilter of 0,0 will not be printed
   bool  mSuppressFlag;

   // If this flag is false then any special print immediately returns.
   bool  mSpecialFlag;

   // Log file for print.txt
   bool  mLogFileEnable;
   char  mLogFileName[200];
};

//**********************************************************************
//**********************************************************************
//**********************************************************************
// Filter constants

static const int  FilterZero = 0;
static const int  PrintInit1 = 1;
static const int  PrintInit2 = 2;
static const int  PrintInit3 = 3;
static const int  PrintInit4 = 4;
static const int  PrintRun1 = 5;
static const int  PrintRun2 = 6;
static const int  PrintRun3 = 7;
static const int  PrintRun4 = 8;
static const int  SocketInit1 = 9;
static const int  SocketInit2 = 10;
static const int  SocketInit3 = 11;
static const int  SocketInit4 = 12;
static const int  SocketRun1 = 13;
static const int  SocketRun2 = 14;
static const int  SocketRun3 = 15;
static const int  SocketRun4 = 16;
static const int  SerialInit1 = 17;
static const int  SerialInit2 = 18;
static const int  SerialInit3 = 19;
static const int  SerialInit4 = 20;
static const int  SerialRun1 = 21;
static const int  SerialRun2 = 22;
static const int  SerialRun3 = 23;
static const int  SerialRun4 = 24;
static const int  FileInit1 = 25;
static const int  FileInit2 = 26;
static const int  FileInit3 = 27;
static const int  FileInit4 = 28;
static const int  FileRun1 = 29;
static const int  FileRun2 = 30;
static const int  FileRun3 = 31;
static const int  FileRun4 = 32;
static const int  ThreadInit1 = 33;
static const int  ThreadInit2 = 34;
static const int  ThreadInit3 = 35;
static const int  ThreadInit4 = 36;
static const int  ThreadRun1 = 37;
static const int  ThreadRun2 = 38;
static const int  ThreadRun3 = 39;
static const int  ThreadRun4 = 40;
static const int  ProcInit1 = 41;
static const int  ProcInit2 = 42;
static const int  ProcInit3 = 43;
static const int  ProcInit4 = 44;
static const int  ProcRun1 = 45;
static const int  ProcRun2 = 46;
static const int  ProcRun3 = 47;
static const int  ProcRun4 = 48;
static const int  ExampleInit1 = 49;
static const int  ExampleInit2 = 50;
static const int  ExampleInit3 = 51;
static const int  ExampleInit4 = 52;
static const int  ExampleRun1 = 53;
static const int  ExampleRun2 = 54;
static const int  ExampleRun3 = 55;
static const int  ExampleRun4 = 56;
static const int  QCallInit1 = 57;
static const int  QCallInit2 = 58;
static const int  QCallInit3 = 59;
static const int  QCallInit4 = 60;
static const int  QCallRun1 = 61;
static const int  QCallRun2 = 62;
static const int  QCallRun3 = 63;
static const int  QCallRun4 = 64;
static const int  CommInit1 = 65;
static const int  CommInit2 = 66;
static const int  CommInit3 = 67;
static const int  CommInit4 = 68;
static const int  CommRun1 = 69;
static const int  CommRun2 = 70;
static const int  CommRun3 = 71;
static const int  CommRun4 = 72;
static const int  ViewInit1 = 73;
static const int  ViewInit2 = 74;
static const int  ViewInit3 = 75;
static const int  ViewInit4 = 76;
static const int  ViewRun1 = 77;
static const int  ViewRun2 = 78;
static const int  ViewRun3 = 79;
static const int  ViewRun4 = 80;
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance

#ifdef _PRNPRINTSETTINGS_CPP_
        PrintSettings gSettings;
#else
        extern PrintSettings gSettings;
#endif

}//namespace
#endif

