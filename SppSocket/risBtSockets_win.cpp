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

BtSocketAddress::BtSocketAddress()
{
   reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BtSocketAddress::reset()
{
   mValid = false;
   memset(&mBtAddress, 0, sizeof(mBtAddress));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseSocket::BaseSocket()
{
   reset();
}

void BaseSocket::reset()
{
   mLocal.reset();
   mRemote.reset();
   mDesc = INVALID_SOCKET;

   mStatus = 0;
   mError = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseSocket::~BaseSocket()
{
   doClose();
   reset();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

bool BaseSocket::doClose()
{
   int tStatus=0;

   tStatus = closesocket(mDesc);
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
   tStatus = ioctlsocket(mDesc, FIONBIO, &value);
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

   mDesc = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

   int tStatus = mDesc == INVALID_SOCKET ? -1 : 0;
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
   tStatus = connect(
      mDesc, 
      (struct sockaddr*)&mRemote.mBtAddress, 
      sizeof(SOCKADDR_BTH));
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
      tStatus=send(mDesc,&aPayload[bytesTotal],bytesRemaining,0);
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
      tStatus=recv(mDesc,&aPayload[bytesTotal],bytesRemaining,0);
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
}//namespace
}//namespace

