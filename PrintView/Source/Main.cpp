#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <stdio.h>

#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"

//******************************************************************************
int main(int argc,char** argv)
{
   int tPort = Ris::Net::PortDef::cPrintView;

   if (argc > 1)
   {
      tPort = atoi(argv[1]);
   }

   Ris::Net::UdpRxStringSocket tSocket;
   tSocket.configure(tPort);

   while (tSocket.doRecvString())
   {
      puts(tSocket.mRxString);
   }
   return 0;
}

