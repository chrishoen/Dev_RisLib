#include "stdafx.h"

#include <windows.h>

#include "MainInit.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

BOOL CtrlHandler( DWORD fdwCtrlType ); 

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   if( SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE ) ) 
   { 
      printf( "\nThe Control Handler is installed.\n" ); 
   } 
   else 
   {
      printf( "\nERROR: Could not set control handler"); 
      return 1;
   }

   printf("first line in main\n");
   //--------------------------------------------------------------------
   // Begin program

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Start user command line executive,
   // Wait for user to exit

   CmdLineExec* exec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(exec);
   delete exec;

   //--------------------------------------------------------------------
   // End program

   printf("last line in main\n");
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BOOL CtrlHandler( DWORD fdwCtrlType ) 
{ 
  switch( fdwCtrlType ) 
  { 
    // Handle the CTRL-C signal. 
    case CTRL_C_EVENT: 
      printf( "Ctrl-C event\n\n" );
      Beep( 750, 300 ); 
      return( TRUE );
 
    // CTRL-CLOSE: confirm that the user wants to exit. 
    case CTRL_CLOSE_EVENT: 
      Beep( 600, 200 ); 
      printf( "Ctrl-Close event\n\n" );
      Sleep(4000);
      printf( "Ctrl-Close event\n\n" );
      return( TRUE ); 
 
    // Pass other signals to the next handler. 
    case CTRL_BREAK_EVENT: 
      Beep( 900, 200 ); 
      printf( "Ctrl-Break event\n\n" );
      return FALSE; 
 
    case CTRL_LOGOFF_EVENT: 
      Beep( 1000, 200 ); 
      printf( "Ctrl-Logoff event\n\n" );
      return FALSE; 
 
    case CTRL_SHUTDOWN_EVENT: 
      Beep( 750, 500 ); 
      printf( "Ctrl-Shutdown event\n\n" );
      return FALSE; 
 
    default: 
      return FALSE; 
  } 
} 
