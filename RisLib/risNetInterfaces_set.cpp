//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "json.h"
#include "risPortableCalls.h"
#include "risSockets.h"
#include "risFileFunctions.h"
#include "risSystemCalls.h"
#include "risThreadsThreads.h"
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
   // Copy the dhcp file to the interfaces file.
   doSystemCommand("cp /opt/prime/special/interfaces_dhcp /etc/network/interfaces");

   // Guard.
   if (!mEth0ValidFlag)
   {
      printf("the file is set, but eth0 is invalid\n");
      return;
   }

   // Bring the interface down and then up.
   doSystemCommand("ifdown --force eth0");
   Threads::threadSleep(1000);
   doSystemCommand("ifup --force eth0");
   Threads::threadSleep(1000);
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
         tOutputLine.replace(tFound, tReplaceAddress.length(),mEth0WantAddress);
      }

      // Find and replace mask tag. 
      tFound = tOutputLine.find(tReplaceMask);
      if (tFound != std::string::npos)
      {
         tOutputLine.replace(tFound, tReplaceMask.length(), mEth0WantMask);
      }

      // Find and replace gateway tag. 
      tFound = tOutputLine.find(tReplaceGateway);
      if (tFound != std::string::npos)
      {
         tOutputLine.replace(tFound, tReplaceGateway.length(), mEth0WantGateway);
      }

      // Write the the output line.
      tOutputFile << tOutputLine << std::endl;
   }
   tInputFile.close();
   tOutputFile.close();

   // Copy the temp file to the interfaces file.
   doSystemCommand("cp /opt/prime/special/tmp_interfaces_static /etc/network/interfaces");

   // Guard.
   if (!mEth0ValidFlag)
   {
      printf("the file is set, but eth0 is invalid\n");
      return;
   }

   // Bring the interface down and then up.
   doSystemCommand("ifdown --force eth0");
   Threads::threadSleep(1000);
   doSystemCommand("ifup --force eth0");
   Threads::threadSleep(1000);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set a member variable. Return true if successful.

bool Interfaces::setEth0WantAddress(std::string aString)
{
   if (Ris::Sockets::isIpAddressValid(aString.c_str()))
   {
      mEth0WantAddress = aString;
      return true;
   }
   else
   {
      Prn::print(0, "Interfaces::setEth0WantAddress ERROR");
      return false;
   }
}
bool Interfaces::setEth0WantMask(std::string aString)
{
   if (Ris::Sockets::isIpAddressValid(aString.c_str()))
   {
      mEth0WantMask = aString;
      return true;
   }
   else
   {
      Prn::print(0, "Interfaces::setEth0WantMask ERROR");
      return false;
   }
}
bool Interfaces::setEth0WantGateway(std::string aString)
{
   if (Ris::Sockets::isIpAddressValid(aString.c_str()))
   {
      mEth0WantGateway = aString;
      return true;
   }
   else
   {
      Prn::print(0, "Interfaces::setEth0WantGateway ERROR");
      return false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace




// doSystemCommand("ip link set dev eth0 down");
// doSystemCommand("ip link set dev eth0 up");
