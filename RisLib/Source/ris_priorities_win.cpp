/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h>

#include "ris_priorities.h"

//******************************************************************************
namespace Ris
{

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   /*
     THREAD_PRIORITY_TIME_CRITICAL;
     THREAD_PRIORITY_HIGHEST;
     THREAD_PRIORITY_ABOVE_NORMAL;
     THREAD_PRIORITY_NORMAL;
     THREAD_PRIORITY_BELOW_NORMAL;
     THREAD_PRIORITY_LOWEST;
     THREAD_PRIORITY_IDLE;
     */

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // RisLib default thread priorities

   int get_default_thread_priority()                      { return THREAD_PRIORITY_NORMAL; }
   int get_default_high_thread_priority()                 { return THREAD_PRIORITY_TIME_CRITICAL; }
   int get_default_low_thread_priority()                  { return THREAD_PRIORITY_LOWEST; }

   int get_default_qcall_thread_priority()                { return THREAD_PRIORITY_NORMAL; }
   int get_default_qcall_timer_thread_priority()          { return THREAD_PRIORITY_ABOVE_NORMAL; }

   int get_default_short_thread_priority()                { return THREAD_PRIORITY_HIGHEST; }
   int get_default_short_timer_thread_priority()          { return THREAD_PRIORITY_TIME_CRITICAL; }
   int get_default_long_thread_priority()                 { return THREAD_PRIORITY_ABOVE_NORMAL; }

   int get_default_udp_rx_thread_priority()               { return THREAD_PRIORITY_HIGHEST; }
   int get_default_tcp_server_thread_priority()           { return THREAD_PRIORITY_HIGHEST; }
   int get_default_tcp_client_thread_priority()           { return THREAD_PRIORITY_HIGHEST; }

}//namespace