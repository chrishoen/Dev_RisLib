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

#include "risSerialPort.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Portable specifics.

class SerialPort::Specific
{
public:
   int mPortFd;
   int mEventFd;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPort::SerialPort()
{
   mSpecific = new Specific;
   mSpecific->mPortFd = 0;
   mSpecific->mEventFd = 0;
   mValidFlag = false;
   mOpenFlag = false;
   mAbortFlag = false;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;
   mTI = 0;
   mRxByteCount = 0;
   mTxByteCount = 0;
   mPortErrorCount = 0;
}

SerialPort::~SerialPort()
{
   delete mSpecific;
}

void SerialPort::initialize(SerialSettings& aSettings)
{
   mSettings = aSettings;
   mTI = aSettings.mTraceIndex;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Open the serial port with the settings.
//
// Configure the port for raw, set the baud rate, and optionally 
// select RS485.

bool SerialPort::doOpen()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   Trc::write(mTI, 0, "SerialPort::doOpen");

   // Guard.
   if (mOpenFlag)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen ERROR already open");
      return false;
   }

   mValidFlag = false;
   mAbortFlag = false;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Open the port.

   // Open the port.
   mSpecific->mPortFd = open(mSettings.mPortDevice, O_RDWR | O_NOCTTY | O_SYNC);

   // Test the return code.
   if (mSpecific->mPortFd < 0)
   {
      if (mOpenErrorShowCount == 0)
      {
         printf("serial_open_error_1 %s %d %s\n", mSettings.mPortDevice, errno, strerror(errno));
      }
      mOpenErrorShowCount++;
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen ERROR 1");
      return false;
   }

   // Open the event.
   mSpecific->mEventFd = eventfd(0, EFD_SEMAPHORE);

   // Test the return code.
   if (mSpecific->mPortFd < 0)
   {
      if (mOpenErrorShowCount == 0)
      {
         printf("serial_open_error_2 %s %d %s\n", mSettings.mPortDevice, errno, strerror(errno));
      }
      mOpenErrorShowCount++;
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen ERROR 2");
      return false;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Configure the port from the settings.

   // Configure the port for raw data.
   struct termios tOptions;
   tcgetattr(mSpecific->mPortFd, &tOptions);
   cfmakeraw(&tOptions);

   // Configure the baud rate.
   if (strcmp(mSettings.mPortSetup, "9600") == 0)
   {
      cfsetispeed(&tOptions, B9600);
      cfsetospeed(&tOptions, B9600);
   }
   else if (strcmp(mSettings.mPortSetup, "19200") == 0)
   {
      cfsetispeed(&tOptions, B19200);
      cfsetospeed(&tOptions, B19200);
   }
   else if (strcmp(mSettings.mPortSetup, "38400") == 0)
   {
      cfsetispeed(&tOptions, B38400);
      cfsetospeed(&tOptions, B38400);
   }
   else
   {
      cfsetispeed(&tOptions, B115200);
      cfsetospeed(&tOptions, B115200);
   }

   if (tcsetattr(mSpecific->mPortFd, TCSANOW, &tOptions) < 0)
   {
      printf("serial_open_error_baud\n", errno);
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doOpen ERROR baud");
      return false;
   }

   // Configure the port for rs485.
   if (mSettings.m485Flag)
   {
      struct serial_rs485 t485conf;
      memset(&t485conf, 0, sizeof(struct serial_rs485));
      t485conf.flags |= SER_RS485_ENABLED;
      t485conf.flags |= SER_RS485_RTS_ON_SEND;
      t485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);
      
      if (ioctl(mSpecific->mPortFd, TIOCSRS485, &t485conf) < 0)
      {
         printf("serial_open_error_485 %d\n", errno);
         Trc::write(mTI, 0, "SerialPort::doOpen ERROR RS485");
         mPortErrorCount++;
         return false;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Flush.

   doFlush();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.
 
   printf("SerialPort open PASS  %s\n", mSettings.mPortDevice);
   mOpenFlag = true;
   mValidFlag = true;
   mOpenErrorShowCount = 0;
   mCloseErrorShowCount = 0;
   Trc::write(mTI, 0, "SerialPort::doOpen done");
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the serial port.

void SerialPort::doClose()
{
   Trc::write(mTI, 0, "SerialPort::doClose");

   // Test if already closed.
   if (!mOpenFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doClose ERROR not open");
      mPortErrorCount++;
      return;
   }

   int tRet = 0;

   // Set invalid.
   mOpenFlag = false;
   mValidFlag = false;

   // Flush the input and output buffers.
   Trc::write(mTI, 1, "serial_close flush");
   doSuspend();
   doFlush();
   //doResume();
   //Ris::Threads::threadSleep(10);

   // Close the event.
   Trc::write(mTI, 1, "SerialPort::doClose close event");
   tRet = close(mSpecific->mEventFd);

   // Test the return code.
   if (tRet != 0)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doClose ERROR 1");
      return;
   }

   // Close the port.
   printf("serial_close port\n");
   tRet = close(mSpecific->mPortFd);

   // Test the return code.
   if (tRet != 0)
   {
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doClose ERROR 2");
      return;
   }
   Trc::write(mTI, 0, "SerialPort::doClose done");

   // Done.
   mSpecific->mEventFd = 0;
   mSpecific->mPortFd = 0;
   mCloseErrorShowCount = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort pending serial port receive.
// 
// Set the abort flag and write bytes to the event semaphore. This will
// cause a pending receive call poll to return and the receive call will
// abort because the abort flag is set.

void SerialPort::doAbort()
{
   Trc::write(mTI, 0, "SerialPort::doAbort");

   // Set the abort flag.
   mAbortFlag = true;

   // Guard.
   int tRet = 0;
   if (!mOpenFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doAbort not open");
      return;
   }

   // Write bytes to the event semaphore.
   unsigned long long tCount = 1;
   tRet = write(mSpecific->mEventFd, &tCount, 8);

   // Test the return code.
   if (tRet < 0)
   {
      Trc::write(mTI, 0, "SerialPort::doAbort ERROR");
      printf("serial_abort_1 ERROR %d\n", errno);
      mPortErrorCount++;
      return;
   }
   Trc::write(mTI, 0, "SerialPort::doAbort done");
   printf("serial_abort done\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Flush serial port buffers.

void SerialPort::doFlush()
{
   Trc::write(mTI, 0, "SerialPort::doFlush");
   // Guard.
   int tRet = 0;
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doFlush INVALID");
      return;
   }

   // Flush the buffers.
   tRet = tcflush(mSpecific->mPortFd, TCIOFLUSH);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_flush ERROR 101 %d\n", errno);
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doFlush ERROR");
      return;
   }
   Trc::write(mTI, 0, "SerialPort::doFlush done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Suspend serial port transmits and receives.

void SerialPort::doSuspend()
{
   Trc::write(mTI, 0, "SerialPort::doSuspend");
   // Guard.
   int tRet = 0;
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doSuspend INVALID");
      return;
   }

   // Suspend output.
   tRet = tcflow(mSpecific->mPortFd, TCOOFF);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_suspend ERROR 1 %d\n", errno);
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doSuspend ERROR");
      return;
   }
   Trc::write(mTI, 0, "SerialPort::doSuspend done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Resume serial port transmits and receives.

void SerialPort::doResume()
{
   Trc::write(mTI, 0, "SerialPort::doResume");
   // Guard.
   int tRet = 0;
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doResume INVALID");
      return;
   }

   // Resume output.
   tRet = tcflow(mSpecific->mPortFd, TCOOFF);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_resume ERROR 101 %d\n", errno);
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doResume ERROR");
      return;
   }
   Trc::write(mTI, 0, "SerialPort::doResume done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the number of bytes that are available to receive.

int SerialPort::doGetAvailableReceiveBytes()
{
   int tBytesAvaiable;
   ioctl(mSpecific->mPortFd, FIONREAD, &tBytesAvaiable);
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
   tPollFd[0].fd = mSpecific->mPortFd;
   tPollFd[0].events = POLLIN;
   tPollFd[0].revents = 0;
   tPollFd[1].fd = mSpecific->mEventFd;
   tPollFd[1].events = POLLIN;
   tPollFd[1].revents = 0;

   // Blocking poll for "there is data to read". This returns when
   // the port has received data or the abort event was posted
   tRet = poll(&tPollFd[0], 2, -1);

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
      //printf("serial_poll_error_2 timeout\n");
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
   tRet = (int)read(mSpecific->mPortFd, aBytes, (size_t)aMaxNumBytes);

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

int SerialPort::doReceiveAllBytes(char* aBytes, int aRequestBytes)
{
   Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes");
   // Guard.
   int tRet = 0;
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes INVALID 1");
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
   Trc::write(mTI, 0, "SerialPort::doReceiveAllBytes done");
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Receive one byte. Block until the byte has been received. Return
// one or zero or a negative error code. Copy the byte into the
// pointer argument.

int SerialPort::doReceiveOneByte(char* aByte)
{
   int tRet = doReceiveAnyBytes(aByte, 1);
   if (tRet > 0)
   {
      mRxByteCount += tRet;
   }
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a fixed number of bytes. Return the actual number of bytes
// sent or a negative error code.

int SerialPort::doSendBytes(const char* aBytes, int aNumBytes)
{
   Trc::write(mTI, 0, "SerialPort::doSendBytes");
   // Guard.
   if (!mValidFlag)
   {
      Trc::write(mTI, 0, "SerialPort::doSendBytes INVALID");
      return cSerialRetError;
   }

   // Local variables.
   int tNumWritten = 0;
   int tRet = 0;

   // Write bytes to the port.
   tRet = write(mSpecific->mPortFd, aBytes, aNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      doAbort();
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doSendBytes ERROR 1");
      return cSerialRetError;
   }

   if (tRet != aNumBytes)
   {
      doAbort();
      mPortErrorCount++;
      Trc::write(mTI, 0, "SerialPort::doSendBytes ERROR 2");
      return cSerialRetError;
   }

   // Write was successful. Return the number of bytes written.
   //printf("SerialPort::doSendBytes PASS %d\n",aNumBytes);
   mTxByteCount += tRet;
   Trc::write(mTI, 0, "SerialPort::doSendBytes done");
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

