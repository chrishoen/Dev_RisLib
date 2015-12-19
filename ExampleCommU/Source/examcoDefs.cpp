/*==============================================================================
File: hwExecState.cpp
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "risPortableCalls.h"

#include "examcoDefs.h"

namespace ExampleComm
{

//******************************************************************************
char* Defs::getAppRoleNameByEnum(int aId)
{
   switch (aId)
   {
      case cRoleTcpServer  :  return "TcpServer";   break;
      case cRoleTcpClient  :  return "TcpClient";   break;
      case cRoleUdpPeer    :  return "UdpPeer";     break;
   }
   return  "UNKNOWN";

};
//******************************************************************************
int Defs::getAppRoleEnumByName (char* aName)
{
   if (aName==0) return 0;

   char tName[100];
   strcpy(tName,aName);
   Ris::portableStrupr(tName);

   if (strcmp(tName,"TCPSERVER")==0)      return cRoleTcpServer;
   if (strcmp(tName,"TCPCLIENT")==0)      return cRoleTcpClient;
   if (strcmp(tName,"UDPPEER")==0)        return cRoleUdpPeer;

   return 0;
}

}//namespace
