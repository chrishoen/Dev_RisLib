/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "Strtok1.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Regional variables.


char* rInput = "formtype=snmp3&sysName=&sysContact=b&sysLocation=a&sysObjectId=&sysDescription=&sysUpTime=&snmpv1-host-1=&snmpv1-port-1=161&snmpv1-community-1=D&snmpv1-host-2=&snmpv1-port-2=161&snmpv1-community-2=D&snmpv1-host-3=&snmpv1-port-3=161&snmpv1-community-3=D&snmpv3-user-1=MotoMaster&snmpv3-user-auth-1=AES&snmpv3-authpass-1=&snmpv3-user-encrypt-1=MD5&snmpv3-user-encryptpass-1=&snmpv3-user-2=MotoMaster&snmpv3-user-auth-2=AES&snmpv3-authpass-2=&snmpv3-user-encryptpass-2=&snmpv3-user-3=MotoMaster&snmpv3-user-auth-3=AES&snmpv3-authpass-3=&snmpv3-user-encrypt-3=MD5&snmpv3-user-encryptpass-3=&snmpv3-trapuser-1=MotoMaster&snmpv3-trapprot-1=UDP&snmpv3-traphost-1=&snmpv3-trapport-1=161&snmpv3-trapuser-2=MotoMaster&snmpv3-trapprot-2=UDP&snmpv3-traphost-2=&snmpv3-trapport-2=161&snmpv3-trapuser-3=MotoMaster&snmpv3-trapprot-3=UDP&snmpv3-traphost-3=&snmpv3-trapport-3=161&@";
char  rBuffer[2000];

// Table of strings split with "&"
char rTable1[100][80];
int  rTable1Size;

// Table of key value pairs derived from Table1.
char rTable2[100][2][40];
int  rTable2Size;


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Test1.

void doStrtokTest1(int aShowCode)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize.

   rTable1Size = 0;
   rTable2Size = 0;

   // Copy the input to the buffer.
   strcpy(rBuffer, rInput);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Split the buffer into Table1. Each row will contain a combined 
   // key,value pair separated by "=".

   char* tToken = strtok(rBuffer, "&");

   int tIndex = 0;
   while (tToken != NULL)
   {
//    printf("%3d %s\n", tIndex, tToken);

      // Copy the token to Table1 at the index.
      if (strlen(tToken) > 1)
      {
         strcpy(rTable1[tIndex], tToken);
         tIndex++;
      }

      // Advance.
      tToken = strtok(NULL, "&");
   }

   // Store the size;
   rTable1Size = tIndex;

   // Show Table1.
   for (int i = 0; i < rTable1Size; i++)
   {
      printf("%3d %s\n", i, &rTable1[i][0]);
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Split the Table1 into Table2. Each row will contain an array[2] of 
   // key,value pairs.

   for (int tRowIndex = 0; tRowIndex < rTable1Size; tRowIndex++)
   {
      // Initialize the Table2 for for a default empty string.
      rTable2[tRowIndex][0][0] = 0;
      rTable2[tRowIndex][1][0] = 0;

      // Copy the row into a temp string.
      char tTable1Row[80];
      strcpy(tTable1Row, &rTable1[tRowIndex][0]);

      // Split the row temp string into a key,value pair and store it in Table2.
      char* tToken = strtok(tTable1Row, "=");
      int tPairIndex = 0;
      while (tToken != NULL)
      {
         printf("PAIR %3d %3d %s\n", tRowIndex, tPairIndex, tToken);

         // Store the token in Table2.
         strcpy(&rTable2[tRowIndex][tPairIndex][0], tToken);
         tPairIndex++;

         // Advance.
         tToken = strtok(NULL, "=");
      }

   }

   // Store the size;
   rTable2Size = rTable1Size;

   // Show Table2.
   for (int i = 0; i < rTable2Size; i++)
   {
      printf("%3d [%s] [%s]\n", i, &rTable2[i][0][0], &rTable2[i][1][0]);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
