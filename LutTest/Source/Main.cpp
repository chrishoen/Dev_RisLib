#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <stdio.h>

#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"

//******************************************************************************
int main(int argc,char** argv)
{
   Ris::Net::UdpRxStringSocket tSocket;
   tSocket.configure(Ris::Net::PortDef::cPrintView);

   while (tSocket.doRecvString())
   {
      puts(tSocket.mRxString);
   }
   return 0;
}

