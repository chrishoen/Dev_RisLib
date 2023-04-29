#pragma once

#include <windows.h>
#include <stdarg.h>

#ifdef NDEBUG
#undef NDEBUG
#include <assert.h>
#define NDEBUG
#else
#include <assert.h>
#endif

#ifndef MAC_ADDR_LEN
#define MAC_ADDR_LEN 6
#endif


/* Open the ndisprot.sys network driver. Store the handle and the
   mac address in the global variables defined below. This is called by
   the win32 driver initialize.*/
BOOL OpenNdisProtDriver();

/* Initialize the ndisprot.sys read capability. Create and launch the read
   thread. This is called by the win32 driver initialize. The read thread
   reads raw ethernet packets from ndisprot.sys and passes them to the
   network driver for simulated isr processing. This is an overlapped read
   that is used concurrently with the ndisprot.sys write call.*/
BOOL nx_win32_start_read_thread();

/* Write a raw ethernet packet to the ndisprot.sys network driver. This is
   an overlapped write that is used concurrently with the ndisprot.sys
   read thread. */
BOOL nx_win32_write_eth_packet(PVOID data, LONG size);

/* Process a raw ethernet packet that is read from the ndisprot.sys network
   driver. This is called by the read thread when a raw packet is available.
   This is called within the context of the read thread simulated isr
   processing, within context save and restore*/
VOID nx_win32_process_recv_eth_packet(PUCHAR aBytes, LONG aNumBytes);

/* Helpers. */
void myprintf (const char* aFormat, ...);
void show_pkt(const char* aLabel, void* aPacket);

/* Global variables. */
#ifdef _NX_WIN32_DEFS_C_
HANDLE nx_win32_device_handle;
UCHAR  nx_win32_mac_addr[MAC_ADDR_LEN] = { 0 };
BOOL   nx_win32_ndisprot_enable = FALSE;
#else
extern HANDLE nx_win32_device_handle;
extern UCHAR  nx_win32_mac_addr[MAC_ADDR_LEN];
extern BOOL   nx_win32_ndisprot_enable;
#endif
