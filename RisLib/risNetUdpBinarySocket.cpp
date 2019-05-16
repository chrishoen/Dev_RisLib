/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risNetUdpBinarySocket.h"

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

UdpRxBinarySocket::UdpRxBinarySocket()
{
   mRxLength = 0;
   mRxCount = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpRxBinarySocket::initialize(Settings& aSettings)
{
   // Store the settings pointer.
   mSettings = aSettings;

   // Variables.
   mRxLength = 0;
   mRxCount = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// configure the socket.

void UdpRxBinarySocket::configure()
{
   // Configure the socket.
   BaseClass::mLocal.setForAny(mSettings.mLocalIpPort);
   BaseClass::doSocket();
   BaseClass::doBind();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      TS::print(0, "UdpRxBinarySocket  PASS %16s : %5d",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort);
   }
   else
   {
      TS::print(0, "UdpRxBinarySocket  FAIL %16s : %5d $ %d %d",
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

bool UdpRxBinarySocket::doRecvBinary ()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   // Do this first.
   mRxBinary[0]=0;
   mRxLength=0;

   // Guard.
   if (!mValidFlag)
   {
      TS::print(0, "ERROR UdpRxBinarySocket INVALID SOCKET");
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read the binary into the receive buffer.

   // Read from the socket.
   BaseClass::doRecvFrom(mFromAddress,mRxBinary,mRxLength,cBinarySize);

   // Guard.
   // If bad status then return false.
   // Returning true  means socket was not closed.
   // Returning false means socket was closed.
   if (mRxLength <= 0)
   {
      if (BaseClass::mError == 0)
      {
         TS::print(1, "UdpRxBinarySocket CLOSED");
      }
      else
      {
         TS::print(0, "ERROR UdpRxBinarySocket %d %d", BaseClass::mStatus, BaseClass::mError);
      }
      return false;
   }

   TS::print(3, "UdpRxBinarySocket rx message %d", mRxLength);
   
   // Returning true  means socket was not closed.
   // Returning false means socket was closed.
   mRxCount++;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

UdpTxBinarySocket::UdpTxBinarySocket()
{
   mValidFlag = false;
   mTxLength = 0;
   mTxCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpTxBinarySocket::initialize(Settings& aSettings)
{
   // Store the settings pointer.
   mSettings = aSettings;

   // Variables.
   mTxLength = 0;
   mTxCount = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void UdpTxBinarySocket::configure()
{
   // Configure the socket.
   BaseClass::mRemote.setByHostName(mSettings.mRemoteIpAddr, mSettings.mRemoteIpPort);
   BaseClass::testRemoteAddress(mSettings.mTestForLocal);
   BaseClass::doSocket();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      TS::print(0, "UdpTxBinarySocket  PASS %16s : %5d",
         BaseClass::mRemote.mString,
         BaseClass::mRemote.mPort);
   }
   else
   {
      TS::print(0, "UdpTxBinarySocket  FAIL %16s : %5d $ %d %d",
         BaseClass::mRemote.mString,
         BaseClass::mRemote.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a binary over the socket via a blocking send call.
// It returns true if successful.
// It is protected by the transmit mutex.

bool UdpTxBinarySocket::doSendBinary(void* aBinary,int aLength)
{
   // Guard
   if (!mValidFlag) return false;

   // Transmit the buffer
   doSendTo(mRemote,(const char*)aBinary,aLength);

   mTxLength = aLength;
   mTxCount++;

   return true;
}

bool UdpTxBinarySocket::doSendBinary(void* aBinary, size_t aLength)
{
   int tLength = (int)aLength;
   return doSendTo(mRemote, (const char*)aBinary, tLength);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

