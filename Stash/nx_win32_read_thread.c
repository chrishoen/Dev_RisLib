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

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "tx_api.h"
#include "nx_api.h"
#include "nx_win32_defs.h"

#define NX_LINK_MTU                 1514
#define NX_MAX_PACKET_SIZE          1536

#pragma warning(disable:6387)

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read thread function. Run a loop that reads raw ethernet packets from
// ndisprot.sys and passes them to the simulated isr processing. 

DWORD WINAPI nx_win32_read_thread(LPVOID* ptr)
{
    myprintf("nx_win32_read_thread 000000000000000000000000000000000000000000000000000000000000000\n");

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************
    // Initialize resources.
       
    // Read completion event.
    HANDLE tReadCompletionEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // Read variables.
    PVOID tBuffer = malloc(NX_MAX_PACKET_SIZE);
    DWORD tNumToRead = NX_MAX_PACKET_SIZE;
    DWORD tNumRead = 0;
    BOOL tWaitingOnRead = FALSE;
    OVERLAPPED tOverlapped = { 0 };
    DWORD tRet = 0;

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************
    // Loop to read a packet or process the periodic timer.

    while (1)
    {
        // Reset Read variables.
        tNumRead = 0;
        tWaitingOnRead = FALSE;
        memset(&tOverlapped, 0, sizeof(tOverlapped));
        tOverlapped.hEvent = tReadCompletionEvent;
        tRet = 0;

        // Read a packet, overlapped i/o.
        tRet = ReadFile(nx_win32_device_handle, tBuffer, tNumToRead, &tNumRead, &tOverlapped);

        // If the read completes immediately.
        if (tRet)
        {
            // Do not wait.
            tWaitingOnRead = FALSE;
        }
        else
        {
            // Check for errors.
            if (GetLastError() != ERROR_IO_PENDING)
            {
                printf("nx_win32_read_thread FAIL1 %d\n", GetLastError());
                assert(0);
                return FALSE;
            }
            // The read is pending.
            else
            {
                // Wait for it.
                tWaitingOnRead = TRUE;
            }
        }

        // If waiting for completion.
        if (tWaitingOnRead)
        {
            // Wait for overlapped i/o completion.
            tRet = WaitForSingleObject(tReadCompletionEvent, INFINITE);

            // Select on the returned status code.
            switch (tRet)
            {
            case WAIT_FAILED:
            {
                printf("nx_win32_read_thread FAIL3 WAIT_FAILED %d\n", GetLastError());
                assert(0);
                return FALSE;
            }
            case WAIT_ABANDONED:
            {
                printf("nx_win32_read_thread FAIL3 WAIT_ABANDONED %d\n", GetLastError());
                assert(0);
                return FALSE;
            }
            case WAIT_OBJECT_0:
            {
                if (!GetOverlappedResult(nx_win32_device_handle, &tOverlapped, &tNumRead, FALSE))
                {
                    printf("nx_win32_read_thread FAIL2 %d\n", GetLastError());
                    assert(0);
                    return FALSE;
                }
                // Success.
                break;
            }
            }
        }

        // Call ThreadX context save for interrupt preparation.
        _tx_thread_context_save();

        // Pass the packet to the simulated isr.
        nx_win32_process_recv_eth_packet(tBuffer, tNumRead);

        // Call ThreadX context restore for interrupt completion.
        _tx_thread_context_restore();
    }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

static HANDLE       nx_win32_read_thread_handle;
static DWORD        nx_win32_read_thread_id;

/* Initialize the ndisprot.sys read capability. Create and launch the read
   thread. This is called by the win32 driver initialize. The read thread
   reads raw ethernet packets from ndisprot.sys and passes them to the
   network driver for simulated isr processing. This is an overlapped read
   that is used concurrently with the ndisprot.sys write call.*/
BOOL nx_win32_start_read_thread()
{
    myprintf("nx_win32_start_read_thread\n");

    /* Create the thread suspended. */
    if ((nx_win32_read_thread_handle =
        CreateThread(NULL, 0, nx_win32_read_thread, (LPVOID)0,
            CREATE_SUSPENDED, &nx_win32_read_thread_id)) == 0)
    {
        printf("nx_win32_start_read_thread FAIL1 %d", GetLastError());
        assert(0);
        return FALSE;
    }

    /* Set the thread priority. */
    if (!SetThreadPriority(nx_win32_read_thread_handle, THREAD_PRIORITY_BELOW_NORMAL))
    {
        printf("nx_win32_start_read_thread FAIL2 %d", GetLastError());
        assert(0);
        return FALSE;
    }

    /* Resume the thread to launch it. */
    if (!ResumeThread(nx_win32_read_thread_handle))
    {
        printf("nx_win32_start_read_thread FAIL3 %d", GetLastError());
        assert(0);
        return FALSE;
    }

    /* Done. */
    myprintf("nx_win32_start_read_thread PASS\n");
    return TRUE;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
