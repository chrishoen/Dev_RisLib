#include "stdafx.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <assert.h>

#include "risSockets.h"
#include "risNetUdpStringSocket.h"
#include "risNetSettings.h"

#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);
   if (aCmd->isCmd("GET"))     executeGetAddr(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGetAddr(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, "192.168.1.10");

   struct addrinfo hints;
   struct addrinfo *result, *rp;
   int s;

   /* Obtain address(es) matching host/port */

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
   hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
   hints.ai_flags = (AI_V4MAPPED | AI_ADDRCONFIG | AI_CANONNAME | AI_NUMERICSERV);
   hints.ai_protocol = 0;          /* Any protocol */

// s = getaddrinfo(aCmd->argString(1), NULL, NULL, &result);
// s = getaddrinfo(aCmd->argString(1), NULL, &hints, &result);
   s = getaddrinfo(aCmd->argString(1), "56001", &hints, &result);
   if (s != 0)
   {
      printf("ERROR1 getaddrinfo: %s\n", gai_strerror(s));
      return;
   }

   /* getaddrinfo() returns a list of address structures.
      Try each address until we successfully connect(2).
      If socket(2) (or connect(2)) fails, we (close the socket
      and) try the next address. */

   for (rp = result; rp != NULL; rp = rp->ai_next)
   {
      printf("DATA****************************\n");
      printf("ai_canonname %s\n", rp->ai_canonname);

      sockaddr_in* tSockAddrIn = (sockaddr_in*)rp->ai_addr;
      struct in_addr tAddr = tSockAddrIn->sin_addr;
      unsigned tValue = ntohl(tAddr.s_addr);
      unsigned tPort = ntohs(tSockAddrIn->sin_port);

      printf("ai_addr value    %x\n", tValue);
      printf("ai_addr port     %d\n", tPort);

      Ris::Sockets::SocketAddress tA1;
      tA1.setByAddress(tValue,tPort);
      printf("A1         %s\n", tA1.mString);
   }

   freeaddrinfo(result);           /* No longer needed */
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, "sms3");
   Ris::Net::UdpStringSocket tSocket;
   Ris::Net::Settings tSettings;

   tSettings.mTestForLocal = true;
   tSettings.setRemoteAddress(aCmd->argString(1), 58001);

   tSocket.initialize(tSettings);
   tSocket.configure();

   if (!tSocket.mValidFlag)
   {
      tSocket.showError("initializePrint");
   }

   Prn::print(0, "tSocket");
   Prn::print(0, "valid    %s", my_string_from_bool(tSocket.mValidFlag));
   Prn::print(0, "status   %d", tSocket.mStatus);
   Prn::print(0, "error    %d", tSocket.mError);
   Prn::print(0, "valid    %s", my_string_from_bool(tSocket.mValidFlag));
   Prn::print(0, "address  %x", tSocket.mRemote.mAddress);
   Prn::print(0, "port     %d", tSocket.mRemote.mPort);
   Prn::print(0, "string   %s", tSocket.mRemote.mString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

