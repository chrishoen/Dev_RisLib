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

void UdpRxBinarySocket::initialize(Settings* aSettings)
{
   // Store a copy of the settings.
   mSettings = *aSettings;

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
   BaseClass::reset();
   BaseClass::mLocal.setByAddress(mSettings.mLocalIpAddr, mSettings.mLocalIpPort);
   if (!BaseClass::doSocket()) goto ConfigDone;
   if (!BaseClass::doBind()) goto ConfigDone;
   mValidFlag = true;

ConfigDone:

   // Show.
   if (mValidFlag)
   {
      printf("UdpRxBinarySocket  PASS %16s : %5d\n",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort);
   }
   else
   {
      printf("UdpRxBinarySocket  FAIL %16s : %5d $ %d %d\n",
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
      printf("ERROR UdpRxBinarySocket INVALID SOCKET\n");
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
         //printf("UdpRxBinarySocket CLOSED\n");
      }
      else
      {
         printf("ERROR UdpRxBinarySocket %d %d\n", BaseClass::mStatus, BaseClass::mError);
      }
      return false;
   }

   //printf("UdpRxBinarySocket rx message %d\n", mRxLength);
   
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
   BaseClass::mRemote.setByAddress(mSettings.mRemoteIpAddr, mSettings.mRemoteIpPort);
   BaseClass::testRemoteAddress(mSettings.mTestForLocal);
   BaseClass::doSocket();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      printf("UdpTxBinarySocket  PASS %16s : %5d\n",
         BaseClass::mRemote.mString,
         BaseClass::mRemote.mPort);
   }
   else
   {
      printf("UdpTxBinarySocket  FAIL %16s : %5d $ %d %d\n",
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

