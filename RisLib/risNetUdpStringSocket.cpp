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

void UdpStringSocket::initialize(Settings& aSettings)
{
   // Store the settings pointer.
   mSettings = aSettings;

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
   // Configure the socket.
   BaseClass::mLocal.setForAny(mSettings.mLocalIpPort);
   BaseClass::mRemote.setByAddress(mSettings.mRemoteIpAddr, mSettings.mRemoteIpPort);
   BaseClass::doSocket();
   BaseClass::doBind();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Guard.
   if (mPrintDisable) return;

   // Show.
   if (mValidFlag)
   {
      if (!mSettings.mUdpWrapFlag)
      {
         Trc::write(mTI, 0, "UdpStringSocket     PASS Rx %16s : %5d Tx %16s : %5d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort);

         printf("UdpStringSocket     PASS Rx %16s : %5d Tx %16s : %5d\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort);
      }
      else
      {
         Trc::write(mTI, 0, "UdpStringSocket     PASS Rx %16s : %5d Tx WRAP",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort);

         printf("UdpStringSocket     PASS Rx %16s : %5d Tx WRAP\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort);
      }
   }
   else
   {
      if (!mSettings.mUdpWrapFlag)
      {
         Trc::write(mTI, 0, "UdpStringSocket     FAIL Rx %16s : %5d Tx %16s : %5d  $ %d %d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort,
            BaseClass::mStatus,
            BaseClass::mError);

         printf("UdpStringSocket     FAIL Rx %16s : %5d Tx %16s : %5d  $ %d %d\n",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mRemote.mString,
            BaseClass::mRemote.mPort,
            BaseClass::mStatus,
            BaseClass::mError);
      }
      else
      {
         Trc::write(mTI, 0, "UdpStringSocket     FAIL Rx %16s : %5d Tx WRAP  $ %d %d",
            BaseClass::mLocal.mString,
            BaseClass::mLocal.mPort,
            BaseClass::mStatus,
            BaseClass::mError);

         printf("UdpStringSocket     FAIL Rx %16s : %5d Tx WRAP  $ %d %d\n",
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
// Configure the socket at the local address at a port number.

void UdpStringSocket::configureLocal(int aPort)
{
   // Configure the socket.
   BaseClass::mLocal.setForAny(aPort);
   BaseClass::doSocket();
   BaseClass::doBind();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      //print("UdpStringSocket  PASS %16s : %5d\n",
      // BaseClass::mLocal.mString,
      // BaseClass::mLocal.mPort);
   }
   else
   {
      printf("UdpStringSocket  FAIL %16s : %5d $ %d %d\n",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This receives a datagram from the socket into a byte buffer and then
// extracts a message from the byte buffer

bool UdpStringSocket::doRecvString ()
{
   Trc::write(mTI, 0, "UdpStringSocket::doReceiveString");
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   // Do this first.
   mRxString[0]=0;
   mRxLength=0;

   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "ERROR UdpStringSocket INVALID SOCKET");
      printf("ERROR UdpStringSocket INVALID SOCKET\n");
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the string into the receive buffer.

   // Read from the socket.
   BaseClass::doRecvFrom(mFromAddress,mRxString,mRxLength,cStringSize);

   // Guard.
   // If bad status then return false.
   // Returning true  means socket was not closed.
   // Returning false means socket was closed.
   if (mRxLength <= 0)
   {
      if (BaseClass::mError == 0)
      {
         Trc::write(mTI, 0, "UdpStringSocket CLOSED");
      }
      else
      {
         Trc::write(mTI, 0, "ERROR UdpStringSocket %d %d", BaseClass::mStatus, BaseClass::mError);
         printf("ERROR UdpStringSocket %d %d\n", BaseClass::mStatus, BaseClass::mError);
      }
      return false;
   }

   Trc::write(mTI, 1, "UdpStringSocket::doRecvFrom %16s : %5d  ",
      mFromAddress.mString,
      mFromAddress.mPort);

   //printf("UdpStringSocket rx message %d\n", mRxLength);

   // Add null terminator.
   mRxString[mRxLength] = 0;

   // Returning true  means socket was not closed.
   // Returning false means socket was closed.
   Trc::write(mTI, 1, "UdpStringSocket doReceiveString done %d %d", mStatus, mError);
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a string over the socket via a blocking send call.
// It returns true if successful.
// It is protected by the transmit mutex.

bool UdpStringSocket::doSendString(const char* aString)
{
   Trc::write(mTI, 0, "UdpStringSocket::doSendString");
   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "ERROR UdpStringSocket INVALID SOCKET");
      printf("ERROR UdpStringSocket INVALID SOCKET\n");
      return false;
   }

   // Transmit the string.
   bool tRet = false;
   int mTxLength = (int)strlen(aString);
   if (!mSettings.mUdpWrapFlag)
   {
      // If this is not wrapping then send to the remote address.
      tRet = doSendTo(mRemote, aString, mTxLength);
   }
   else
   {
      // If this is wrapping then send to the last received from address.
      if (mRxCount)
      {
         Trc::write(mTI, 1, "UdpStringSocket::doSendString WRAP Tx %16s : %5d  ",
            mFromAddress.mString,
            mFromAddress.mPort);
         // If this is wrapping then send to the last received from address.
         tRet = doSendTo(mRemote, aString, mTxLength);
      }
   }

   mTxCount++;

   if (tRet)
   {
      //printf("UdpStringSocket tx message %d\n", mTxLength);
   }
   else
   {
      Trc::write(mTI, 0, "ERROR UdpStringSocket INVALID SEND");
      printf("ERROR UdpStringSocket INVALID SEND\n");
   }

   // Done.
   Trc::write(mTI, 1, "UdpStringSocket::doSendString done %d", mTxLength);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

