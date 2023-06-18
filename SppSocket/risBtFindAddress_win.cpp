#include "stdafx.h"

#include <stdio.h>
#include <initguid.h>
#include <winsock2.h>
#include <ws2bth.h>
#include <strsafe.h>
#include <intsafe.h>
#include <bluetoothapis.h>
#include "risBtFindAddress.h"

// SPP UUID 1101
// {00001101-0000-1000-8000-00805F9B34FB}
//                                00001101    0000    1000    80    00    00    80    5F    9B    34    FB}
DEFINE_GUID(g_guidServiceClass, 0x00001101, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb);


#define CXN_BDADDR_STR_LEN                17   // 6 two-digit hex values plus 5 colons
#define CXN_MAX_INQUIRY_RETRY             3
#define CXN_DELAY_NEXT_INQUIRY            15
#define CXN_SUCCESS                       0
#define CXN_ERROR                         1
#define CXN_DEFAULT_LISTEN_BACKLOG        4


namespace Ris
{
namespace BtSockets
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This was derived from sample btcxn.cpp
/// ULONG NameToBthAddr(_In_ const LPSTR pszRemoteName, _Out_ PSOCKADDR_BTH pRemoteBthAddr);
//
bool doFindAddressFromName(BtSocketAddress* aSocketAddress, const char* aRemoteName)
{
   aSocketAddress->reset();

   const LPSTR pszRemoteName = (LPSTR)aRemoteName;
   PSOCKADDR_BTH pRemoteBtAddr = &aSocketAddress->mBtAddress;

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
            //printf("*INFO* | Inquiring device from cache...\n");
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
                  //printf("SHOW WSAQuerySet\n");
                  //printf("SHOW SockAddrBthServer.btAddr         %llx\n", pRemoteBtAddr->btAddr);
                  //printf("SHOW SockAddrBthServer.port           %d\n", pRemoteBtAddr->port);
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
      aSocketAddress->mValid = true;
   }
   else
   {
      iResult = CXN_ERROR;
      aSocketAddress->mValid = false;
   }

   return (iResult == CXN_SUCCESS);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
