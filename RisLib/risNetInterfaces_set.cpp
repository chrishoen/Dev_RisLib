//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "json.h"
#include "risPortableCalls.h"
#include "risFileFunctions.h"
#include "risSystemCalls.h"
#include "my_functions.h"
#include "prnPrint.h"

#include "risNetInterfaces.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the network settings for eth0.

void Interfaces::doSetNetSettingsEth0()
{

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the network settings for eth0.

void Interfaces::doSetNetSettingsEth0_dhcp()
{
   doSystemCommand("cp /opt/prime/special/interfaces_dhcp /etc/network/interfaces");
   doSystemCommand("ip link set dev eth0 down");
   doSystemCommand("ip link set dev eth0 up");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the network settings for eth0.

void Interfaces::doSetNetSettingsEth0_static()
{
   // Input and output files.
   std::ifstream tInputFile("/opt/prime/special/interfaces_static");
   std::ofstream tOutputFile("/opt/prime/special/tmp_interfaces_static");

   // String variables.
   std::string tInputLine;
   std::string tOutputLine;
   size_t tFound = 0;
   std::string tReplaceAddress = "REPLACE_ADDRESS";
   std::string tReplaceMask = "REPLACE_MASK";
   std::string tReplaceGateway = "REPLACE_GATEWAY";

   // Copy the input template file to the output file. Find and replace
   // tags for address, mask, and gateway.
   while (std::getline(tInputFile, tInputLine))
   {
      // Copy input line to output line.
      tOutputLine = tInputLine;

      // Find and replace address tag. 
      tFound = tOutputLine.find(tReplaceAddress);
      if (tFound != std::string::npos)
      {
         tOutputLine.replace(tFound, tReplaceAddress.length(),mEth0Address);
      }

      // Find and replace mask tag. 
      tFound = tOutputLine.find(tReplaceMask);
      if (tFound != std::string::npos)
      {
         tOutputLine.replace(tFound, tReplaceMask.length(), mEth0Mask);
      }

      // Find and replace gateway tag. 
      tFound = tOutputLine.find(tReplaceGateway);
      if (tFound != std::string::npos)
      {
         tOutputLine.replace(tFound, tReplaceGateway.length(), mEth0Gateway);
      }

      // Write the the output line.
      tOutputFile << tOutputLine << std::endl;
   }
   tInputFile.close();
   tOutputFile.close();
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
