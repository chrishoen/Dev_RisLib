/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risNetTcpServerHubSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TcpServerHubSocket::TcpServerHubSocket()
{
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void TcpServerHubSocket::initialize(Settings& aSettings)
{
   // Store the settings pointer.
   mSettings = aSettings;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void TcpServerHubSocket::configure()
{
   // Configure the socket.
   BaseClass::reset();
   BaseClass::mLocal.setForAny(mSettings.mLocalIpPort);
   BaseClass::doSocket();
   BaseClass::setOptionReuseAddr();
   BaseClass::doBind();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag)
   {
      printf("TcpServerHubSocket PASS %16s : %5d\n",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort);
   }
   else
   {
      printf("TcpServerHubSocket FAIL %16s : %d $ %d %d\n",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This re-initializes the socket.

void TcpServerHubSocket::reconfigure()
{
   BaseClass::doSocket();
   BaseClass::setOptionReuseAddr();
   BaseClass::doBind();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
