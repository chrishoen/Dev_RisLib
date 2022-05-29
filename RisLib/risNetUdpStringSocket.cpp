/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

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

UdpRxStringSocket::UdpRxStringSocket()
{
   mRxString[0] = 0;
   mRxLength = 0;
   mRxCount = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpRxStringSocket::initialize(Settings& aSettings)
{
   // Store the settings pointer.
   mSettings = aSettings;

   // Variables.
   mRxString[0] = 0;
   mRxLength = 0;
   mRxCount = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// configure the socket.

void UdpRxStringSocket::configure()
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
      printf("UdpRxStringSocket  PASS %16s : %5d\n",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort);
   }
   else
   {
      printf("UdpRxStringSocket  FAIL %16s : %5d $ %d %d\n",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// configure the socket at the local address at a port number.

void UdpRxStringSocket::configureLocal(int aPort)
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
      //print("UdpRxStringSocket  PASS %16s : %5d\n",
      // BaseClass::mLocal.mString,
      // BaseClass::mLocal.mPort);
   }
   else
   {
      printf("UdpRxStringSocket  FAIL %16s : %5d $ %d %d\n",
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

bool UdpRxStringSocket::doRecvString ()
{
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
      printf("ERROR UdpRxStringSocket INVALID SOCKET\n");
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
         //printf("UdpRxStringSocket CLOSED\n");
      }
      else
      {
         printf("ERROR UdpRxStringSocket %d %d\n", BaseClass::mStatus, BaseClass::mError);
      }
      return false;
   }

   //printf("UdpRxStringSocket rx message %d\n", mRxLength);

   // Add null terminator.
   mRxString[mRxLength] = 0;

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

UdpTxStringSocket::UdpTxStringSocket()
{
   mValidFlag = false;
   mTxLength = 0;
   mTxCount = 0;
   mPrintDisable = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpTxStringSocket::initialize(Settings& aSettings)
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

void UdpTxStringSocket::configure()
{
   // Configure the socket.
   BaseClass::mRemote.setByAddress(mSettings.mRemoteIpAddr, mSettings.mRemoteIpPort);
   BaseClass::testRemoteAddress(mSettings.mTestForLocal);
   BaseClass::doSocket();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Guard.
   if (mPrintDisable) return;

   // Show.
   if (mValidFlag)
   {
      printf("UdpTxStringSocket  PASS %16s : %5d\n",
         BaseClass::mRemote.mString,
         BaseClass::mRemote.mPort);
   }
   else
   {
      printf("UdpTxStringSocket  FAIL %16s : %5d $ %d %d\n",
         BaseClass::mRemote.mString,
         BaseClass::mRemote.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a string over the socket via a blocking send call.
// It returns true if successful.
// It is protected by the transmit mutex.

bool UdpTxStringSocket::doSendString(const char* aString)
{
   // Guard
   if (!mValidFlag) return false;

   // Mutex
   mTxMutex.lock();

   // Transmit the buffer
   int tLength = (int)strlen(aString);
   doSendTo(mRemote,aString,tLength);

   mTxLength = tLength;
   mTxCount++;

   // Mutex
   mTxMutex.unlock();

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

