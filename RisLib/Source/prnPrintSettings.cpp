//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>


#define  _PRNPRINTSETTINGS_CPP_
#include "prnPrintSettings.h"

namespace Prn
{


//******************************************************************************

PrintSettings::PrintSettings()
{
   reset();
}

//******************************************************************************
// Initialize variables to defaults

void PrintSettings::reset()
{
   mInitFlag=false;
   mSuppressFlag=false;
   mSpecialFlag=false;
   mLogFileEnable=false;
   mAppNumber=0;;

   // All disabled, except entry zero
   for(int i=0;i<cFilterTableSize;i++)
   {
      mFilterTable  [i] = false;
      mConsoleTable [i] = 0;
   }
   mFilterTable[0] = true;

   mSectionFlag=true;
   mSection[0]=0;
}

//******************************************************************************
// For a given command line "Begin Section", this returns true
// if "Section", the first command line argument, is the same as the section 
// specified in initialize.

bool PrintSettings::isMySection(Ris::CmdLineCmd* aCmd)
{
   bool tFlag=false;

   if (aCmd->numArg()==1)
   {
      if (aCmd->isArgString(1,mSection))
      {
         tFlag=true;
      }
   }

   return tFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// set filters

void PrintSettings::setFilter(int aFilter, bool aEnablePrint, int aConsole)
{
   mFilterTable  [aFilter] = aEnablePrint;
   mConsoleTable [aFilter] = aConsole;
}   	

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Try filters and topics

void PrintSettings::tryFilterStart()
{
   mFilterTry = -1;
}

//******************************************************************************

void PrintSettings::tryFilterString (Ris::CmdLineCmd* aCmd,char* aFilterString,int aFilter)
{
   if (aCmd->isArgString(1,aFilterString))
   {
      mFilterTry=aFilter;
   }
}

//******************************************************************************

void PrintSettings::tryFilterSet(Ris::CmdLineCmd* aCmd)
{
   if (mFilterTry<0)    return;
   
   if (aCmd->numArg() == 2)
   {
      setFilter(mFilterTry, aCmd->argBool(2));
   }
   else
   {
      setFilter(mFilterTry, aCmd->argBool(2), aCmd->argInt(3));
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is called for each command line in the settings file.
// It processes commands, with arguments
// BEGIN starts a section, END exits a section
// Only commands for a section are processed

void PrintSettings::execute(Ris::CmdLineCmd* aCmd)
{
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Section commands


	if(aCmd->isCmd("SectionBegin"      ))  mSectionFlag=isMySection(aCmd);
   if(aCmd->isCmd("SectionEnd"        ))  mSectionFlag=false;

   if (!mSectionFlag) return;

   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   //---------------------------------------------------------------------------
   // Only process commands for the section specified in initialize.

   //---------------------------------------------------------------------------
   if(aCmd->isCmd("TEST"       ))
   {
      printf("TEST1   %d\n",aCmd->argInt(1));
      printf("TEST2   %d\n",aCmd->argInt(2));
      printf("TEST3   %d\n",aCmd->argInt(3));
      printf("TEST4   %d\n",aCmd->argInt(4));
   }

   if(aCmd->isCmd("RESET"      ))  reset();

   //---------------------------------------------------------------------------
   // Detect a filter and store
   // For command "Filter String Enable"

   if(aCmd->isCmd("FILTER")) 
   {
      tryFilterStart();

      tryFilterString (aCmd, "PrintInit1",   PrintInit1   );
      tryFilterString (aCmd, "PrintInit2",   PrintInit2   );
      tryFilterString (aCmd, "PrintInit3",   PrintInit3   );
      tryFilterString (aCmd, "PrintInit4",   PrintInit4   );
      tryFilterString (aCmd, "PrintRun1",    PrintRun1    );
      tryFilterString (aCmd, "PrintRun2",    PrintRun2    );
      tryFilterString (aCmd, "PrintRun3",    PrintRun3    );
      tryFilterString (aCmd, "PrintRun4",    PrintRun4    );
      tryFilterString (aCmd, "SocketInit1",  SocketInit1  );
      tryFilterString (aCmd, "SocketInit2",  SocketInit2  );
      tryFilterString (aCmd, "SocketInit3",  SocketInit3  );
      tryFilterString (aCmd, "SocketInit4",  SocketInit4  );
      tryFilterString (aCmd, "SocketRun1",   SocketRun1   );
      tryFilterString (aCmd, "SocketRun2",   SocketRun2   );
      tryFilterString (aCmd, "SocketRun3",   SocketRun3   );
      tryFilterString (aCmd, "SocketRun4",   SocketRun4   );
      tryFilterString (aCmd, "SerialInit1",  SerialInit1  );
      tryFilterString (aCmd, "SerialInit2",  SerialInit2  );
      tryFilterString (aCmd, "SerialInit3",  SerialInit3  );
      tryFilterString (aCmd, "SerialInit4",  SerialInit4  );
      tryFilterString (aCmd, "SerialRun1",   SerialRun1   );
      tryFilterString (aCmd, "SerialRun2",   SerialRun2   );
      tryFilterString (aCmd, "SerialRun3",   SerialRun3   );
      tryFilterString (aCmd, "SerialRun4",   SerialRun4   );
      tryFilterString (aCmd, "FileInit1",    FileInit1    );
      tryFilterString (aCmd, "FileInit2",    FileInit2    );
      tryFilterString (aCmd, "FileInit3",    FileInit3    );
      tryFilterString (aCmd, "FileInit4",    FileInit4    );
      tryFilterString (aCmd, "FileRun1",     FileRun1     );
      tryFilterString (aCmd, "FileRun2",     FileRun2     );
      tryFilterString (aCmd, "FileRun3",     FileRun3     );
      tryFilterString (aCmd, "FileRun4",     FileRun4     );
      tryFilterString (aCmd, "ThreadInit1",  ThreadInit1  );
      tryFilterString (aCmd, "ThreadInit2",  ThreadInit2  );
      tryFilterString (aCmd, "ThreadInit3",  ThreadInit3  );
      tryFilterString (aCmd, "ThreadInit4",  ThreadInit4  );
      tryFilterString (aCmd, "ThreadRun1",   ThreadRun1   );
      tryFilterString (aCmd, "ThreadRun2",   ThreadRun2   );
      tryFilterString (aCmd, "ThreadRun3",   ThreadRun3   );
      tryFilterString (aCmd, "ThreadRun4",   ThreadRun4   );
      tryFilterString (aCmd, "ProcInit1",    ProcInit1    );
      tryFilterString (aCmd, "ProcInit2",    ProcInit2    );
      tryFilterString (aCmd, "ProcInit3",    ProcInit3    );
      tryFilterString (aCmd, "ProcInit4",    ProcInit4    );
      tryFilterString (aCmd, "ProcRun1",     ProcRun1     );
      tryFilterString (aCmd, "ProcRun2",     ProcRun2     );
      tryFilterString (aCmd, "ProcRun3",     ProcRun3     );
      tryFilterString (aCmd, "ProcRun4",     ProcRun4     );
      tryFilterString (aCmd, "ExampleInit1", ExampleInit1 );
      tryFilterString (aCmd, "ExampleInit2", ExampleInit2 );
      tryFilterString (aCmd, "ExampleInit3", ExampleInit3 );
      tryFilterString (aCmd, "ExampleInit4", ExampleInit4 );
      tryFilterString (aCmd, "ExampleRun1",  ExampleRun1  );
      tryFilterString (aCmd, "ExampleRun2",  ExampleRun2  );
      tryFilterString (aCmd, "ExampleRun3",  ExampleRun3  );
      tryFilterString (aCmd, "ExampleRun4",  ExampleRun4  );
      tryFilterString (aCmd, "QCallInit1",   QCallInit1   );
      tryFilterString (aCmd, "QCallInit2",   QCallInit2   );
      tryFilterString (aCmd, "QCallInit3",   QCallInit3   );
      tryFilterString (aCmd, "QCallInit4",   QCallInit4   );
      tryFilterString (aCmd, "QCallRun1",    QCallRun1    );
      tryFilterString (aCmd, "QCallRun2",    QCallRun2    );
      tryFilterString (aCmd, "QCallRun3",    QCallRun3    );
      tryFilterString (aCmd, "QCallRun4",    QCallRun4    );
      tryFilterString (aCmd, "CommInit1",    CommInit1    );
      tryFilterString (aCmd, "CommInit2",    CommInit2    );
      tryFilterString (aCmd, "CommInit3",    CommInit3    );
      tryFilterString (aCmd, "CommInit4",    CommInit4    );
      tryFilterString (aCmd, "CommRun1",     CommRun1     );
      tryFilterString (aCmd, "CommRun2",     CommRun2     );
      tryFilterString (aCmd, "CommRun3",     CommRun3     );
      tryFilterString (aCmd, "CommRun4",     CommRun4     );
      tryFilterString (aCmd, "MsgTx1",       MsgTx1       );
      tryFilterString (aCmd, "MsgTx2",       MsgTx2       );
      tryFilterString (aCmd, "MsgTx3",       MsgTx3       );
      tryFilterString (aCmd, "MsgTx4",       MsgTx4       );
      tryFilterString (aCmd, "MsgRx1",       MsgRx1       );
      tryFilterString (aCmd, "MsgRx2",       MsgRx2       );
      tryFilterString (aCmd, "MsgRx3",       MsgRx3       );
      tryFilterString (aCmd, "MsgRx4",       MsgRx4       );
      tryFilterString (aCmd, "ViewInit1",    ViewInit1    );
      tryFilterString (aCmd, "ViewInit2",    ViewInit2    );
      tryFilterString (aCmd, "ViewInit3",    ViewInit3    );
      tryFilterString (aCmd, "ViewInit4",    ViewInit4    );
      tryFilterString (aCmd, "ViewRun1",     ViewRun1     );
      tryFilterString (aCmd, "ViewRun2",     ViewRun2     );
      tryFilterString (aCmd, "ViewRun3",     ViewRun3     );
      tryFilterString (aCmd, "ViewRun4",     ViewRun4     );
      tryFilterString (aCmd, "CamInit1",     CamInit1     );
      tryFilterString (aCmd, "CamInit2",     CamInit2     );
      tryFilterString (aCmd, "CamInit3",     CamInit3     );
      tryFilterString (aCmd, "CamInit4",     CamInit4     );
      tryFilterString (aCmd, "CamRun1",      CamRun1      );
      tryFilterString (aCmd, "CamRun2",      CamRun2      );
      tryFilterString (aCmd, "CamRun3",      CamRun3      );
      tryFilterString (aCmd, "CamRun4",      CamRun4      );
      tryFilterString (aCmd, "CamError1",    CamError1    );
      tryFilterString (aCmd, "CamError2",    CamError2    );
      tryFilterString (aCmd, "CamError3",    CamError3    );
      tryFilterString (aCmd, "CamError4",    CamError4    );
      tryFilterString (aCmd, "SensorInit1",  SensorInit1     );
      tryFilterString (aCmd, "SensorInit2",  SensorInit2     );
      tryFilterString (aCmd, "SensorInit3",  SensorInit3     );
      tryFilterString (aCmd, "SensorInit4",  SensorInit4     );
      tryFilterString (aCmd, "SensorRun1",   SensorRun1      );
      tryFilterString (aCmd, "SensorRun2",   SensorRun2      );
      tryFilterString (aCmd, "SensorRun3",   SensorRun3      );
      tryFilterString (aCmd, "SensorRun4",   SensorRun4      );
      tryFilterString (aCmd, "SensorError1", SensorError1    );
      tryFilterString (aCmd, "SensorError2", SensorError2    );
      tryFilterString (aCmd, "SensorError3", SensorError3    );
      tryFilterString (aCmd, "SensorError4", SensorError4    );

      // Set the stored try filter
      tryFilterSet(aCmd);
   }
}



//******************************************************************************
//******************************************************************************
//******************************************************************************

bool PrintSettings::initialize(char* aSettingsFileName,char* aSection)
{ 
   // Guard
   if ((aSettingsFileName==0)||(aSection==0))
   {
      printf("ERROR PrintSettings::initialize\n");
      return false;
   }

   // Copy arguments
   strcpy(mSection,aSection);
   // Apply settings file to this executive   
   Ris::CmdLineFile tCmdLineFile;

   if (tCmdLineFile.open(aSettingsFileName))
   {
      tCmdLineFile.execute(this);
      tCmdLineFile.close();
      return true;
   }
   else
   {
      printf("PrintSettings::file open failed\n");
      return false;
   }
}

}//namespace



