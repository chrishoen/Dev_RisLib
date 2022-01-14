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
#include "prnPrint.h"

#include "risSerialPort2.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Portable specifics.

class SerialPort2::Specific
{
public:
   int mPortFd;
   int mEventFd;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialPort2::SerialPort2()
{
   mSpecific = new Specific;
   mSpecific->mPortFd = 0;
   mSpecific->mEventFd = 0;
   mValidFlag = false;
   mAbortFlag = false;
}

SerialPort2::~SerialPort2()
{
   doClose();
   delete mSpecific;
}

void SerialPort2::initialize(SerialSettings& aSettings)
{
   mSettings = aSettings;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Open the serial port with the settings.
//
// Configure the port for raw, set the baud rate, and optionally 
// select RS485.

bool SerialPort2::doOpen()
{
   // Do this first.
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
      printf("serial_open_error_1 %d %s\n", errno, strerror(errno));
      return false;
   }

   // Open the event.
   mSpecific->mEventFd = eventfd(0, EFD_SEMAPHORE);

   // Test the return code.
   if (mSpecific->mPortFd < 0)
   {
      printf("serial_open_error_2 %d\n", errno);
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
      cfsetispeed(&tOptions, B38400);
      cfsetospeed(&tOptions, B38400);
   }

   if (tcsetattr(mSpecific->mPortFd, TCSANOW, &tOptions) < 0)
   {
      printf("serial_open_error_baud\n", errno);
      return false;
   }

   // Configure the port for rs485.
   if (mSettings.m485Flag)
   {
      struct serial_rs485 t485conf = { 0 };
      t485conf.flags |= SER_RS485_ENABLED;
      t485conf.flags |= SER_RS485_RTS_ON_SEND;
      t485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);
      
      if (ioctl(mSpecific->mPortFd, TIOCSRS485, &t485conf) < 0)
      {
         printf("serial_open_error_485 %d\n", errno);
         return false;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Flush.

   Ris::Threads::threadSleep(100);
   doFlush();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.
 
#if 0
   if (!mSettings.m485Flag)
   {
      printf("SerialPort2 initialize PASS  $ %s : %16s\n",
         mSettings.mPortDevice,
         mSettings.mPortSetup);
   }
   else
   {
      printf("SerialPort2 initialize PASS  $ %s : %16s RS485\n",
         mSettings.mPortDevice,
         mSettings.mPortSetup);
   }
#endif

   mValidFlag = true;
   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close the serial port.

void SerialPort2::doClose()
{
   int tRet = 0;
   if (!mValidFlag) return;

   // Set invalid.
   mValidFlag = false;

   // Close the event.
   tRet = close(mSpecific->mEventFd);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_close_error_1 %d\n", errno);
   }

   // Close the port.
   tRet = close(mSpecific->mPortFd);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_close_error_2 %d\n", errno);
   }

   // Done.
   mSpecific->mEventFd = 0;
   mSpecific->mPortFd = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Abort pending serial port receive.
// 
// Set the abort flag and write bytes to the event semaphore. This will
// cause a pending receive call poll to return and the receive call will
// abort because the abort flag is set.

void SerialPort2::doAbort()
{
   // Guard.
   int tRet = 0;
   if (!mValidFlag) return;

   // Set the abort flag.
   mAbortFlag = true;

   // Write bytes to the event semaphore.
   unsigned long long tCount = 1;
   tRet = write(mSpecific->mEventFd, &tCount, 8);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_abort_1 %d\n", errno);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Flush serial port buffers.

void SerialPort2::doFlush()
{
   return;

   // Guard.
   int tRet = 0;
   if (!mValidFlag) return;

   // Flush the buffers.
   tRet = tcflush(mSpecific->mPortFd, TCIFLUSH);

   // Test the return code.
   if (tRet != 0)
   {
      printf("serial_flush_1 %d\n", errno);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Send a number of bytes, return the actual number of bytes sent.

int SerialPort2::doSendBytes(const char* aData, int aNumBytes)
{
   // Guard.
   if (!mValidFlag) return cSerialRetError;

   // Local variables.
   int tNumWritten = 0;
   int tRet  = 0;

   // Write bytes to the port.
   tRet = write(mSpecific->mPortFd, aData, aNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      printf("serial_write_error_1 %d\n", errno);
      return cSerialRetError;
   }

   if (tRet != aNumBytes)
   {
      printf("serial_write_error_2 %d\n", tRet);
      return cSerialRetError;
   }

   // Write was successful. Return the number of bytes written.
   //printf("SerialPort2::doSendBytes PASS %d\n",aNumBytes);
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read any available receive bytes. Block until at least one byte has
// been received. Return the number of bytes read or a negative error code.

int SerialPort2::doReadAnyBytes(char* aData, int aMaxNumBytes)
{
   // Guard.
   if (!mValidFlag) return cSerialRetError;
   int tRet = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Set the number of bytes to read.
#if 0
   struct termios tOptions;
   tcgetattr(mSpecific->mPortFd, &tOptions);
   tOptions.c_cc[VMIN] = aNumBytes;
   tcsetattr(mSpecific->mPortFd, TCSANOW, &tOptions);
#endif
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
   if (!mAbortFlag)
   {
      //printf("serial_poll_abort\n");
      return cSerialRetAbort;
   }

   // Test the return code for error.
   if (tRet < 0)
   {
      //printf("serial_poll_error_1 %d\n", errno);
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
      //printf("serial_poll_error_3 close\n");
      return cSerialRetError;
   }

   //printf("serial_poll_pass %d\n",tRet);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Read from the serial port.

   // Read.
   tRet = (int)read(mSpecific->mPortFd, aData, (size_t)aMaxNumBytes);

   // Test the return code.
   if (tRet < 0)
   {
      printf("serial_read_error_1 %d\n", errno);
      return cSerialRetError;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Done.

   // Read was successful. Return the number of bytes read.
   return tRet;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return the number of bytes that are available to receive.

int SerialPort2::getAvailableReceiveBytes()
{
   int tBytesAvaiable;
   ioctl(mSpecific->mPortFd, FIONREAD, &tBytesAvaiable);
   return tBytesAvaiable;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

