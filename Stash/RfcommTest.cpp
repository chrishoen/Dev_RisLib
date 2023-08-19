// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//
// BTHCxnDemo.cpp - Simple Bluetooth application using Winsock 2.2 and RFCOMM protocol
//
//
//      This example demonstrates how to use Winsock-2.2 APIs to connect
//      between two remote bluetooth devices and transfer data between them.
//      This example only supports address family of type AF_BTH,
//      socket of type SOCK_STREAM, protocol of type BTHPROTO_RFCOMM.
//
//      Once this source code is built, the resulting application can be
//      run either in server mode or in client mode.  See command line help
//      for command-line-examples and detailed explanation about all options.
//

#include <stdio.h>
#include <initguid.h>
#include <winsock2.h>
#include <ws2bth.h>
#include <strsafe.h>
#include <intsafe.h>
#include <bluetoothapis.h>
#include "RfcommTest.h"


// {B62C4E8D-62CC-404b-BBBF-BF3E3BBB1374}
//                                B62C4E8D    62CC    404b    BB    BF    BF   3E     3B    BB   13     74
DEFINE_GUID(g_guidServiceClas2, 0xb62c4e8d, 0x62cc, 0x404b, 0xbb, 0xbf, 0xbf, 0x3e, 0x3b, 0xbb, 0x13, 0x74);

// {00001101-0000-1000-8000-00805F9B34FB}
//                                00001101    0000    1000    80    00    00    80    5F    9B    34    FB}
DEFINE_GUID(g_guidServiceClass, 0x00001101, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb);

#define CXN_TEST_DATA_STRING              ("ABCDEFGH")
#define CXN_TRANSFER_DATA_LENGTH          (sizeof(CXN_TEST_DATA_STRING))


#define CXN_BDADDR_STR_LEN                17   // 6 two-digit hex values plus 5 colons
#define CXN_MAX_INQUIRY_RETRY             3
#define CXN_DELAY_NEXT_INQUIRY            15
#define CXN_SUCCESS                       0
#define CXN_ERROR                         1
#define CXN_DEFAULT_LISTEN_BACKLOG        4



char g_szRemoteName[BTH_MAX_NAME_SIZE + 1] = { 0 };  // 1 extra for trailing NULL character
char g_szRemoteAddr[CXN_BDADDR_STR_LEN + 1] = { 0 }; // 1 extra for trailing NULL character
int  g_ulMaxCxnCycles = 1;



ULONG NameToBthAddr(_In_ const LPSTR pszRemoteName, _Out_ PSOCKADDR_BTH pRemoteBthAddr);
ULONG RunClientMode(_In_ SOCKADDR_BTH ululRemoteBthAddr, _In_ int iMaxCxnCycles = 1);


void rfcomm_test1()
{
   ULONG       ulRetCode = CXN_SUCCESS;
   WSADATA     WSAData = { 0 };
   SOCKADDR_BTH RemoteBthAddr = { 0 };

   strcpy(g_szRemoteName, "Stenograph Writer");
   //
   // Ask for Winsock version 2.2.
   //
   if (CXN_SUCCESS == ulRetCode)
   {
      ulRetCode = WSAStartup(MAKEWORD(2, 2), &WSAData);
      if (CXN_SUCCESS != ulRetCode)
      {
         printf("-FATAL- | Unable to initialize Winsock version 2.2\n");
      }
   }

   if (CXN_SUCCESS == ulRetCode)
   {

      //
      // Note, this app "prefers" the name if provided, but it is app-specific
      // Other applications may provide more generic treatment.
      //
      if ('\0' != g_szRemoteName[0])
      {
         //
         // Get address from the name of the remote device and run the application
         // in client mode
         //
         ulRetCode = NameToBthAddr(g_szRemoteName, &RemoteBthAddr);
         if (CXN_SUCCESS != ulRetCode)
         {
            printf("-FATAL- | Unable to get address of the remote radio having name %s\n", g_szRemoteName);
         }

         if (CXN_SUCCESS == ulRetCode)
         {
            ulRetCode = RunClientMode(RemoteBthAddr, g_ulMaxCxnCycles);
         }

      }
      else if ('\0' != g_szRemoteAddr[0])
      {

         //
         // Get address from formated address-string of the remote device and
         // run the application in client mode
         //
         int iAddrLen = sizeof(RemoteBthAddr);
         ulRetCode = WSAStringToAddressA(g_szRemoteAddr,
            AF_BTH,
            NULL,
            (LPSOCKADDR)&RemoteBthAddr,
            &iAddrLen);
         if (CXN_SUCCESS != ulRetCode)
         {
            printf("-FATAL- | Unable to get address of the remote radio having formated address-string %s\n", g_szRemoteAddr);
         }

         if (CXN_SUCCESS == ulRetCode)
         {
            ulRetCode = RunClientMode(RemoteBthAddr, g_ulMaxCxnCycles);
         }

      }
      else
      {
         //
         // No remote name/address specified.  Run the application in server mode
         //
         printf("FAIL BAD\n");
      }
   }

   printf("DONE %d\n", (int)ulRetCode);
}


//
// NameToBthAddr converts a bluetooth device name to a bluetooth address,
// if required by performing inquiry with remote name requests.
// This function demonstrates device inquiry, with optional LUP flags.
//
ULONG NameToBthAddr(_In_ const LPSTR pszRemoteName, _Out_ PSOCKADDR_BTH pRemoteBtAddr)
{
   INT             iResult = CXN_SUCCESS;
   BOOL            bContinueLookup = FALSE, bRemoteDeviceFound = FALSE;
   ULONG           ulFlags = 0;
   ULONG           ulPQSSize = sizeof(WSAQUERYSET);
   HANDLE          hLookup = NULL;
   PWSAQUERYSET    pWSAQuerySet = NULL;

   ZeroMemory(pRemoteBtAddr, sizeof(*pRemoteBtAddr));

   pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
      HEAP_ZERO_MEMORY,
      ulPQSSize);
   if (NULL == pWSAQuerySet)
   {
      iResult = STATUS_NO_MEMORY;
      printf("!ERROR! | Unable to allocate memory for WSAQUERYSET\n");
   }

   //
   // Search for the device with the correct name
   //

   if (CXN_SUCCESS == iResult)
   {

      for (INT iRetryCount = 0;
         !bRemoteDeviceFound && (iRetryCount < CXN_MAX_INQUIRY_RETRY);
         iRetryCount++)
      {

         if (0 == iRetryCount)
         {
            printf("*INFO* | Inquiring device from cache...\n");
         }
         else
         {
            //
            // Flush the device cache for all inquiries, except for the first inquiry
            //
            // By setting LUP_FLUSHCACHE flag, we're asking the lookup service to do
            // a fresh lookup instead of pulling the information from device cache.
            //
            ulFlags |= LUP_FLUSHCACHE;

            //
            // Pause for some time before all the inquiries after the first inquiry
            //
            // Remote Name requests will arrive after device inquiry has
            // completed.  Without a window to receive IN_RANGE notifications,
            // we don't have a direct mechanism to determine when remote
            // name requests have completed.
            //
            printf("*INFO* | Unable to find device.  Waiting for %d seconds before re-inquiry...\n", CXN_DELAY_NEXT_INQUIRY);
            Sleep(CXN_DELAY_NEXT_INQUIRY * 1000);

            printf("*INFO* | Inquiring device ...\n");
         }

         //
         // Start the lookup service
         //
         iResult = CXN_SUCCESS;
         hLookup = 0;
         bContinueLookup = FALSE;
         ZeroMemory(pWSAQuerySet, ulPQSSize);
         pWSAQuerySet->dwNameSpace = NS_BTH;
         pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);

         ulFlags =
            LUP_CONTAINERS |
            LUP_FLUSHCACHE |
            LUP_RETURN_NAME |
            LUP_RETURN_ADDR;

         iResult = WSALookupServiceBegin(pWSAQuerySet, ulFlags, &hLookup);

         //
         // Even if we have an error, we want to continue until we
         // reach the CXN_MAX_INQUIRY_RETRY
         //
         if ((NO_ERROR == iResult) && (NULL != hLookup))
         {
            bContinueLookup = TRUE;
         }
         else if (0 < iRetryCount)
         {
            printf("=CRITICAL= | WSALookupServiceBegin() failed with error code %d, WSAGetLastError = %d\n", iResult, WSAGetLastError());
            break;
         }

         while (bContinueLookup)
         {
            //
            // Get information about next bluetooth device
            //
            // Note you may pass the same WSAQUERYSET from LookupBegin
            // as long as you don't need to modify any of the pointer
            // members of the structure, etc.
            //
            // ZeroMemory(pWSAQuerySet, ulPQSSize);
            // pWSAQuerySet->dwNameSpace = NS_BTH;
            // pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
            if (NO_ERROR == WSALookupServiceNext(hLookup,
               ulFlags,
               &ulPQSSize,
               pWSAQuerySet))
            {

               //
               // Compare the name to see if this is the device we are looking for.
               //
               if ((pWSAQuerySet->lpszServiceInstanceName != NULL) &&
                  (CXN_SUCCESS == strcmp(pWSAQuerySet->lpszServiceInstanceName, pszRemoteName)))
               {
                  //
                  // Found a remote bluetooth device with matching name.
                  // Get the address of the device and exit the lookup.
                  //
                  CopyMemory(pRemoteBtAddr,
                     (PSOCKADDR_BTH)pWSAQuerySet->lpcsaBuffer->RemoteAddr.lpSockaddr,
                     sizeof(*pRemoteBtAddr));
                  bRemoteDeviceFound = TRUE;
                  bContinueLookup = FALSE;
                  printf("SHOW WSAQuerySet\n");
                  printf("SHOW SockAddrBthServer.btAddr         %llx\n", pRemoteBtAddr->btAddr);
                  printf("SHOW SockAddrBthServer.port           %d\n", pRemoteBtAddr->port);
               }
            }
            else
            {
               iResult = WSAGetLastError();
               if (WSA_E_NO_MORE == iResult)
               { //No more data
                   //
                   // No more devices found.  Exit the lookup.
                   //
                  bContinueLookup = FALSE;
               }
               else if (WSAEFAULT == iResult)
               {
                  //
                  // The buffer for QUERYSET was insufficient.
                  // In such case 3rd parameter "ulPQSSize" of function "WSALookupServiceNext()" receives
                  // the required size.  So we can use this parameter to reallocate memory for QUERYSET.
                  //
                  HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
                  pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
                     HEAP_ZERO_MEMORY,
                     ulPQSSize);
                  if (NULL == pWSAQuerySet)
                  {
                     printf("!ERROR! | Unable to allocate memory for WSAQERYSET\n");
                     iResult = STATUS_NO_MEMORY;
                     bContinueLookup = FALSE;
                  }
               }
               else
               {
                  printf("=CRITICAL= | WSALookupServiceNext() failed with error code %d\n", iResult);
                  bContinueLookup = FALSE;
               }
            }
         }

         //
         // End the lookup service
         //
         WSALookupServiceEnd(hLookup);

         if (STATUS_NO_MEMORY == iResult)
         {
            break;
         }
      }
   }

   if (NULL != pWSAQuerySet)
   {
      HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
      pWSAQuerySet = NULL;
   }

   if (bRemoteDeviceFound)
   {
      iResult = CXN_SUCCESS;
   }
   else
   {
      iResult = CXN_ERROR;
   }

   return iResult;
}

//
// RunClientMode runs the application in client mode.  It opens a socket, connects it to a
// remote socket, transfer some data over the connection and closes the connection.
//
ULONG RunClientMode(_In_ SOCKADDR_BTH RemoteAddr, _In_ int iMaxCxnCycles)
{
   ULONG           ulRetCode = CXN_SUCCESS;
   int             iCxnCount = 0;
   CHAR* pszData = NULL;
   SOCKET          LocalSocket = INVALID_SOCKET;
   SOCKADDR_BTH    SockAddrBthServer = RemoteAddr;

   pszData = (PCHAR)malloc(CXN_TRANSFER_DATA_LENGTH);
   if (!pszData) return 0;

   //
   // Setting address family to AF_BTH indicates winsock2 to use Bluetooth sockets
   // Port should be set to 0 if ServiceClassId is spesified.
   //
   SockAddrBthServer.addressFamily = AF_BTH;
   SockAddrBthServer.serviceClassId = g_guidServiceClass;
   SockAddrBthServer.port = BT_PORT_DYN_FIRST;
   SockAddrBthServer.port = BT_PORT_ANY;
   SockAddrBthServer.port = 1;

   //
   // Create a static data-string, which will be transferred to the remote
   // Bluetooth device
   //
   strcpy(pszData, CXN_TEST_DATA_STRING);

   if (CXN_SUCCESS == ulRetCode)
   {
      printf("\n");

      //
      // Open a bluetooth socket using RFCOMM protocol
      //
      LocalSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
      if (INVALID_SOCKET == LocalSocket)
      {
         printf("=CRITICAL= | socket() call failed. WSAGetLastError = [%d]\n", WSAGetLastError());
         ulRetCode = CXN_ERROR;
         return(ulRetCode);
      }

      //
      // Connect the socket (pSocket) to a given remote socket represented by address (pServerAddr)
      //
      bool tChanFound = false;
      int tChan = 0;
      for (tChan = 0; tChan < 1; tChan++)
      {
         SockAddrBthServer.port = tChan;
         SockAddrBthServer.port = BT_PORT_ANY;
         printf("SHOW Connect\n");
         printf("SHOW SockAddrBthServer.btAddr         %llx\n", SockAddrBthServer.btAddr);
         printf("SHOW SockAddrBthServer.port           %d\n", SockAddrBthServer.port);
         if (SOCKET_ERROR == connect(LocalSocket,
            (struct sockaddr*)&SockAddrBthServer,
            sizeof(SOCKADDR_BTH))) {
            printf("=CRITICAL= | connect() call failed. WSAGetLastError=[%d]\n", WSAGetLastError());
            ulRetCode = CXN_ERROR;
         }
         else
         {
            tChanFound = true;
            break;
         }
      }
      if (tChanFound)
      {
         printf("SHOW Connect PASS %d\n", tChan);
         Sleep(2000);
      }
      else
      {
         printf("SHOW Connect NOT FOUND\n");
      }

      //
      // Close the socket
      //
      if (SOCKET_ERROR == closesocket(LocalSocket))
      {
         printf("=CRITICAL= | closesocket() call failed w/socket = [0x%I64X]. WSAGetLastError=[%d]\n", (ULONG64)LocalSocket, WSAGetLastError());
         ulRetCode = CXN_ERROR;
         return(ulRetCode);
      }

      LocalSocket = INVALID_SOCKET;
   }

   if (INVALID_SOCKET != LocalSocket)
   {
      closesocket(LocalSocket);
      LocalSocket = INVALID_SOCKET;
   }

   if (NULL != pszData)
   {
      free(pszData);
      pszData = NULL;
   }

   return(ulRetCode);
}

