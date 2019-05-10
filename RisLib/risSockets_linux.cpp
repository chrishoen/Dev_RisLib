/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
extern int h_errno;

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "risSockets.h"

namespace Ris
{
namespace Sockets
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

IpAddress :: IpAddress()
{
   reset();
}

//******************************************************************************

void IpAddress::reset()
{
   mValid = false;
   mValue = 0;
   strncpy(mString,"",16);
}

//******************************************************************************

void IpAddress::set(const char* aAddress)
{
   reset();
   struct in_addr tInAddr;
   if (inet_aton(aAddress, &tInAddr)==0) return;
   mValue = ntohl(tInAddr.s_addr);
   strncpy(mString,inet_ntoa(tInAddr),16);
   mValid = true;
}

//******************************************************************************

void IpAddress::set(unsigned aAddress)
{
   reset();
   mValue = aAddress;
   struct in_addr tInAddr;
   tInAddr.s_addr = htonl(mValue);
   strncpy(mString, inet_ntoa(tInAddr), 16);
   mValid = true;
}

//******************************************************************************

void IpAddress::setForBroadcast()
{
   set(0xFFFFFFFF);
}

//******************************************************************************

bool IpAddress::isBroadcast()
{
   return mValue==0xFFFFFFFF;
}

//******************************************************************************

bool IpAddress::isMulticast()
{
   IpAddress tMulticastLo;
   IpAddress tMulticastHi;
   tMulticastLo.set("224.0.0.0");
   tMulticastHi.set("239.255.255.255");

   return (tMulticastLo.mValue <= mValue) && (mValue <= tMulticastHi.mValue);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

SocketAddress::SocketAddress()
{
   reset();
}

//******************************************************************************

void SocketAddress::reset()
{
   mIpAddr.reset();
   mPort = 1024;
}

//******************************************************************************

void SocketAddress::set(const char* aIpAddr,int aPort)
{
   mIpAddr.set(aIpAddr);
   mPort = aPort;
}

//******************************************************************************

void SocketAddress::set(IpAddress aIpAddr,int aPort)
{
   mIpAddr = aIpAddr;
   mPort   = aPort;
}

//******************************************************************************

void SocketAddress::setByHostName(const char* aNode, int aPort)
{
   struct addrinfo hints;
   struct addrinfo *result, *rp;
   int s;

   char tPortString[20];
   sprintf(tPortString, "%d", aPort);

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
   hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
   hints.ai_flags = (AI_V4MAPPED | AI_ADDRCONFIG | AI_NUMERICSERV);
   hints.ai_protocol = 0;          /* Any protocol */

// s = getaddrinfo(aCmd->argString(1), NULL, NULL, &result);
// s = getaddrinfo(aCmd->argString(1), NULL, &hints, &result);
   s = getaddrinfo(aNode, tPortString, &hints, &result);
   if (s != 0)
   {
      printf("ERROR1 getaddrinfo: %s %s\n", aNode, gai_strerror(s));
      return;
   }

   for (rp = result; rp != NULL; rp = rp->ai_next)
   {
      printf("DATA****************************\n");

      sockaddr_in* tSockAddrIn = (sockaddr_in*)rp->ai_addr;
      struct in_addr tAddr = tSockAddrIn->sin_addr;
      unsigned tValue = ntohl(tAddr.s_addr);
      unsigned tPort = ntohs(tSockAddrIn->sin_port);

      printf("ai_addr value    %x\n", tValue);
      printf("ai_addr port     %d\n", tPort);

      Ris::Sockets::IpAddress tA1;
      tA1.set(tValue);
      printf("A1         %s\n", tA1.mString);

      mIpAddr.set(tValue);
      mPort = tPort;
   }

   freeaddrinfo(result);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseSocket::BaseSpecific
{
public:
   int mDesc;
};

//******************************************************************************

BaseSocket::BaseSocket()
{
   mBaseSpecific = new BaseSpecific;
   reset();
}

//******************************************************************************

BaseSocket::~BaseSocket()
{
   doClose();
   reset();
   delete mBaseSpecific;
}

//******************************************************************************

void BaseSocket::reset()
{
   mBaseSpecific->mDesc = -1;
   mStatus       = 0;
   mError        = 0;
   mType         = 0;
   mProtocol     = 0;
   mLocal.reset();
   mRemote.reset();
}

//******************************************************************************

bool BaseSocket::doBind()
{
   int tStatus=0;

   sockaddr_in localName;memset(&localName,0,sizeof(localName));
   localName.sin_family      = AF_INET;
   localName.sin_addr.s_addr = inet_addr(mLocal.mIpAddr.mString);
   localName.sin_port        = htons(mLocal.mPort);

   tStatus = bind(mBaseSpecific->mDesc,(sockaddr*) &localName,sizeof(localName));
   return updateError(tStatus);
}

//******************************************************************************

bool BaseSocket::doClose()
{
   int tStatus=0;

   if (mBaseSpecific->mDesc == -1) return false;
// tStatus = close(mBaseSpecific->mDesc);
   tStatus = shutdown(mBaseSpecific->mDesc, SHUT_RDWR);
   mBaseSpecific->mDesc = -1;
   return updateError(tStatus);
}

//******************************************************************************

bool BaseSocket::updateError(int aStatus)
{
   int tError;

   if(aStatus < 0)
   {
      tError = errno;
   }
   else
   {
      tError = 0;
   }

   mStatus = aStatus;
   mError  = tError;

   return aStatus >= 0;
}

//******************************************************************************

bool BaseSocket::setOptionBroadcast ()
{
   int tStatus=0;

   bool bValue=true;
   tStatus=setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_BROADCAST,(char*)&bValue,sizeof(bool));
   return updateError(tStatus);
}

//******************************************************************************

bool BaseSocket::setOptionReuseAddr ()
{
   int tStatus=0;

   bool bValue=true;
   tStatus=setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_REUSEADDR,(char*)&bValue,sizeof(bool));
   return updateError(tStatus);
}

//******************************************************************************

bool BaseSocket::setOptionDontRoute ()
{
   int tStatus=0;

   bool bValue=true;
   tStatus=setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_DONTROUTE,(char*)&bValue,sizeof(bool));
   return updateError(tStatus);
}

//******************************************************************************

bool BaseSocket::setOptionDontLinger ()
{
   int tStatus=0;

   bool bValue=false;
   tStatus=setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_LINGER,(char*)&bValue,sizeof(bool));
   return updateError(tStatus);
}

//******************************************************************************

bool BaseSocket::setOptionNoDelay ()
{
   int tStatus=0;

// bool bValue=true;
// tStatus=setsockopt(mBaseSpecific->mDesc,IPPROTO_TCP,TCP_NODELAY,(char*)&bValue,sizeof(bool));
   return updateError(tStatus);
}

//******************************************************************************

bool BaseUdpSocket::setOptionMulticast(IpAddress& aGroup,IpAddress& aInterface)
{
   int tStatus=0;

   struct ip_mreq tMreq;
   tMreq.imr_multiaddr.s_addr = htonl(aGroup.mValue); 
   tMreq.imr_interface.s_addr = htonl(aInterface.mValue); 

   tStatus=setsockopt(mBaseSpecific->mDesc,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&tMreq,sizeof(ip_mreq));
   return updateError(tStatus);
}

//******************************************************************************

bool BaseSocket::ioctlBlocking(bool aBlocking)
{
   int tStatus=0;

   if (!aBlocking)
   {
      tStatus = fcntl(mBaseSpecific->mDesc, F_SETFL, fcntl(mBaseSpecific->mDesc, F_GETFL) & ~O_NONBLOCK);
   }

   return updateError(tStatus);
}

//******************************************************************************

bool BaseSocket::ioctlFlush ()
{
   return false;
}

//******************************************************************************

bool BaseSocket::ioctlGetBcastAddr (IpAddress& aBcastAddr)
{
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::doSocket()
{
   int tStatus=0;

   mType = SOCK_DGRAM;
   mProtocol = 0;
   mBaseSpecific->mDesc = socket(AF_INET,mType,mProtocol);

   if (mBaseSpecific->mDesc == -1) tStatus = -1;
   else                                  tStatus =  0;

   return updateError(tStatus);
}

//******************************************************************************

bool BaseUdpSocket :: setMulticast()
{
   return false;
}


//******************************************************************************

bool BaseUdpSocket::doConnect()
{
   int tStatus=0;

   sockaddr_in hostName;memset(&hostName,0,sizeof(hostName));
   hostName.sin_family      = AF_INET;
   hostName.sin_addr.s_addr = inet_addr(mRemote.mIpAddr.mString);
   hostName.sin_port        = htons(mRemote.mPort);

   tStatus = connect(mBaseSpecific->mDesc,(sockaddr*) &hostName,sizeof(hostName));

   return updateError(tStatus);
}

//******************************************************************************

bool BaseUdpSocket::doSend(const char* aPayload,int& aLength)
{
   int tStatus=0;

   if (aLength==0) return true;

   tStatus = send(mBaseSpecific->mDesc,aPayload,aLength,0);

   return updateError(tStatus);
}

//******************************************************************************

bool BaseUdpSocket::doRecv(char* aPayload,int& aLength,int aMaxLength)
{
   int tStatus=0;

   if (aMaxLength==0) return true;

   tStatus = recv(mBaseSpecific->mDesc,aPayload,aMaxLength,0);
   if(tStatus>0) aLength = tStatus;
   else          aLength = 0;

   return updateError(tStatus);
}

//******************************************************************************

bool BaseUdpSocket::doSendTo(SocketAddress& aHost,const char* aPayload,int& aLength)
{
   int tStatus=0;

   if (aLength==0) return true;

   sockaddr_in destinAddr;
   int         destinAddrSize = sizeof(sockaddr_in);
   memset(&destinAddr,0,sizeof(destinAddr));
   destinAddr.sin_family      = AF_INET;
   destinAddr.sin_addr.s_addr = inet_addr(aHost.mIpAddr.mString);
   destinAddr.sin_port        = htons(aHost.mPort);
   tStatus = sendto(mBaseSpecific->mDesc,aPayload,aLength,0,(sockaddr*)&destinAddr,sizeof(destinAddr));

   return updateError(tStatus);
}

//******************************************************************************

bool BaseUdpSocket::doRecvFrom(SocketAddress& aHost,char* aPayload,int& aLength,int aMaxLength)
{
   int tStatus=0;

   if (aMaxLength==0) return true;

   sockaddr_in sender;
   int         senderSize=sizeof(sender);

   tStatus = (int)recvfrom(mBaseSpecific->mDesc,aPayload,aMaxLength,0,(struct sockaddr*)&sender,(socklen_t*)&senderSize);
   
   if(tStatus>0) aLength = tStatus;
   else          aLength = 0;

   aHost.mIpAddr.set(inet_ntoa(sender.sin_addr));
   aHost.mPort = sender.sin_port;

   return updateError(tStatus);
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseTcpServerHubSocket::Specific
{
public:
   fd_set  mReadSet;
};

//******************************************************************************

BaseTcpServerHubSocket::BaseTcpServerHubSocket()
{
   mSpecific = new Specific;
}

//******************************************************************************

BaseTcpServerHubSocket::~BaseTcpServerHubSocket()
{
   delete mSpecific;
}

//******************************************************************************

bool BaseTcpServerHubSocket::doSocket()
{
   int tStatus=0;

   mType = SOCK_STREAM;
   mProtocol = 0;
   mBaseSpecific->mDesc = socket(AF_INET,mType,mProtocol);

   if (mBaseSpecific->mDesc == -1) tStatus = -1;
   else                                  tStatus =  0;

   return updateError(tStatus);
}

//******************************************************************************

bool BaseTcpServerHubSocket::doListen()
{
   int tStatus=0;

   tStatus = listen(mBaseSpecific->mDesc,SOMAXCONN);

   return updateError(tStatus);
}

//******************************************************************************

bool BaseTcpServerHubSocket::doAccept(BaseTcpStreamSocket& aStream)
{
   int tStatus=0;

   sockaddr tempName;
   int tempNameLength;
   sockaddr_in clientName;
   aStream.reset();

   int desc;
   tempNameLength = sizeof(tempName);
   desc = accept(mBaseSpecific->mDesc,&tempName,(socklen_t*)&tempNameLength);
   memmove(&clientName,&tempName,sizeof(tempName));
   if(desc != -1)
   {
       tStatus=0;
       aStream.mBaseSpecific->mDesc = desc;
       aStream.mLocal = mLocal;
       aStream.mType  = SOCK_STREAM;
       aStream.mRemote.mIpAddr.set((int)clientName.sin_addr.s_addr);
       aStream.mRemote.mPort = (int)clientName.sin_port;
   }
   else
   {
     tStatus = -1;
   }

   return updateError(tStatus);
}

//******************************************************************************

void BaseTcpServerHubSocket::resetReadSet()
{
   FD_ZERO(&mSpecific->mReadSet);
}

//******************************************************************************

void BaseTcpServerHubSocket::addSelfToReadSet()
{
   FD_SET(mBaseSpecific->mDesc,&mSpecific->mReadSet);
}

//******************************************************************************

void BaseTcpServerHubSocket::addToReadSet(BaseTcpStreamSocket* aStream)
{
   FD_SET(aStream->mBaseSpecific->mDesc,&mSpecific->mReadSet);
}

//******************************************************************************

bool BaseTcpServerHubSocket::isSelfInReadSet()
{
   return 0!=FD_ISSET(mBaseSpecific->mDesc,&mSpecific->mReadSet);
}

//******************************************************************************

bool BaseTcpServerHubSocket::isInReadSet(BaseTcpStreamSocket* aStream)
{
   return 0!=FD_ISSET(aStream->mBaseSpecific->mDesc,&mSpecific->mReadSet);
}

//******************************************************************************

int BaseTcpServerHubSocket::selectOnReadSet()
{
   timeval tTimeout;
   tTimeout.tv_sec=0;
   tTimeout.tv_usec=500000;

   return select(FD_SETSIZE,&mSpecific->mReadSet,0,0,&tTimeout);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpStreamSocket::doSocket()
{
   int tStatus=0;

   mType = SOCK_STREAM;
   mProtocol = 0;
   mBaseSpecific->mDesc = socket(AF_INET,mType,mProtocol);

   if (mBaseSpecific->mDesc == -1) tStatus = -1;
   else                                  tStatus =  0;

   return updateError(tStatus);
}

//******************************************************************************

bool BaseTcpStreamSocket::setOptionKeepAlive ()
{
   int tStatus=0;

   bool bValue=true;
   tStatus=setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_KEEPALIVE,(char*)&bValue,sizeof(bool));
   return updateError(tStatus);
}

//******************************************************************************

bool BaseTcpStreamSocket::doConnect()
{
   int tStatus=0;

   sockaddr_in hostName;memset(&hostName,0,sizeof(hostName));
   hostName.sin_family      = AF_INET;
   hostName.sin_addr.s_addr = inet_addr(mRemote.mIpAddr.mString);
   hostName.sin_port        = htons(mRemote.mPort);
   tStatus = connect(mBaseSpecific->mDesc,(sockaddr*) &hostName,sizeof(hostName));

   return updateError(tStatus);
}

//******************************************************************************

bool BaseTcpStreamSocket::doSend(const char* aPayload,int aLength)
{
   int tStatus=0;

   if (aLength==0) return true;

   int bytesRequired=aLength;
   int bytesRemaining=aLength;
   int bytesTotal=0;

   bool going=true;
   while(going)
   {
      tStatus=send(mBaseSpecific->mDesc,&aPayload[bytesTotal],bytesRemaining,0);
      if(tStatus>0)
      {
         bytesTotal     += tStatus;
         bytesRemaining -= tStatus;
         if (bytesTotal==bytesRequired)
         {
            going=false;
            tStatus=bytesTotal;
         }
      }
      else
      {
         going=false;
      }
   }

   return updateError(tStatus);
}

//******************************************************************************

bool BaseTcpStreamSocket::doRecv(char* aPayload,int aLength,int& aStatus)
{
   int tStatus=0;

   if (aLength==0) return true;

   int bytesRequired=aLength;
   int bytesRemaining=aLength;
   int bytesTotal=0;

   bool going=true;
   while(going)
   {
      tStatus=recv(mBaseSpecific->mDesc,&aPayload[bytesTotal],bytesRemaining,0);
      if(tStatus>0)
      {
         bytesTotal     += tStatus;
         bytesRemaining -= tStatus;
         if (bytesTotal==bytesRequired)
         {
            going=false;
            tStatus=bytesTotal;
         }
      }
      else
      {
         going=false;
      }
   }
   aStatus=tStatus;
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* memAlloc(int aSize)
{
   return (char*)malloc(aSize);
}
void  memFree(char* aMem)
{
   free(aMem);
}

//******************************************************************************
}//namespace
}//namespace

