/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/
#pragma warning(disable:4201)   // nameless struct/union
#pragma warning(disable:4127)   // conditional expression is constant

#include <windows.h>

#include "tx_api.h"
#include "nx_api.h"
#include "nx_win32_defs.h"

#pragma warning(disable:6011)
#pragma warning(disable:6387)

// Write a raw ethernet packet to the ndisprot.sys network driver. This is
// an overlapped write that is used concurrently with the ndisprot.sys
// read thread.
BOOL nx_win32_write_eth_packet(PVOID aData, LONG aNumBytes)
{
    // If the link is disabled then drop the packet.
    if (!nx_win32_ndisprot_enable)
    {
        myprintf("nx_win32_write_eth_packet DROP\n");
        return TRUE;
    }

    show_pkt("nx_win32_write_eth_packet", aData);

    // Event.
    static HANDLE tWriteCompletionEvent = 0;
    if (tWriteCompletionEvent == 0) tWriteCompletionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // Local variables.
    DWORD tRet = 0;
    DWORD tNumWritten = 0;
    BOOL tWaitingOnWrite = FALSE;
    OVERLAPPED tOverlapped = { 0 };
    tOverlapped.hEvent = tWriteCompletionEvent;

    // Write a packet, overlapped i/o.
    tRet = WriteFile(nx_win32_device_handle, aData, aNumBytes, &tNumWritten, &tOverlapped);

    // If the write completes immediately.
    if (tRet)
    {
        // Do not wait.
        tWaitingOnWrite = FALSE;
    }
    else
    {
        // Check for errors.
        if (GetLastError() != ERROR_IO_PENDING)
        {
            printf("nx_win32_write_eth_packet FAIL1 %d\n", GetLastError());
            assert(0);
            return FALSE;
        }
        // The write is pending.
        else
        {
            // Wait for it.
            tWaitingOnWrite = TRUE;
        }
    }

    // If waiting for completion.
    if (tWaitingOnWrite)
    {
        // Wait for overlapped i/o completion.
        tRet = WaitForSingleObject(tWriteCompletionEvent, INFINITE);

        // Select on the returned status code.
        switch (tRet)
        {
        case WAIT_FAILED:
        {
            printf("nx_win32_write_eth_packet FAIL3 WAIT_FAILED %d\n", GetLastError());
            assert(0);
            return FALSE;
        }
        case WAIT_ABANDONED:
        {
            printf("nx_win32_write_eth_packet FAIL3 WAIT_ABANDONED %d\n", GetLastError());
            assert(0);
            return FALSE;
        }
        case WAIT_OBJECT_0:
        {
            if (!GetOverlappedResult(nx_win32_device_handle, &tOverlapped, &tNumWritten, FALSE))
            {
                printf("nx_win32_write_eth_packet FAIL2 %d\n", GetLastError());
                assert(0);
                return FALSE;
            }
            // Success.
            break;
        }
        }
    }
    // Done.
    return TRUE;
}


