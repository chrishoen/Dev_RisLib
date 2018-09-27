/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"

#include "risNetTcpMsgServerHubSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

TcpMsgServerHubSocket::TcpMsgServerHubSocket()
{
   mSettings = 0;
   mValidFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void TcpMsgServerHubSocket::initialize(Settings* aSettings)
{
   // Store the settings pointer.
   mSettings = aSettings;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void TcpMsgServerHubSocket::configure()
{
   // Configure the socket.
   BaseClass::reset();
   BaseClass::mLocal.set(mSettings->mLocalIpAddr, mSettings->mLocalIpPort);
   BaseClass::doSocket();
   BaseClass::setOptionReuseAddr();
   BaseClass::doBind();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag == 0)
   {
      Prn::print(Prn::SocketInit2, "TcpServerHubSocket $ %16s : %d",
         BaseClass::mLocal.mIpAddr.mString,
         BaseClass::mLocal.mPort);
   }
   else
   {
      Prn::print(Prn::SocketInit2, "TcpServerHubSocket $ %16s : %d $ %d %d",
         BaseClass::mLocal.mIpAddr.mString,
         BaseClass::mLocal.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This re-initializes the socket.

void TcpMsgServerHubSocket::reconfigure()
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
