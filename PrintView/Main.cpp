#include "stdafx.h"

#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"

//******************************************************************************

Ris::Net::UdpRxStringSocket rSocket;

int main(int argc,char** argv)
{
   int tPort = Ris::Net::PortDef::cPrintView;

   if (argc > 1)
   {
      tPort = atoi(argv[1]);
   }

   rSocket.configureLocal(tPort);

   while (rSocket.doRecvString())
   {
      puts(rSocket.mRxString);
   }
   return 0;
}

