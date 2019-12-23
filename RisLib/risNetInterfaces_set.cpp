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
   // Read the json value from the input file.
   std::ifstream tInputFile("/opt/prime/special/interfaces_static");
   std::ofstream tOutputFile("/opt/prime/special/tmp_interfaces_static");
   std::string tLine;
   while (std::getline(tInputFile, tLine))
   {
      tOutputFile << tLine << std::endl;
   }
   tInputFile.close();
   tOutputFile.close();
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
