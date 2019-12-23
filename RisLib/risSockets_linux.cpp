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
//******************************************************************************
//******************************************************************************
//******************************************************************************

bool isIpAddressValid(const char* aString)
{
   struct sockaddr_in sa;
   int result = inet_pton(AF_INET, aString, &(sa.sin_addr));
   return result != 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

SocketAddress::SocketAddress()
{
   reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SocketAddress::reset()
{
   mValid = false;
   mAddress = 0;
   strcpy(mString,"invalid");
   mPort = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SocketAddress::setByAddress(unsigned aAddress, int aPort)
{
   reset();
   mAddress = aAddress;
   struct in_addr tInAddr;
   tInAddr.s_addr = htonl(mAddress);
   strncpy(mString, inet_ntoa(tInAddr), 16);
   mValid = true;
   mPort = aPort;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SocketAddress::setByHostName(const char* aNode, int aPort)
{
   reset();

   unsigned tAddress;

   struct hostent* hostEnt = gethostbyname(aNode);

   if (hostEnt)
   {
      char* bytePtr = *hostEnt->h_addr_list;
      u_long* uintPtr = (u_long*)bytePtr;
      tAddress = ntohl(*uintPtr);
   }
   else
   {
      tAddress = 0;
   }
   setByAddress(tAddress, aPort);
   return;


   struct addrinfo hints;
   struct addrinfo *result;
   int s;

   char tPortString[20];
   sprintf(tPortString, "%d", aPort);

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_DGRAM;
   hints.ai_flags = (AI_V4MAPPED | AI_ADDRCONFIG | AI_NUMERICSERV);
   hints.ai_protocol = 0;

   s = getaddrinfo(aNode, tPortString, &hints, &result);
   if (s != 0)
   {
      printf("ERROR1 getaddrinfo: %s %s\n", aNode, gai_strerror(s));
      return;
   }

   sockaddr_in* tAddrIn = (sockaddr_in*)result->ai_addr;
   setByAddress(ntohl(tAddrIn->sin_addr.s_addr), ntohs(tAddrIn->sin_port));

   freeaddrinfo(result);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SocketAddress::setForAny(int aPort)
{
   setByAddress((unsigned)0,aPort);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SocketAddress::setForBroadcast(int aPort)
{
   setByAddress(0xFFFFFFFF,aPort);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool SocketAddress::isBroadcast()
{
   return mAddress == 0xFFFFFFFF;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool SocketAddress::isMulticast()
{
   SocketAddress tMulticastLo;
   SocketAddress tMulticastHi;
   tMulticastLo.setByHostName("224.0.0.0", 0);
   tMulticastHi.setByHostName("239.255.255.255", 0);
   return (tMulticastLo.mAddress <= mAddress) && (mAddress <= tMulticastHi.mAddress);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseSocket::BaseSpecific
{
public:
   int mDesc;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseSocket::BaseSocket()
{
   mBaseSpecific = new BaseSpecific;
   reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseSocket::~BaseSocket()
{
   doClose();
   reset();
   delete mBaseSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseSocket::reset()
{
   mStatus = 0;
   mError = 0;
   mBaseSpecific->mDesc = -1;
   mType = 0;
   mProtocol = 0;
   mLocal.reset();
   mRemote.reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::doClose()
{
   int tStatus = 0;

   if (mBaseSpecific->mDesc == -1) return false;
   // tStatus = close(mBaseSpecific->mDesc);
   tStatus = shutdown(mBaseSpecific->mDesc, SHUT_RDWR);
   mBaseSpecific->mDesc = -1;
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::updateError(int aStatus)
{
   if (mStatus < 0) return false;

   int tError;

   if (aStatus < 0)
   {
      tError = errno;
   }
   else
   {
      tError = 0;
   }

   mStatus = aStatus;
   mError = tError;

   return aStatus >= 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseSocket::setError(int aError)
{
   mStatus = -1;
   mError = aError;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseSocket::showError(const char* aLabel)
{
   if (mError == 666)
   {
      printf("SOCKET ERROR %s %d %d %s\n",
         aLabel, mStatus, mError, "bad remote address");
   }
   else if (mError == 667)
   {
      printf("SOCKET ERROR %s %d %d %s\n",
         aLabel, mStatus, mError, "bad local address");
   }
   else
   {
      printf("SOCKET ERROR %s %d %d %s\n",
         aLabel, mStatus, mError, strerror(mError));
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseSocket::testRemoteAddress(bool aTestForLocal)
{
   if (!mRemote.mValid)
   {
      setError(666);
      return;
   }

   if (aTestForLocal)
   {
      bool tFail = true;
      if ((mRemote.mAddress & 0xc0000000) == 0xc0000000) tFail = false;
      if ((mRemote.mAddress & 0x0a000000) == 0x0a000000) tFail = false;
      if (tFail) setError(666);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::setOptionBroadcast ()
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   bool bValue=true;
   tStatus = setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_BROADCAST,&bValue,sizeof(bool));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::setOptionReuseAddr ()
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   int tValue = 1;
   tStatus = setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_REUSEADDR,&tValue,sizeof(int));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::setOptionDontRoute()
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   int tValue = 1;
   tStatus = setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_DONTROUTE,&tValue,sizeof(int));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::setOptionDontLinger ()
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   int tValue = 1;
   tStatus = setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_LINGER,&tValue,sizeof(int));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::setOptionNoDelay ()
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   int tValue = 1;
// tStatus = setsockopt(mBaseSpecific->mDesc,IPPROTO_TCP,TCP_NODELAY,(char*)&tValue,sizeof(bool));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::setOptionMulticast(SocketAddress& aGroup, SocketAddress& aInterface)
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   struct ip_mreq tMreq;
   tMreq.imr_multiaddr.s_addr = htonl(aGroup.mAddress);
   tMreq.imr_interface.s_addr = htonl(aInterface.mAddress);

   tStatus = setsockopt(mBaseSpecific->mDesc, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&tMreq, sizeof(ip_mreq));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::ioctlBlocking(bool aBlocking)
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   if (!aBlocking)
   {
      tStatus = fcntl(mBaseSpecific->mDesc, F_SETFL, fcntl(mBaseSpecific->mDesc, F_GETFL) & ~O_NONBLOCK);
   }

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::ioctlFlush ()
{
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::ioctlGetBcastAddr (SocketAddress& aBcastAddr)
{
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::doSocket()
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   mType = SOCK_DGRAM;
   mProtocol = 0;
   mBaseSpecific->mDesc = socket(AF_INET,mType,mProtocol);

   if (mBaseSpecific->mDesc == -1) tStatus = -1;
   else                            tStatus =  0;

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket :: setMulticast()
{
   return false;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::doBind()
{
   if (mStatus < 0) return false;

   if (!mLocal.mValid)
   {
      setError(667);
      return false;
   }

   int tStatus = 0;
   sockaddr_in localName; memset(&localName, 0, sizeof(localName));
   localName.sin_family = AF_INET;
   localName.sin_addr.s_addr = htonl(mLocal.mAddress);
   localName.sin_port = htons(mLocal.mPort);

   tStatus = bind(mBaseSpecific->mDesc, (sockaddr*)&localName, sizeof(localName));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::doConnect()
{
   if (mStatus < 0) return false;

   if (!mRemote.mValid)
   {
      setError(666);
      return false;
   }

   int tStatus = 0;
   sockaddr_in hostName;memset(&hostName,0,sizeof(hostName));
   hostName.sin_family      = AF_INET;
   hostName.sin_addr.s_addr = htonl(mRemote.mAddress);
   hostName.sin_port        = htons(mRemote.mPort);

   tStatus = connect(mBaseSpecific->mDesc,(sockaddr*) &hostName,sizeof(hostName));

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::doSend(const char* aPayload,int& aLength)
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   if (aLength == 0) return true;

   tStatus = send(mBaseSpecific->mDesc,aPayload,aLength,0);

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::doRecv(char* aPayload,int& aLength,int aMaxLength)
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   if (aMaxLength == 0) return true;

   tStatus = recv(mBaseSpecific->mDesc,aPayload,aMaxLength,0);
   if(tStatus>0) aLength = tStatus;
   else          aLength = 0;

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::doSendTo(SocketAddress& aHost,const char* aPayload,int& aLength)
{
   if (mStatus < 0) return false;

   if (!aHost.mValid)
   {
      setError(666);
      return false;
   }

   int tStatus = 0;
   if (aLength == 0) return true;

   sockaddr_in destinAddr;
   int         destinAddrSize = sizeof(sockaddr_in);
   memset(&destinAddr,0,sizeof(destinAddr));
   destinAddr.sin_family      = AF_INET;
   destinAddr.sin_addr.s_addr = htonl(aHost.mAddress);
   destinAddr.sin_port        = htons(aHost.mPort);
   tStatus = sendto(mBaseSpecific->mDesc,aPayload,aLength,0,(sockaddr*)&destinAddr,sizeof(destinAddr));

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::doRecvFrom(SocketAddress& aHost,char* aPayload,int& aLength,int aMaxLength)
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   if (aMaxLength == 0) return true;

   sockaddr_in sender;
   int         senderSize=sizeof(sender);

   tStatus = (int)recvfrom(mBaseSpecific->mDesc,aPayload,aMaxLength,0,(struct sockaddr*)&sender,(socklen_t*)&senderSize);
   
   if (mBaseSpecific->mDesc == -1)
   {
      mStatus = 0;
      mError = 0;
      return false;
   }

   if(tStatus>0) aLength = tStatus;
   else          aLength = 0;

   aHost.setByAddress(ntohl(sender.sin_addr.s_addr), ntohs(sender.sin_port));

   return updateError(tStatus);
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseTcpServerHubSocket::Specific
{
public:
   fd_set  mReadSet;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseTcpServerHubSocket::BaseTcpServerHubSocket()
{
   mSpecific = new Specific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseTcpServerHubSocket::~BaseTcpServerHubSocket()
{
   delete mSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpServerHubSocket::doSocket()
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   mType = SOCK_STREAM;
   mProtocol = 0;
   mBaseSpecific->mDesc = socket(AF_INET,mType,mProtocol);

   if (mBaseSpecific->mDesc == -1) tStatus = -1;
   else                            tStatus =  0;

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpServerHubSocket::doListen()
{
   if (mStatus < 0) return false;

   int tStatus = 0;

   tStatus = listen(mBaseSpecific->mDesc,SOMAXCONN);

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpServerHubSocket::doAccept(BaseTcpStreamSocket& aStream)
{
   if (mStatus < 0) return false;

   int tStatus = 0;

   sockaddr tempName;
   int tempNameLength;
   sockaddr_in clientName;
   aStream.reset();

   int desc;
   tempNameLength = sizeof(tempName);
   desc = accept(mBaseSpecific->mDesc, &tempName, (socklen_t*)&tempNameLength);
   memmove(&clientName, &tempName, sizeof(tempName));
   if (desc != -1)
   {
      tStatus = 0;
      aStream.mBaseSpecific->mDesc = desc;
      aStream.mLocal = mLocal;
      aStream.mType = SOCK_STREAM;
      aStream.mRemote.setByAddress(ntohl(clientName.sin_addr.s_addr), ntohs(clientName.sin_port));
   }
   else
   {
      tStatus = -1;
   }

   return updateError(tStatus);
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseTcpServerHubSocket::resetReadSet()
{
   FD_ZERO(&mSpecific->mReadSet);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseTcpServerHubSocket::addSelfToReadSet()
{
   FD_SET(mBaseSpecific->mDesc,&mSpecific->mReadSet);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseTcpServerHubSocket::addToReadSet(BaseTcpStreamSocket* aStream)
{
   FD_SET(aStream->mBaseSpecific->mDesc,&mSpecific->mReadSet);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpServerHubSocket::isSelfInReadSet()
{
   return 0!=FD_ISSET(mBaseSpecific->mDesc,&mSpecific->mReadSet);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpServerHubSocket::isInReadSet(BaseTcpStreamSocket* aStream)
{
   return 0!=FD_ISSET(aStream->mBaseSpecific->mDesc,&mSpecific->mReadSet);
}

//******************************************************************************
//******************************************************************************
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
//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpStreamSocket::doSocket()
{
   if (mStatus < 0) return false;

   int tStatus = 0;

   mType = SOCK_STREAM;
   mProtocol = 0;
   mBaseSpecific->mDesc = socket(AF_INET,mType,mProtocol);

   if (mBaseSpecific->mDesc == -1) tStatus = -1;
   else                                  tStatus =  0;

   setOptionDontLinger();

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpStreamSocket::setOptionKeepAlive ()
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   bool bValue=true;
   tStatus=setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_KEEPALIVE,(char*)&bValue,sizeof(bool));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpStreamSocket::doConnect()
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   sockaddr_in hostName;memset(&hostName,0,sizeof(hostName));
   hostName.sin_family      = AF_INET;
   hostName.sin_addr.s_addr = htonl(mRemote.mAddress);
   hostName.sin_port        = htons(mRemote.mPort);
   tStatus = connect(mBaseSpecific->mDesc,(sockaddr*) &hostName,sizeof(hostName));

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpStreamSocket::doSend(const char* aPayload,int aLength)
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   if (aLength == 0) return true;

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
//******************************************************************************
//******************************************************************************

bool BaseTcpStreamSocket::doRecv(char* aPayload,int aLength,int& aStatus)
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   if (aLength == 0) return true;

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
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

