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

#include "risBtSockets.h"

#pragma unmanaged

namespace Ris
{
namespace BtSockets
{

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
   strcpy(mString, "invalid");
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

void SocketAddress::setByAddress(const char* aString, int aPort)
{
   reset();
   if (strlen(aString) > 16) return;
   unsigned int tAddress = ntohl(inet_addr(aString));
   setByAddress(tAddress, aPort);
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

bool BaseSocket::doBind()
{
   if (mStatus < 0) return false;

   if (!mLocal.mValid)
   {
      setError(666);
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

void BaseSocket::reset()
{
   mStatus       = 0;
   mError        = 0;
   mBaseSpecific->mDesc  = -1;
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
   if (mStatus < 0) return false;

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

bool BaseSocket::ioctlBlocking(bool aBlocking)
{
   if (mStatus < 0) return false;

   int tStatus = 0;
   u_long value;
   if (aBlocking) value=0;
   else           value=1;
   tStatus = ioctlsocket(mBaseSpecific->mDesc, FIONBIO, &value);
   return updateError(tStatus);
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
   mBaseSpecific->mDesc = socket(AF_INET, mType, mProtocol);

   tStatus = mBaseSpecific->mDesc == INVALID_SOCKET ? -1 : 0;

   return updateError(tStatus);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseTcpStreamSocket::doConnect()
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

