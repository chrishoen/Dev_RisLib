#include "stdafx.h"

#include "risFileFunctions.h"
#include "risSystemCalls.h"
#include "risNetEth0.h"
#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("GO1"))   executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))   executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))   executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))   executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))   executeGo5(aCmd);
   if (aCmd->isCmd("GO6"))   executeGo6(aCmd);
   if (aCmd->isCmd("GO7"))   executeGo7(aCmd);
   if (aCmd->isCmd("GO8"))   executeGo8(aCmd);
   if (aCmd->isCmd("GO9"))   executeGo9(aCmd);
   if (aCmd->isCmd("GO10"))  executeGo10(aCmd);
   if (aCmd->isCmd("GO11"))  executeGo11(aCmd);
   if (aCmd->isCmd("GO12"))  executeGo12(aCmd);
   if (aCmd->isCmd("GO13"))  executeGo13(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   std::string tFileName = Ris::doGetRandomFileName();
   Prn::print(0, "Unique FileName     %s", tFileName.c_str());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   std::vector<std::string> tResponse;
   Ris::doSystemCommand("ls", tResponse);

   for (int i = 0; i < tResponse.size(); i++)
   {
      Prn::print(0,"RESPONSE %d $ %s",i, tResponse[i].c_str());
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   std::vector<std::string> tResponse;
   Ris::doSystemCommand("ifconfig eth0", tResponse);

   for (int i = 0; i < tResponse.size(); i++)
   {
      Prn::print(0, "RESPONSE %d $ %s", i, tResponse[i].c_str());
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   std::vector<std::string> tResponse;
   Ris::doSystemCommand("ifconfig eth0", tResponse);

   std::vector<std::string> tParse;
   char tDelimiter = ' ';
   std::stringstream tStream(tResponse[1]);
   std::string tToken;

   while (std::getline(tStream, tToken, tDelimiter))
   {
      if (tToken.length())
      {
         tParse.push_back(tToken);
      }
   }

   std::string tAddr;
   std::string tMask;

   for (int i = 0; i < tParse.size(); i++)
   {
      Prn::print(0, "tParse $ %s", tParse[i].c_str());
      if (tParse[i] == "inet") tAddr = tParse[i + 1];
      if (tParse[i] == "netmask") tMask = tParse[i + 1];
   }

   Prn::print(0, "");
   Prn::print(0, "tAddr  $ %s", tAddr.c_str());
   Prn::print(0, "tMask  $ %s", tMask.c_str());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   std::vector<std::string> tResponse;
   Ris::doSystemCommand("ip route show", tResponse);

   std::vector<std::string> tParse;
   char tDelimiter = ' ';
   std::stringstream tStream(tResponse[0]);
   std::string tToken;

   while (std::getline(tStream, tToken, tDelimiter))
   {
      if (tToken.length())
      {
         tParse.push_back(tToken);
      }
   }

   std::string tGateway;
   bool tDhcpFlag = false;

   for (int i = 0; i < tParse.size(); i++)
   {
      Prn::print(0, "tParse $ %s", tParse[i].c_str());
      if (tParse[i] == "via") tGateway = tParse[i + 1];
      if (tParse[i] == "dhcp") tDhcpFlag = true;
   }

   Prn::print(0, "");
   Prn::print(0, "tGateway  $ %s", tGateway.c_str());
   Prn::print(0, "tDhcpFlag $ %s", my_string_from_bool(tDhcpFlag));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo7(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo8(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo9(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo10(Ris::CmdLineCmd* aCmd)
{
   Ris::Net::Eth0 tEth0;
   tEth0.doGetNetsettings();
   tEth0.doWrite();
   tEth0.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo11(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo12(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo13(Ris::CmdLineCmd* aCmd)
{
}


