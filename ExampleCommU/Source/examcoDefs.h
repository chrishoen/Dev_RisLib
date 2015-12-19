#ifndef _EXAMCODEFS_H_
#define _EXAMCODEFS_H_

/*==============================================================================
Description:
ExampleComm definitions
==============================================================================*/
namespace ExampleComm
{

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Definitions

   class Defs
   {
   public:
      static const int cRoleTcpServer = 1;
      static const int cRoleTcpClient = 2;
      static const int cRoleUdpPeer   = 3;

      static char*  getAppRoleNameByEnum(int aId);
      static int    getAppRoleEnumByName(char* aName);
   };

}//namespace

#endif

