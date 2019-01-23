/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <assert.h>

#include "risSockets.h"

#pragma unmanaged

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

IpAddress::IpAddress()
{
   reset();
}

IpAddress::IpAddress(char* aAddress)
{
   set(aAddress);
}

IpAddress::IpAddress(int   aAddress)
{
   set(aAddress);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void IpAddress::reset()
{
   mValue = 0;
   strncpy(mString,"",16);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void IpAddress::set(char* aAddress)
{
   setByHostName(aAddress);
   return;

   if(strlen(aAddress)>16) return;
   mValue = ntohl(inet_addr(aAddress));
   strncpy(mString,aAddress,16);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void IpAddress::set(int aAddress)
{
   mValue = aAddress;

   struct in_addr inAddr;
   inAddr.s_addr = htonl(mValue);
   strcpy(mString,inet_ntoa(inAddr));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void IpAddress::setByHostLocal()
{
   char name[200];
   mValue = gethostname(name,200);
   setByHostName(name);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void IpAddress::setByHostName(char* aName)
{
   struct hostent* hostEnt = gethostbyname(aName);

   if(hostEnt)
   {
      char* bytePtr = *hostEnt->h_addr_list;
      u_long* uintPtr = (u_long*)bytePtr;
      mValue = ntohl(*uintPtr);
   }
   else
   {
      mValue = 0;
   }
   struct in_addr inAddr;
   inAddr.s_addr = htonl(mValue);
   strcpy(mString,inet_ntoa(inAddr));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void IpAddress::setForBroadcast()
{
   set(0xFFFFFFFF);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool IpAddress::isBroadcast()
{
   return mValue==0xFFFFFFFF;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool IpAddress::isMulticast()
{
   IpAddress tMulticastLo("224.0.0.0");
   IpAddress tMulticastHi("239.255.255.255");

   return (tMulticastLo.mValue <= mValue) && (mValue <= tMulticastHi.mValue);
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

SocketAddress::SocketAddress(char* aIpAddr,int aPort)
{
   mIpAddr.set(aIpAddr);
   mPort = aPort;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

SocketAddress::SocketAddress(IpAddress aIpAddr,int aPort)
{
   mIpAddr = aIpAddr;
   mPort = aPort;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SocketAddress::reset()
{
   mIpAddr.reset();
   mPort = 1024;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SocketAddress::set(char* aIpAddr,int aPort)
{
   mIpAddr.set(aIpAddr);
   mPort = aPort;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void SocketAddress::set(IpAddress aIpAddr,int aPort)
{
   mIpAddr = aIpAddr;
   mPort   = aPort;
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
   SOCKET mDesc;
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
   mStatus       = 0;
   mError        = 0;
   mBaseSpecific->mDesc         = -1;
   mType         = 0;
   mProtocol     = 0;
   mLocal.reset();
   mRemote.reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::doClose()
{
   int tStatus=0;

   tStatus = closesocket(mBaseSpecific->mDesc);
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::updateError(int aStatus)
{
   int tError;

   if(aStatus < 0)
   {
      switch(tError = WSAGetLastError())
      {
         case WSA_IO_PENDING:
            tError = 0;
            aStatus = 0;
            break;
         default:
            break;
      }
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
//******************************************************************************
//******************************************************************************

bool BaseSocket::setOptionBroadcast ()
{
   int tStatus=0;

   BOOL bValue=true;
   tStatus=setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_BROADCAST,(char*)&bValue,sizeof(BOOL));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::setOptionReuseAddr ()
{
   int tStatus=0;

   BOOL bValue=true;
   tStatus=setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_REUSEADDR,(char*)&bValue,sizeof(BOOL));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::setOptionDontRoute ()
{
   int tStatus=0;

   BOOL bValue=true;
   tStatus=setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_DONTROUTE,(char*)&bValue,sizeof(BOOL));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::setOptionDontLinger ()
{
   int tStatus=0;

   BOOL bValue=true;
   tStatus=setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_DONTLINGER,(char*)&bValue,sizeof(BOOL));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::setOptionNoDelay ()
{
   int tStatus=0;

   BOOL bValue=true;
   tStatus=setsockopt(mBaseSpecific->mDesc,IPPROTO_TCP,TCP_NODELAY,(char*)&bValue,sizeof(BOOL));
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
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
//******************************************************************************
//******************************************************************************

bool BaseSocket::ioctlBlocking(bool aBlocking)
{
   int tStatus=0;

   u_long value;
   if (aBlocking) value=0;
   else           value=1;
   tStatus=ioctlsocket(mBaseSpecific->mDesc,FIONBIO,&value);

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::ioctlFlush ()
{
   int tStatus=0;

   tStatus=ioctlsocket(mBaseSpecific->mDesc,SIO_FLUSH,NULL);
   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::ioctlGetBcastAddr (IpAddress& aBcastAddr)
{
   int tStatus=0;

   sockaddr_in outputAddr;
   DWORD       outputAddrSize = sizeof(sockaddr_in);
   memset(&outputAddr,0,sizeof(outputAddr));
   DWORD bytesReturned;

   tStatus=WSAIoctl(
      mBaseSpecific->mDesc,
      SIO_GET_BROADCAST_ADDRESS,
      NULL,
      0,
      (LPVOID)&outputAddr,
      outputAddrSize,
      (LPDWORD)&bytesReturned,
      NULL,
      NULL);

   aBcastAddr.reset();
   aBcastAddr.set(outputAddr.sin_addr.s_addr);

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::doSocket()
{
   int tStatus=0;

   mType = SOCK_DGRAM;
   mProtocol = 0;
   DWORD dwFlags = WSA_FLAG_OVERLAPPED;
   mBaseSpecific->mDesc = WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,dwFlags);

   if (mBaseSpecific->mDesc==INVALID_SOCKET) tStatus = -1;
   else                       tStatus =  0;

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket :: setMulticast()
{
   assert(0);
   return false;
}

//******************************************************************************
//******************************************************************************
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
//******************************************************************************
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
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::doSend(char* aPayload,int& aLength)
{
   int tStatus=0;

   if (aLength==0) return true;

   tStatus = send(mBaseSpecific->mDesc,aPayload,aLength,0);

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
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
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::doSendTo(SocketAddress& aHost,char* aPayload,int& aLength)
{
   int tStatus=0;

   if (aLength==0) return true;

   sockaddr_in destinAddr;
   int         destinAddrSize = sizeof(sockaddr_in);
   memset(&destinAddr,0,sizeof(destinAddr));
   destinAddr.sin_family      = AF_INET;
   destinAddr.sin_addr.s_addr = inet_addr(aHost.mIpAddr.mString);
   destinAddr.sin_port        = htons(aHost.mPort);
   tStatus = sendto(mBaseSpecific->mDesc,aPayload,aLength,0,(SOCKADDR*)&destinAddr,sizeof(destinAddr));

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseUdpSocket::doRecvFrom(SocketAddress& aHost,char* aPayload,int& aLength,int aMaxLength)
{
   int tStatus=0;

   if (aMaxLength==0) return true;

   sockaddr_in sender;
   int         senderSize=sizeof(sender);

   tStatus = recvfrom(mBaseSpecific->mDesc,aPayload,aMaxLength,0,(SOCKADDR*)&sender,&senderSize);
   if(tStatus>0) aLength = tStatus;
   else          aLength = 0;

   aHost.mIpAddr.set(inet_ntoa(sender.sin_addr));
   aHost.mPort = sender.sin_port;

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
   int tStatus=0;

   mType = SOCK_STREAM;
   mProtocol = 0;
   mBaseSpecific->mDesc = socket(AF_INET,SOCK_STREAM,0);

   if (mBaseSpecific->mDesc==INVALID_SOCKET) tStatus = -1;
   else                       tStatus =  0;

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpServerHubSocket::doListen()
{
   int tStatus=0;

   tStatus = listen(mBaseSpecific->mDesc,SOMAXCONN);

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpServerHubSocket::doAccept(BaseTcpStreamSocket& aStream)
{
   int tStatus=0;

   sockaddr tempName;
   int tempNameLength;
   sockaddr_in clientName;
   aStream.reset();

   SOCKET desc;
   tempNameLength = sizeof(tempName);
   desc = accept(mBaseSpecific->mDesc,&tempName,&tempNameLength);
   memmove(&clientName,&tempName,sizeof(tempName));
   if(desc != INVALID_SOCKET)
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
   int tStatus=0;

   mType = SOCK_STREAM;
   mProtocol = 0;
   mBaseSpecific->mDesc = socket(AF_INET,SOCK_STREAM,0);

   if (mBaseSpecific->mDesc==INVALID_SOCKET) tStatus = -1;
   else                       tStatus =  0;

   setOptionDontLinger();

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpStreamSocket::setOptionKeepAlive ()
{
   int tStatus=0;

   BOOL bValue=true;
   tStatus=setsockopt(mBaseSpecific->mDesc,SOL_SOCKET,SO_KEEPALIVE,(char*)&bValue,sizeof(BOOL));

   DWORD dwBytes;
   tcp_keepalive keepalive1;
   tcp_keepalive keepalive2;
   keepalive1.onoff=1;
   keepalive1.keepalivetime=1000;
   keepalive1.keepaliveinterval=1000;

   tStatus = WSAIoctl(
      mBaseSpecific->mDesc,
      SIO_KEEPALIVE_VALS,
      &keepalive1,
      sizeof(tcp_keepalive),
      &keepalive2,
      sizeof(tcp_keepalive),
      &dwBytes,
      0,
      0);

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
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
//******************************************************************************
//******************************************************************************

bool BaseTcpStreamSocket::doSend(char* aPayload,int aLength)
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
//******************************************************************************
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
//******************************************************************************
//******************************************************************************
//******************************************************************************

char* memAlloc(int aSize)
{
   return (char*)GlobalAlloc(0,aSize);
}
void  memFree(char* aMem)
{
   GlobalFree(aMem);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

class SocketInitClass
{
public:
   SocketInitClass()
   {
      WORD    Version = MAKEWORD(2,2);
      WSADATA Data;

      WSAStartup(Version,&Data);
   }
   ~SocketInitClass()
   {
      WSACleanup();
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

SocketInitClass gSocketInitInstance;

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

