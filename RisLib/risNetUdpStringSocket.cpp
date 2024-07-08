/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "trcTrace.h"
#include "risNetUdpStringSocket.h"

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

UdpStringSocket::UdpStringSocket()
{
   mRxString[0] = 0;
   mTxLength = 0;
   mRxLength = 0;
   mTxCount = 0;
   mRxCount = 0;
   mValidFlag = false;
   mTI = 0;
   mPrintDisable = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpStringSocket::initialize(Settings* aSettings)
{
   // Store a copy of the settings.
   mSettings = *aSettings;

   // Variables.
   mRxString[0] = 0;
   mTxLength = 0;
   mRxLength = 0;
   mTxCount = 0;
   mRxCount = 0;
   mValidFlag = false;
   mTI = mSettings.mTraceIndex;
   mPrintDisable = false;

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpStringSocket::configure()
{
   // Reset the socket.
   BaseClass::reset();

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
      if (!BaseClass::setOptionReuseAddr()) goto ConfigDone;
      if (!BaseClass::doBind()) goto ConfigDone;
   }

   // If broadcast then set accordingly.
   if (mSettings.mUdpBroadcast)
   {
      BaseClass::mRemote.setForBroadcast(mSettings.mRemoteIpPort);
      if (!BaseClass::setOptionBroadcast()) goto ConfigDone;
   }

   // Set valid.
   mValidFlag = true;

ConfigDone:

   // Test for don't print status.
   if (mPrintDisable) return;

   // Show.
   if (mValidFlag)
   {
      if (!mSettings.mUdpWrapFlag)
      {
         Trc::write(mTI, 0, "UdpStringSocket    PASS %16s : %5d Tx %16s : %5d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort);

         printf("UdpStringSocket    PASS %16s : %5d Tx %16s : %5d\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort);
      }
      else
      {
         Trc::write(mTI, 0, "UdpStringSocket    PASS %16s : %5d Tx WRAP",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort);

         printf("UdpStringSocket    PASS %16s : %5d Tx WRAP\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort);
      }
   }
   else
   {
      if (!mSettings.mUdpWrapFlag)
      {
         Trc::write(mTI, 0, "UdpStringSocket    FAIL %16s : %5d Tx %16s : %5d  $ %d %d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort,
            BaseClass::mStatus,
            BaseClass::mError);

         printf("UdpStringSocket    FAIL %16s : %5d Tx %16s : %5d  $ %d %d\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort,
            BaseClass::mStatus,
            BaseClass::mError);
      }
      else
      {
         Trc::write(mTI, 0, "UdpStringSocket    FAIL %16s : %5d Tx WRAP  $ %d %d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mStatus,
            BaseClass::mError);

         printf("UdpStringSocket    FAIL %16s : %5d Tx WRAP  $ %d %d\n",
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
// Receive a datagram from the socket into a byte buffer and then
// extract a message from the byte buffer

bool UdpStringSocket::doRecvString()
{
   Trc::write(mTI, 0, "UdpStringSocket::doReceiveString");
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "UdpStringSocket::doRecvString ERROR INVALID SOCKET");
      printf("UdpStringSocket::doRecvString ERROR INVALID SOCKET\n");
      return false;
   }

   // Initialize.
   mRxString[0] = 0;
   mRxLength = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the string into the receive buffer.

   // Read from the socket.
   BaseClass::doRecvFrom(mFromAddress,mRxString,mRxLength,cStringSize);

   // If bad status then return false.
   if (mRxLength <= 0)
   {
      if (BaseClass::mError == 0)
      {
         Trc::write(mTI, 0, "UdpStringSocket::doRecvString CLOSED");
      }
      else
      {
         Trc::write(mTI, 0, "UdpStringSocket::doRecvString ERROR %d %d", BaseClass::mStatus, BaseClass::mError);
         printf("UdpStringSocket::doRecvString ERROR %d %d\n", BaseClass::mStatus, BaseClass::mError);
      }
      return false;
   }

   Trc::write(mTI, 1, "UdpStringSocket::doRecvString %16s : %5d  ",
      mFromAddress.mString,
      mFromAddress.mPort);

   // Add null terminator.
   mRxString[mRxLength] = 0;

   // Done.
   Trc::write(mTI, 1, "UdpStringSocket doReceiveString done %d %d", mStatus, mError);
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a string over the socket via a send call. Return true if successful.

bool UdpStringSocket::doSendString(const char* aString)
{
   Trc::write(mTI, 0, "UdpStringSocket::doSendString");

   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "UdpStringSocket::doSendString ERROR INVALID SOCKET");
      printf("UdpStringSocket::doSendString ERROR INVALID SOCKET\n");
      return false;
   }

   // Transmit the string.
   bool tRet = false;
   int mTxLength = (int)strlen(aString) + 1;
   if (!mSettings.mUdpWrapFlag)
   {
      // If this is not wrapping then send to the remote address.
      tRet = doSendTo(mRemote, aString, mTxLength);
   }
   else if (mRxCount)
   {
      // If this is wrapping then send to the last received from address, 
      // if it is valid (rx count is greater than zero).
      tRet = doSendTo(mFromAddress, aString, mTxLength);
   }

   if (tRet)
   {
      // The send was successful. Update metrics.
      mTxCount++;
   }
   else
   {
      // The send was not successful.
      Trc::write(mTI, 0, "UdpStringSocket::doSendString ERROR INVALID SEND");
      printf("UdpMsgSocket::doSendString ERROR INVALID SEND\n");
      // Set the socket invalid and close it.
      mValidFlag = false;
      doClose();
   }

   // Done. Return true if successful.
   Trc::write(mTI, 1, "UdpStringSocket::doSendString done %d", mTxLength);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

