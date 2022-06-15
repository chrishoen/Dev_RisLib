#include "stdafx.h"

#include "risNetPortDef.h"
#include "risNetUdpStringSocket.h"

//******************************************************************************

Ris::Net::UdpStringSocket rSocket;
Ris::Net::Settings rSettings;

int main(int argc,char** argv)
{
   int tPort = Ris::Net::PortDef::cPrintView;

   if (argc > 1)
   {
      tPort = atoi(argv[1]);
   }

   rSettings.mLocalIpPort = tPort;
   rSocket.initialize(rSettings);
   rSocket.mPrintDisable = true;
   rSocket.configure();

   while (rSocket.doRecvString())
   {
      puts(rSocket.mRxString);
   }
   return 0;
}

