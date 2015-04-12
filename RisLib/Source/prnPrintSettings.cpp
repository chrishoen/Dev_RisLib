//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>


#define  _PRNPRINTSETTINGS_CPP_
#include "prnPrintSettings.h"

#include "prnPrint.h"

namespace Prn
{

PrintSettings*  ptrPrintSettings() {return &gSettings;}
//******************************************************************************

PrintSettings::PrintSettings()
{
   reset();
}

//******************************************************************************
// Initialize varibles to defaults

void PrintSettings::reset()
{
   mInitFlag=false;
   mSuppressFlag=false;
   mSpecialFlag=false;
   mLogFileEnable=false;
   mAppNumber=0;;

   // all disabled
   for(int row=0;row<FILTER_TABLE_SIZE;row++)
   {
      mFilterTable[row] = 0;
   }

   // enable (0,0)
   setFilter(0,0,true);

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

void PrintSettings::setFilter(int aTopic, int aSubTopic, bool aEnablePrint)
{
   if (aEnablePrint)
   {
      mFilterTable[aTopic] |= aSubTopic;
   }
   else
   {
      mFilterTable[aTopic] &= (~aSubTopic);
   }

}   	

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Try filters and topics

void PrintSettings::tryFilterStart()
{
   mFilterTopic = -1;
   mFilterSubTopic = 0;
}

//******************************************************************************

void PrintSettings::tryFilterTopic(Ris::CmdLineCmd* aCmd,char* aTopicStr,int aTopic)
{
   if (aCmd->isArgString(1,aTopicStr))
   {
      mFilterTopic=aTopic;
   }
}

//******************************************************************************

void PrintSettings::tryFilterSubTopic(Ris::CmdLineCmd* aCmd,char* aSubTopicStr,int aSubTopic)
{
   if (aCmd->isArgString(2,aSubTopicStr))
   {
      mFilterSubTopic=aSubTopic;
   }
}

//******************************************************************************

void PrintSettings::tryFilterSet(Ris::CmdLineCmd* aCmd)
{
   if (mFilterTopic<0)    return;
   if (mFilterSubTopic==0) return;

   setFilter(mFilterTopic,mFilterSubTopic,aCmd->argBool(3));
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
   // Detect a Topic and store

   //---------------------------------------------------------------------------
   // Detect a filter and store, argument 1
   // For command "Filter Topic Subtopic"

   if(aCmd->isCmd("FILTER")) 
   {
      tryFilterStart();

      tryFilterTopic(aCmd,"PrintInit",PrintInit);
      tryFilterTopic(aCmd,"PrintRun",PrintRun);
      tryFilterTopic(aCmd,"PrintPer",PrintPer);
      tryFilterTopic(aCmd,"SocketInit",SocketInit);
      tryFilterTopic(aCmd,"SocketRun",SocketRun);
      tryFilterTopic(aCmd,"SocketPer",SocketPer);
      tryFilterTopic(aCmd,"SerialInit",SerialInit);
      tryFilterTopic(aCmd,"SerialRun",SerialRun);
      tryFilterTopic(aCmd,"FileInit",FileInit);
      tryFilterTopic(aCmd,"FileRun",FileRun);
      tryFilterTopic(aCmd,"FilePer",FilePer);
      tryFilterTopic(aCmd,"ThreadInit",ThreadInit);
      tryFilterTopic(aCmd,"ThreadRun",ThreadRun);
      tryFilterTopic(aCmd,"ThreadPer",ThreadPer);
      tryFilterTopic(aCmd,"ProcInit",ProcInit);
      tryFilterTopic(aCmd,"ProcRun",ProcRun);
      tryFilterTopic(aCmd,"ProcPer",ProcPer);
      tryFilterTopic(aCmd,"Example1Init",Example1Init);
      tryFilterTopic(aCmd,"Example1Run",Example1Run);
      tryFilterTopic(aCmd,"Example1Per",Example1Per);
      tryFilterTopic(aCmd,"QCallThreadInit",QCallThreadInit);
      tryFilterTopic(aCmd,"CommInit",CommInit);
      tryFilterTopic(aCmd,"CommRun",CommRun);

      tryFilterTopic(aCmd,"Msg",Msg);

      // Detect a filter subtopic and store
      // For command "Filter Topic Subtopic"

      tryFilterSubTopic(aCmd,"Any",Any);
      tryFilterSubTopic(aCmd,"Init1",Init1);
      tryFilterSubTopic(aCmd,"Init2",Init2);
      tryFilterSubTopic(aCmd,"Init3",Init3);
      tryFilterSubTopic(aCmd,"Init4",Init4);
      tryFilterSubTopic(aCmd,"Run1",Run1);
      tryFilterSubTopic(aCmd,"Run2",Run2);
      tryFilterSubTopic(aCmd,"Run3",Run3);
      tryFilterSubTopic(aCmd,"Run4",Run4);
      tryFilterSubTopic(aCmd,"Per1",Per1);
      tryFilterSubTopic(aCmd,"Per2",Per2);
      tryFilterSubTopic(aCmd,"Per3",Per3);
      tryFilterSubTopic(aCmd,"Per4",Per4);
      tryFilterSubTopic(aCmd,"Tx1", Tx1);
      tryFilterSubTopic(aCmd,"Tx2", Tx2);
      tryFilterSubTopic(aCmd,"Tx3", Tx3);
      tryFilterSubTopic(aCmd,"Tx4", Tx4);
      tryFilterSubTopic(aCmd,"Rx1", Rx1);
      tryFilterSubTopic(aCmd,"Rx2", Rx2);
      tryFilterSubTopic(aCmd,"Rx3", Rx3);
      tryFilterSubTopic(aCmd,"Rx4", Rx4);
      tryFilterSubTopic(aCmd,"Show1",Show1);
      tryFilterSubTopic(aCmd,"Show2",Show2);
      tryFilterSubTopic(aCmd,"Show3",Show3);
      tryFilterSubTopic(aCmd,"Show4",Show4);
      tryFilterSubTopic(aCmd,"Spare1",Spare1);
      tryFilterSubTopic(aCmd,"Spare2",Spare2);
      tryFilterSubTopic(aCmd,"Spare3",Spare3);
      tryFilterSubTopic(aCmd,"Spare4",Spare4);

      // Set the filter for stored topic and subtopic
      // For command "Filter Topic Subtopic"
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



