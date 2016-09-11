/*==============================================================================

==============================================================================*/
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "my_functions.h"
#include "risCmdLineFile.h"
#include "risPortableCalls.h"

#include "Network_Settings.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

Network_IPAddress_Settings::Network_IPAddress_Settings()
{
   mCOMM   [0]=0;
   mPLT    [0]=0;
   mDIAG   [0]=0;
   mSCP    [0]=0;
   mRTC    [0]=0;
   mGGI    [0]=0;
   mGUI    [0]=0;
   mSCC1   [0]=0;
   mSCC2   [0]=0;
   mPMC    [0]=0;
}

//******************************************************************************

bool Network_IPAddress_Settings::isValid_COMM() {return mCOMM[0] !=0;}
bool Network_IPAddress_Settings::isValid_PLT()  {return mPLT[0]  !=0;}
bool Network_IPAddress_Settings::isValid_DIAG() {return mDIAG[0] !=0;}
bool Network_IPAddress_Settings::isValid_SCP()  {return mSCP[0]  !=0;}
bool Network_IPAddress_Settings::isValid_RTC()  {return mRTC[0]  !=0;}
bool Network_IPAddress_Settings::isValid_GGI()  {return mGGI[0]  !=0;}
bool Network_IPAddress_Settings::isValid_GUI()  {return mGUI[0]  !=0;}
bool Network_IPAddress_Settings::isValid_SCC1() {return mSCC1[0] !=0;}
bool Network_IPAddress_Settings::isValid_SCC2() {return mSCC2[0] !=0;}
bool Network_IPAddress_Settings::isValid_PMC()  {return mPMC[0]  !=0;}

//******************************************************************************

void Network_IPAddress_Settings::show()
{
   printf("IPAddress  COMM          %16s\n", mCOMM);
   printf("IPAddress  PLT           %16s\n", mPLT);
   printf("IPAddress  DIAG          %16s\n", mDIAG);
   printf("IPAddress  SCP           %16s\n", mSCP);
   printf("IPAddress  RTC           %16s\n", mRTC);
   printf("IPAddress  GGI           %16s\n", mGGI);
   printf("\n");

   printf("IPAddress  GUI           %16s\n", mGUI);
   printf("IPAddress  SCC1          %16s\n", mSCC1);
   printf("IPAddress  SCC2          %16s\n", mSCC2);
   printf("IPAddress  PMC           %16s\n", mPMC);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class overload, called for each line in the file

void Network_IPAddress_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))        nestedPop(aCmd);

   // Read variables
   if (aCmd->isCmd("COMM"))       aCmd->copyArgString(1, mCOMM,  MaxIPAddressSize);
   if (aCmd->isCmd("PLT"))        aCmd->copyArgString(1, mPLT,   MaxIPAddressSize);
   if (aCmd->isCmd("DIAG"))       aCmd->copyArgString(1, mDIAG,  MaxIPAddressSize);
   if (aCmd->isCmd("SCP"))        aCmd->copyArgString(1, mSCP,   MaxIPAddressSize);
   if (aCmd->isCmd("RTC"))        aCmd->copyArgString(1, mRTC,   MaxIPAddressSize);
   if (aCmd->isCmd("GGI"))        aCmd->copyArgString(1, mGGI,   MaxIPAddressSize);
   if (aCmd->isCmd("GUI"))        aCmd->copyArgString(1, mGUI,   MaxIPAddressSize);
   if (aCmd->isCmd("SCC1"))       aCmd->copyArgString(1, mSCC1,  MaxIPAddressSize);
   if (aCmd->isCmd("SCC2"))       aCmd->copyArgString(1, mSCC2,  MaxIPAddressSize);
   if (aCmd->isCmd("PMC"))        aCmd->copyArgString(1, mPMC,   MaxIPAddressSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

Network_NetParameters_Settings::Network_NetParameters_Settings()
{
   mNetmask   [0]=0;
   mGateway   [0]=0;
}

//******************************************************************************

bool Network_NetParameters_Settings::isValid_Netmask() {return mNetmask[0] !=0;}
bool Network_NetParameters_Settings::isValid_Gateway() {return mGateway[0] !=0;}

//******************************************************************************

void Network_NetParameters_Settings::show()
{
   printf("NetParameters  Netmask %16s\n", mNetmask);
   printf("NetParameters  Gateway %16s\n", mGateway);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class overload, called for each line in the file

void Network_NetParameters_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))  nestedPop(aCmd);

   // Read variables
   if (aCmd->isCmd("Netmask"))    aCmd->copyArgString(1, mNetmask,  MaxIPAddressSize);
   if (aCmd->isCmd("Gateway"))    aCmd->copyArgString(1, mGateway,  MaxIPAddressSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

Network_Port_Settings::Network_Port_Settings()
{
   mCOMM_TO_SCP = 0;
   mPLT_TO_SCP = 0;
   mDIAG_TO_SCP = 0;
   mRTC_TO_SCP = 0;
   mGGI_TO_SCP = 0;

   mSCP_TO_COMM = 0;
   mSCP_TO_PLT = 0;
   mSCP_TO_DIAG = 0;
   mSCP_TO_RTC = 0;
   mSCP_TO_GGI = 0;

   mSCP_TO_GUI = 0;
   mGUI_TO_SCP = 0;

   mSCP_TO_SCC1 = 0;
   mSCC1_TO_SCP = 0;

   mSCP_TO_SCC2 = 0;
   mSCC2_TO_SCP = 0;

   mSCP_TO_PMC = 0;
   mPMC_TO_SCP = 0;
}

//******************************************************************************

void Network_Port_Settings::show()
{
   printf("Port       COMM_TO_SCP              %5d\n", mCOMM_TO_SCP);
   printf("Port       PLT_TO_SCP               %5d\n", mPLT_TO_SCP);
   printf("Port       DIAG_TO_SCP              %5d\n", mDIAG_TO_SCP);
   printf("Port       RTC_TO_SCP               %5d\n", mRTC_TO_SCP);
   printf("Port       GGI_TO_SCP               %5d\n", mGGI_TO_SCP);
   printf("\n");

   printf("Port       SCP_TO_COMM              %5d\n", mSCP_TO_COMM);
   printf("Port       SCP_TO_PLT               %5d\n", mSCP_TO_PLT);
   printf("Port       SCP_TO_DIAG              %5d\n", mSCP_TO_DIAG);
   printf("Port       SCP_TO_RTC               %5d\n", mSCP_TO_RTC);
   printf("Port       SCP_TO_GGI               %5d\n", mSCP_TO_GGI);
   printf("\n");

   printf("Port       SCP_TO_GUI               %5d\n", mSCP_TO_GUI);
   printf("Port       GUI_TO_SCP               %5d\n", mGUI_TO_SCP);
   printf("\n");

   printf("Port       SCP_TO_SCC1              %5d\n", mSCP_TO_SCC1);
   printf("Port       SCC1_TO_SCP              %5d\n", mSCC1_TO_SCP);
   printf("\n");
 
   printf("Port       SCP_TO_SCC2              %5d\n", mSCP_TO_SCC2);
   printf("Port       SCC2_TO_SCP              %5d\n", mSCC2_TO_SCP);
   printf("\n");

   printf("Port       SCP_TO_PMC               %5d\n", mSCP_TO_PMC);
   printf("Port       PMC_TO_SCP               %5d\n", mPMC_TO_SCP);
   printf("\n");
}

//******************************************************************************
// Base class overload, called for each line in the file

void Network_Port_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))        nestedPop(aCmd);

   // Read variables
   if (aCmd->isCmd("COMM_TO_SCP"))   mCOMM_TO_SCP = aCmd->argInt(1);
   if (aCmd->isCmd("PLT_TO_SCP"))    mPLT_TO_SCP  = aCmd->argInt(1);
   if (aCmd->isCmd("DIAG_TO_SCP"))   mDIAG_TO_SCP = aCmd->argInt(1);
   if (aCmd->isCmd("RTC_TO_SCP"))    mRTC_TO_SCP  = aCmd->argInt(1);
   if (aCmd->isCmd("GGI_TO_SCP"))    mGGI_TO_SCP  = aCmd->argInt(1);

   if (aCmd->isCmd("SCP_TO_COMM"))   mSCP_TO_COMM = aCmd->argInt(1);
   if (aCmd->isCmd("SCP_TO_PLT"))    mSCP_TO_PLT  = aCmd->argInt(1);
   if (aCmd->isCmd("SCP_TO_DIAG"))   mSCP_TO_DIAG = aCmd->argInt(1);
   if (aCmd->isCmd("SCP_TO_RTC"))    mSCP_TO_RTC  = aCmd->argInt(1);
   if (aCmd->isCmd("SCP_TO_GGI"))    mSCP_TO_GGI  = aCmd->argInt(1);

   if (aCmd->isCmd("SCP_TO_GUI"))    mSCP_TO_GUI  = aCmd->argInt(1);
   if (aCmd->isCmd("GUI_TO_SCP"))    mGUI_TO_SCP  = aCmd->argInt(1);

   if (aCmd->isCmd("SCP_TO_SCC1"))   mSCP_TO_SCC1 = aCmd->argInt(1);
   if (aCmd->isCmd("SCC1_TO_SCP"))   mSCC1_TO_SCP = aCmd->argInt(1);
 
   if (aCmd->isCmd("SCP_TO_SCC2"))   mSCP_TO_SCC2 = aCmd->argInt(1);
   if (aCmd->isCmd("SCC2_TO_SCP"))   mSCC2_TO_SCP = aCmd->argInt(1);

   if (aCmd->isCmd("SCP_TO_PMC"))    mSCP_TO_PMC  = aCmd->argInt(1);
   if (aCmd->isCmd("PMC_TO_SCP"))    mPMC_TO_SCP  = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

Network_Settings::Network_Settings()
{
   mCode1 = 0;
   mCode2 = 0;
}

//******************************************************************************

void Network_Settings::show()
{
   printf("\n");
   printf("Network_Settings************************************************************\n");
   printf("\n");

   printf("Code1               %d\n", mCode1);
   printf("Code2               %d\n", mCode2);
   printf("\n");
   mIPAddress.show();
   printf("\n");
   mNetParameters.show();
   printf("\n");
   mPort.show();
}

//******************************************************************************
// Base class overload, called for each line in the file

void Network_Settings::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))        nestedPop(aCmd);

   // Read variables
   if (aCmd->isCmd("Code1"))      mCode1 = aCmd->argInt(1);
   if (aCmd->isCmd("Code2"))      mCode2 = aCmd->argInt(1);
   if (aCmd->isCmd("Begin"))      executeOnBegin(aCmd);
}

//******************************************************************************
// Execute specific command

void Network_Settings::executeOnBegin(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("END"))  nestedPop(aCmd);

   // Read variables
   if (aCmd->isArgString(1,"IPAddress"))
   {
     nestedPush(aCmd, &mIPAddress);
   }
   if (aCmd->isArgString(1,"NetParameters"))
   {
     nestedPush(aCmd, &mNetParameters);
   }
   if (aCmd->isArgString(1,"Port"))
   {
     nestedPush(aCmd, &mPort);
   }
}

