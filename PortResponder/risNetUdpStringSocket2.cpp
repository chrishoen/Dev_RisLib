/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risNetUdpStringSocket2.h"

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

UdpWrapStringSocket::UdpWrapStringSocket()
{
   mRxString[0] = 0;
   mTxLength = 0;
   mRxLength = 0;
   mTxCount = 0;
   mRxCount = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void UdpWrapStringSocket::initialize(Settings& aSettings)
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
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// configure the socket.

void UdpWrapStringSocket::configure()
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
      printf("UdpWrapStringSocket  PASS %16s : %5d\n",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort);
   }
   else
   {
      printf("UdpWrapStringSocket  FAIL %16s : %5d $ %d %d\n",
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

void UdpWrapStringSocket::configureLocal(int aPort)
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
      //print("UdpWrapStringSocket  PASS %16s : %5d\n",
      // BaseClass::mLocal.mString,
      // BaseClass::mLocal.mPort);
   }
   else
   {
      printf("UdpWrapStringSocket  FAIL %16s : %5d $ %d %d\n",
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

bool UdpWrapStringSocket::doRecvString ()
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
      printf("ERROR UdpWrapStringSocket INVALID SOCKET\n");
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
         //printf("UdpWrapStringSocket CLOSED\n");
      }
      else
      {
         printf("ERROR UdpWrapStringSocket %d %d\n", BaseClass::mStatus, BaseClass::mError);
      }
      return false;
   }

   //printf("UdpWrapStringSocket rx message %d\n", mRxLength);

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
// Send a string over the socket via a blocking send call.
// It returns true if successful.
// It is protected by the transmit mutex.

bool UdpWrapStringSocket::doSendString(const char* aString)
{
   // Guard
   if (!mValidFlag) return false;

   // Transmit the buffer
   int tLength = (int)strlen(aString);
   doSendTo(mFromAddress,aString,tLength);

   mTxLength = tLength;
   mTxCount++;

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

