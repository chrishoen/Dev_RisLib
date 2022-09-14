/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "prnPrint.h"
#include "trcTrace.h"

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
   mTI = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Initialize variables.

void TcpServerHubSocket::initialize(Settings& aSettings)
{
   // Store the settings.
   mSettings = aSettings;

   // Trace.
   mTI = mSettings.mTraceIndex;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure the socket.

void TcpServerHubSocket::configure(bool aShowFlag)
{
   // Configure the socket.
   BaseClass::reset();
   BaseClass::mLocal.setByAddress(mSettings.mLocalIpAddr, mSettings.mLocalIpPort);
   BaseClass::doSocket();
   BaseClass::setOptionReuseAddr();
   BaseClass::setOptionNoDelay();
   BaseClass::setOptionDontLinger();


   BaseClass::doBind();

   // Set valid flag from base class results.
   mValidFlag = BaseClass::mStatus == 0;

   // Show.
   if (mValidFlag && aShowFlag)
   {
      Trc::write(mTI, 0, "TcpServerHubSocket PASS %16s : %5d",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort);
      printf("TcpServerHubSocket PASS %16s : %5d\n",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort);
   }
   else if (!mValidFlag)
   {
      Trc::write(mTI, 0, "TcpServerHubSocket FAIL % 16s : % d $ % d % d",
         BaseClass::mLocal.mString,
         BaseClass::mLocal.mPort,
         BaseClass::mStatus,
         BaseClass::mError);
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
}//namespace
}//namespace
