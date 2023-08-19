//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <signal.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/serial.h>

#include "my_functions.h"
#include "risThreadsThreads.h"
#include "trcTrace.h"

#include "risSerialPort_linux.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the number of bytes that are available to receive.

int SerialPort::doGetAvailableReceiveBytes()
{
   int tBytesAvaiable;
   ioctl(mPortFd, FIONREAD, &tBytesAvaiable);
   return tBytesAvaiable;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive any available bytes. Block until at least one byte has
// been received. Return the number of bytes received or a negative
// error code. Copy the bytes into the pointer argument.

int SerialPort::doReceiveAnyBytes(char* aBytes, int aMaxNumBytes)
{
   Trc::write(mTI, 0, "SerialPort::doReceiveAnyBytes");
   // Guard.
   int tRet = 0;
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doReceiveAnyBytes INVALID 1");
      return cSerialRetError;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Poll the port for a read or a close.

   // Setup a poll structure for the port and event fds.
   struct pollfd tPollFd[2];
   tPollFd[0].fd = mPortFd;
   tPollFd[0].events = POLLIN;
   tPollFd[0].revents = 0;
   tPollFd[1].fd = mEventFd;
   tPollFd[1].events = POLLIN;
   tPollFd[1].revents = 0;

   // Receive timeout.
   int tTimeout = mSettings.mRxTimeout > 0 ? mSettings.mRxTimeout : -1;
   
   // Blocking poll for "there is data to read". This returns when
   // the port has received data or the abort event was posted
   tRet = poll(&tPollFd[0], 2, tTimeout);

   // Test for an abort request.
   if (mAbortFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doReceiveAnyBytes ABORT");
      return cSerialRetAbort;
   }

   // Test for an invalid port.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doReceiveAnyBytes INVALID 2");
      return cSerialRetError;
   }

   // Test the return code for error.
   if (tRet < 0)
   {
      mValidFlag = false;
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveAnyBytes ERROR 1");
      return cSerialRetError;
   }

   // Test the return code for timeout.
   if (tRet == 0)
   {
      Trc::write(mTI, 0, "SerialPort::doReceiveAnyBytes timeout");
      printf("serial_poll_error_2 timeout\n");
      return cSerialRetTimeout;
   }

   if (tPollFd[0].revents & POLLIN)
   {
      //printf("serial_poll_event0 %d %04X\n",tRet, tPollFd[0].revents);
   }

   if (tPollFd[1].revents & POLLIN)
   {
      //printf("serial_poll_event1 %d %04X\n", tRet, tPollFd[1].revents);
   }

   // Test the return code for closed port.
   if (tRet == 2)
   {
      mValidFlag = false;
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveAnyBytes ERROR 2");
      return cSerialRetError;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read from the serial port.

   // Read.
   tRet = (int)read(mPortFd, aBytes, (size_t)aMaxNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      mValidFlag = false;
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doReceiveAnyBytes ERROR 3");
      return cSerialRetError;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   // Read was successful. Return the number of bytes read.
   Trc::write(mTI, 0, "SerialPort::doReceiveAnyBytes done");
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive a requested number of bytes. Block until all of the bytes
// have been received. Return the number of bytes received or a
// negative error code. Copy the bytes into the pointer argument.

int SerialPort::doReceiveBytes(char* aBytes, int aRequestBytes)
{
   Trc::write(mTI, 0, "SerialPort::doReceiveBytes");
   // Guard.
   int tRet = 0;
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doReceiveBytes INVALID 1");
      return cSerialRetError;
   }

   // Loop to read all of the requested bytes.
   int tBytesRequired = aRequestBytes;
   int tBytesRemaining = aRequestBytes;
   int tBytesTotal = 0;

   bool tGoing = true;
   while (tGoing)
   {
      // Read any available receive bytes. Block until at least one byte
      // has been received. Return the number of bytes read or a negative
      // error code.
      tRet = doReceiveAnyBytes(&aBytes[tBytesTotal], tBytesRemaining);

      if (tRet > 0)
      {
         tBytesTotal += tRet;
         tBytesRemaining -= tRet;
         if (tBytesTotal == tBytesRequired)
         {
            tGoing = false;
            tRet = tBytesTotal;
         }
      }
      else
      {
         tGoing = false;
      }
   }

   // If success then return the total number of bytes read, which is 
   // equal to the requested number of bytes. If failure then return
   // the negative error code.
   mRxByteCount += tRet;
   Trc::write(mTI, 0, "SerialPort::doReceiveBytes done");
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

