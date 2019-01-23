#include "stdafx.h"

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
   tSocket.configureLocal(tPort);
// tSocket.show();

   while (tSocket.doRecvString())
   {
      puts(tSocket.mRxString);
      if(strcmp("PRINTVIEW_SHUTDOWN",tSocket.mRxString)==0) return 0;
   }
   return 0;
}

