#include "stdafx.h"

#include <windows.h>
#include "prnPrint.h"

#include "Experiment.h"

namespace Experiment
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Regionals

   HANDLE mCompletionPort;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize

   void initialize()
   {
      mCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,NULL);

      if (mCompletionPort == NULL)
      {
         Prn::print(0,"CreateIoCompletionPort FAIL");
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Send

   void send()
   {
      DWORD        tNumberOfBytes=101;
      ULONG_PTR    tCompletionKey=0;
      LPOVERLAPPED tOverlapped=0;

      BOOL tStatus = PostQueuedCompletionStatus(
         mCompletionPort,
         tNumberOfBytes,
         tCompletionKey,
         tOverlapped);

      if (tStatus == NULL)
      {
         Prn::print(0,"PostQueuedCompletionStatus FAIL");
      }

   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Receive

   void receive()
   {
      DWORD        tNumberOfBytes=0;
      ULONG_PTR    tCompletionKey=0;
      LPOVERLAPPED tOverlapped=0;

      BOOL tStatus = GetQueuedCompletionStatus(
         mCompletionPort,
        &tNumberOfBytes,
        &tCompletionKey,
        &tOverlapped,
         INFINITE);

      if (tStatus == NULL)
      {
         Prn::print(0,"GetQueuedCompletionStatus FAIL");
      }
   }
}