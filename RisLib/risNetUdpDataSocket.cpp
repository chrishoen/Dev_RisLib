/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "trcTrace.h"
#include "risNetUdpDataSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

UdpDataSocket::UdpDataSocket()
{
   mTxMsgCount = 0;
   mTxByteCount = 0;
   mRxMsgCount = 0;
   mRxByteCount = 0;
   mValidFlag = false;
   mTI = 0;
   mPrintDisable = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpDataSocket::initialize(Settings* aSettings)
{
   // Store the settings pointer.
   mSettings = *aSettings;

   // Variables.
   mTxMsgCount = 0;
   mTxByteCount = 0;
   mRxMsgCount = 0;
   mRxByteCount = 0;
   mValidFlag = false;
   mTI = mSettings.mTraceIndex;
   mPrintDisable = false;

}

UdpDataSocket::~UdpDataSocket()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpDataSocket::configure()
{
   // Set the socket receive address.
   if (mSettings.mLocalIpPort)
   {
      BaseClass::mLocal.setByAddress(mSettings.mLocalIpAddr, mSettings.mLocalIpPort);
   }

   // Set the socket send address.
   if (mSettings.mRemoteIpPort)
   {
      BaseClass::mRemote.setByAddress(mSettings.mRemoteIpAddr, mSettings.mRemoteIpPort);
   }

   // Configure the socket.
   if (!BaseClass::doSocket()) goto ConfigDone;

   // If listening then bind the socket to the receive address.
   if (BaseClass::mLocal.mPort)
   {
      BaseClass::setOptionReuseAddr();
      if (!BaseClass::doBind()) goto ConfigDone;
   }

   // Test for broadcast.
   if (mSettings.mUdpBroadcast)
   {
      BaseClass::mRemote.setForBroadcast(mSettings.mRemoteIpPort);
      if (!BaseClass::setOptionBroadcast()) goto ConfigDone;
   }

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Guard for don't print status.
   if (mPrintDisable) return;

ConfigDone:

   // Show.
   if (mValidFlag)
   {
      if (!mSettings.mUdpWrapFlag)
      {
         Trc::write(mTI, 0, "UdpDataSocket    PASS %16s : %5d Tx %16s : %5d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort);

         printf("UdpDataSocket    PASS %16s : %5d Tx %16s : %5d\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort);
      }
      else
      {
         Trc::write(mTI, 0, "UdpDataSocket    PASS %16s : %5d Tx WRAP",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort);

         printf("UdpDataSocket    PASS %16s : %5d Tx WRAP\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort);
      }
   }
   else
   {
      if (!mSettings.mUdpWrapFlag)
      {
         Trc::write(mTI, 0, "UdpDataSocket    FAIL %16s : %5d Tx %16s : %5d  $ %d %d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort,
            BaseClass::mStatus,
            BaseClass::mError);

         printf("UdpDataSocket    FAIL %16s : %5d Tx %16s : %5d  $ %d %d\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort,
            BaseClass::mStatus,
            BaseClass::mError);
      }
      else
      {
         Trc::write(mTI, 0, "UdpDataSocket    FAIL %16s : %5d Tx WRAP  $ %d %d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mStatus,
            BaseClass::mError);

         printf("UdpDataSocket    FAIL %16s : %5d Tx WRAP  $ %d %d\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mStatus,
            BaseClass::mError);
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This receives a datagram from the socket into a byte buffer and then
// extracts a message from the byte buffer

bool UdpDataSocket::doRecvData(char* aData, int* aSize)
{
   Trc::write(mTI, 0, "UdpDataSocket::doRecData");
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   // Do this first.
   int tSize = 0;
   *aSize = 0;

   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "UdpDataSocket::doRecvData ERROR INVALID SOCKET");
      printf("UdpDataSocket::doRecvData ERROR INVALID SOCKET\n");
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the data into the receive buffer.

   // Read from the socket.
   BaseClass::doRecvFrom(mFromAddress, aData, tSize, cBufferSize);

   // Guard.
   // If bad status then return false.
   // Returning true  means socket was not closed.
   // Returning false means socket was closed.
   if (tSize <= 0)
   {
      if (BaseClass::mError == 0)
      {
         Trc::write(mTI, 0, "UdpDataSocket::doRecvData CLOSED");
      }
      else
      {
         Trc::write(mTI, 0, "UdpDataSocket::doRecvData ERROR %d %d", BaseClass::mStatus, BaseClass::mError);
         printf("UdpDataSocket::doRecvData ERROR %d %d\n", BaseClass::mStatus, BaseClass::mError);
      }
      return false;
   }

   Trc::write(mTI, 1, "UdpDataSocket::doRecvData %16s : %5d  ",
      mFromAddress.mString,
      mFromAddress.mPort);

   //printf("UdpDataSocket rx message %d\n", mRxSize);

   // Returning true  means socket was not closed.
   // Returning false means socket was closed.
   Trc::write(mTI, 1, "UdpDataSocket doRecvData done %d %d", mStatus, mError);
   mRxMsgCount++;
   mRxByteCount += tSize;
   *aSize = tSize;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a data over the socket via a blocking send call.
// It returns true if successful.
// It is protected by the transmit mutex.

bool UdpDataSocket::doSendData(const char* aData, int aSize)
{
   Trc::write(mTI, 0, "UdpDataSocket::doSendData");
   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "UdpDataSocket::doSendData ERROR INVALID SOCKET");
      printf("UdpDataSocket::doSendData ERROR INVALID SOCKET\n");
      return false;
   }

   // Transmit the data.
   bool tRet = false;
   if (!mSettings.mUdpWrapFlag)
   {
      // If this is not wrapping then send to the remote address.
      tRet = doSendTo(mRemote, aData, aSize);
   }
   else
   {
      // If this is wrapping then send to the last received from address.
      if (mRxMsgCount)
      {
         Trc::write(mTI, 1, "UdpDataSocket::doSendData WRAP Tx %16s : %5d  ",
            mFromAddress.mString,
            mFromAddress.mPort);
         // If this is wrapping then send to the last received from address.
         tRet = doSendTo(mFromAddress, aData, aSize);
      }
   }

   if (tRet)
   {
      // The send was successful.
      // Metrics.
      mTxMsgCount++;
      mTxByteCount += aSize;
   }
   else
   {
      // The send was not successful.
      Trc::write(mTI, 0, "UdpDataSocket::doSendData ERROR INVALID SEND");
      printf("UdpMsgSocket::doSendData ERROR INVALID SEND\n");
      // Set the socket invalid.
      mValidFlag = false;
      doClose();
   }

   // Done.
   Trc::write(mTI, 1, "UdpDataSocket::doSendData done %d", aSize);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

