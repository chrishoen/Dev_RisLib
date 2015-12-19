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
char* getAppRoleNameByEnum(int aId)
{
   switch (aId)
   {
      case AR_TcpServer  :  return "TcpServer";   break;
      case AR_TcpClient  :  return "TcpClient";   break;
      case AR_UdpPeer    :  return "UdpPeer";     break;
   }
   return  "UNKNOWN";

};
//******************************************************************************
int getAppRoleEnumByName (char* aName)
{
   if (aName==0) return 0;

   char tName[100];
   strcpy(tName,aName);
   Ris::portableStrupr(tName);

   if (strcmp(tName,"TCPSERVER")==0)      return AR_TcpServer;
   if (strcmp(tName,"TCPCLIENT")==0)      return AR_TcpClient;
   if (strcmp(tName,"UDPPEER")==0)        return AR_UdpPeer;

   return 0;
}

}//namespace
