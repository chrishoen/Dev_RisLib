#ifndef _EXAMPLEDEFS_H_
#define _EXAMPLEDEFS_H_

/*==============================================================================
Description:
Example definitions
==============================================================================*/
namespace Example
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

