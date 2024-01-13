/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <time.h>

#include "risPortableCalls.h"
#include "my_functions.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Min,Max

double my_fmin (double aA,double aB) {return aA > aB ? aB : aA;}
double my_fmax (double aA,double aB) {return aA > aB ? aA : aB;}

int    my_imin (int aA,int aB) {return aA > aB ? aB : aA;}
int    my_imax (int aA, int aB) { return aA > aB ? aA : aB; }
int    my_iabs (int aA) { return aA >=0  ? aA : -aA; }

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close to

bool my_iscloseto (double aA,double aB,double aResolution)
{
   return (fabs(aA - aB) <= aResolution);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Round to nearest integer

int my_round (double aA)
{
   if   (aA >= 0.0)  return int(aA + 0.5);
   else              return int(aA - 0.5);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Random numbers

// Seeds random with current time
void my_srand()
{
   unsigned int tTime = (unsigned int) (Ris::portableGetHiResCounter() &0xffffffff);
   srand(tTime);
}

// Return random double 0.0 <= x < 1.0
double my_frand()
{
   return double(rand())/RAND_MAX;
}

// Return random double A <= x < B
double my_frand(double aA,double aB)
{
   double tR=my_frand();
   return aA + (aB-aA)*tR; 
}

// Return random int 0 <= x < A
int my_irand(int aA)
{
   return rand() % aA;
}

// Return random int A <= x <= B
int my_irand(int aA,int aB)
{
   return aA + rand() % (aB - aA + 1);
}

// Return random flag with probability P
bool my_randflag(double aP)
{
   return (double(rand())/RAND_MAX) <= aP;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Strings

// Trim CRLF from the end of a string. CRLF (\r\n,13,10)
int my_trimCRLF(char* aString)
{
   unsigned tStringLen = (unsigned)strlen(aString);
   int tCode = 0;
   if (tStringLen == 0) return 0;
   if (tStringLen == 1)
   {
      if (aString[0] == 10)
      {
         aString[0] = 0;
         tCode++;
      }
      if (aString[0] == 13)
      {
         aString[0] = 0;
         tCode++;
      }
   }
   else if (tStringLen >= 2)
   {
      if (aString[tStringLen - 1] == 10)
      {
         aString[tStringLen - 1] = 0;
         tCode++;

         if (aString[tStringLen - 2] == 13)
         {
            aString[tStringLen - 2] = 0;
            tCode++;
         }
      }
   }
   return tCode;
}

// Convert a string to uppercase.
void my_string_toupper(char* aString)
{
   for (int i = 0; i < (int)strlen(aString); i++)
   {
      aString[i] = toupper(aString[i]);
   }
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Strings

// Return string "true" "false" for a bool
char* my_string_from_bool(bool aValue)
{
   return aValue ? (char*)"true" : (char*)"false";
}

// Return string "true" "false" for a bool
char* my_string_from_true(bool aValue)
{
   return aValue ? (char*)"true" : (char*)"false";
}

// Return string "pass" "fail" for a bool
char* my_string_from_pass(bool aValue)
{
   return aValue ? (char*)"PASS" : (char*)"FAIL";
}

// Return string "High" "Low" for a bool
char* my_string_from_high(bool aValue)
{
   return aValue ? (char*)"High" : (char*)"Low";
}

// Return formatted string from float.
char* my_string_from_float(char* aString, const char* aFormat, float aValue)
{
   sprintf(aString, aFormat, aValue);
   return aString;
}

// Return formatted string from double.
char* my_string_from_double(char* aString, const char* aFormat, double aValue)
{
   sprintf(aString, aFormat, aValue);
   return aString;
}

// Return string from bluetooth mac address.
typedef struct _MY_BLUETOOTH_ADDRESS
{
   union
   {
      unsigned long long ullLong;
      unsigned char rgBytes[6];
   };
} MY_BLUETOOTH_ADDRESS_STRUCT;

char* my_string_from_btaddr(char* aString, unsigned long long aValue)
{
   MY_BLUETOOTH_ADDRESS_STRUCT tTemp;
   tTemp.ullLong = aValue;

   sprintf(aString, "%02x:%02x:%02x:%02x:%02x:%02x\n",
      tTemp.rgBytes[5],
      tTemp.rgBytes[4],
      tTemp.rgBytes[3],
      tTemp.rgBytes[2],
      tTemp.rgBytes[1],
      tTemp.rgBytes[0]);

   return aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Time conversions

double my_time_from_string(char* aString)
{
#if 0
   int tLength = (int)strlen(aString);
   char tString[40];
   double tTime=0.0;

   switch (aString[tLength-1])
   {
   case 's' :
   case 'S' :
      strncpy(tString,aString,tLength-1);
      tTime = atof(tString);
      break;
   case 'm' :
   case 'M' :
      strncpy(tString,aString,tLength-1);
      tTime = atof(tString);
      tTime *= 60.0;
      break;
   case 'h' :
   case 'H' :
      strncpy(tString,aString,tLength-1);
      tTime = atof(tString);
      tTime *= 3600.0;
      break;
   default:
      tTime = atof(aString);
      break;
   }
   return tTime;
#endif
   return 0;
}

char*  my_string_from_time(double aTime, char* aBuffer)
{
   int     tTimeSeconds  = (int)aTime;
// double  tTimeSecondsF = aTime;

   int  tSecInOneMin   = 60;
   int  tSecInOneHour  = 60*60;
   int  tSecInOneDay   = 60*60*24;

   int  tSecInDay  = tTimeSeconds  % tSecInOneDay;
   int  tSecInHour = tTimeSeconds  % tSecInOneHour;
   int  tSecInMin  = tTimeSeconds  % tSecInOneMin;

   int  tHourInDay = tSecInDay  / tSecInOneHour;
   int  tMinInHour = tSecInHour / tSecInOneMin;

   sprintf(aBuffer,"%02d:%02d:%02d",tHourInDay,tMinInHour,tSecInMin);
   return aBuffer;
}

// Convert seconds to millisecond ticks
int  my_ticks_from_time(double aTime)
{
   return int(round(aTime*1000.0));
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* my_timestamp1(char* aBuffer)
{
   timespec tTimeSpec;
   timespec_get(&tTimeSpec, TIME_UTC);

   strftime(aBuffer, 40, "%F %T", localtime(&tTimeSpec.tv_sec));

   return aBuffer;
}

char* my_timestamp2(char* aBuffer)
{
   timespec tTimeSpec;
   timespec_get(&tTimeSpec, TIME_UTC);

   char tTemp[40];
   strftime(tTemp, 40, "%F %T", localtime(&tTimeSpec.tv_sec));
   sprintf(aBuffer, "%s.%03ld", tTemp, tTimeSpec.tv_nsec / 1000000);

   return aBuffer;
}

char* my_timestamp3(char* aBuffer)
{
   timespec tTimeSpec;
   timespec_get(&tTimeSpec, TIME_UTC);

   time_t rawtime;
   struct tm* info;
   char time_stamp[80];
   time(&rawtime);
   info = localtime(&rawtime);
   strftime(time_stamp, sizeof(time_stamp), "%x-%I:%M:%S%p", info);
   sprintf(aBuffer, "%s.%li", time_stamp, tTimeSpec.tv_nsec / 1000000);

   return aBuffer;
}

char* my_timestamp4(char* aBuffer)
{
   timespec tTimeSpec;
   timespec_get(&tTimeSpec, TIME_UTC);

   char tTemp[40];
   strftime(tTemp, 40, "%T", localtime(&tTimeSpec.tv_sec));
   sprintf(aBuffer, "%s.%03ld", tTemp, tTimeSpec.tv_nsec / 1000000);

   return aBuffer;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Convert string to upper case

void my_strupr(char* aString)
{
   unsigned char* tPtr = (unsigned char*)aString;
   int tIndex=0;
   while (tPtr[tIndex]!=0)
   {
      int tValue = (int)tPtr[tIndex];
      if ((tValue >= 97)&&(tValue <= 122))
      {
         tValue -= 32;
         tPtr[tIndex] = (unsigned char)tValue;
      }
      tIndex++;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy string

void my_strncpy(char* aDestin,const char* aSource,unsigned int aSize)
{
   if (aSize <= 1)
   {
      aDestin[0] = 0;
      return;
   }

   int tIndex = 0;
   while (true)
   {
      aDestin[tIndex] = aSource[tIndex];
      if (aSource[tIndex] == 0) break;
      if (tIndex == (int)aSize - 1) break;
      tIndex++;
   }
   aDestin[tIndex] = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// File operations

bool my_copy_file(char* aSourcePath, char* aDestinPath)
{

   //--------------------------------------------------------------
   // Open source file

   FILE* tSourceFile = fopen(aSourcePath,"rb");
   if (tSourceFile == 0) return false;

   // Get source file size
   fseek(tSourceFile,0, SEEK_END);
   int tSourceSize = ftell(tSourceFile);
   rewind(tSourceFile);

   //--------------------------------------------------------------
   // Open destination file

   FILE* tDestinFile = fopen(aDestinPath,"wb");
   if (tDestinFile == 0) return false;

   //--------------------------------------------------------------
   // Copy source file to destination file

   char* tBuffer = (char*)malloc(1024);
   if (!tBuffer) return false;
   int  tReadResult  = 0;
   int  tWriteResult = 0;
   int  tReadCount   = 0;
   int  tWriteCount  = 0;

   // Loop 
   bool tGoing=true;
   while (tGoing)
   {
      // Read into buffer from source file
      tReadResult  = (int)fread(tBuffer,1,1024,tSourceFile);
      tReadCount += tReadResult;

      // Guard
      if (ferror(tSourceFile)) return false;

      // Write buffer to destination file
      tWriteResult = (int)fwrite(tBuffer,1,tReadResult,tDestinFile);
      tWriteCount += tWriteResult;

      // Guard
      if (ferror(tDestinFile)) return false;

      // Done 
      if (tWriteCount >= tSourceSize) tGoing=false;
   }


   //--------------------------------------------------------------
   // Done

   // Close files
   fclose(tSourceFile);
   fclose(tDestinFile);
   // Free
   free((void*)tBuffer);

   return true;
}

bool my_compare_files (char* aFile1Path, char* aFile2Path, int aNumOfBytes)
{
   bool tSameFlag = true;
   bool tGoing    = true;

   //--------------------------------------------------------------
   // Open files

   FILE* tFile1File = fopen(aFile1Path,"rb");
   if (tFile1File == 0) return false;

   FILE* tFile2File = fopen(aFile2Path,"rb");
   if (tFile2File == 0) return false;

   //--------------------------------------------------------------
   // Allocate memory

   char* tBuffer1 = (char*)malloc(aNumOfBytes);
   char* tBuffer2 = (char*)malloc(aNumOfBytes);
   if (!tBuffer1) return false;
   if (!tBuffer2) return false;

   //--------------------------------------------------------------
   // Read from files

   int tReadResult1  = (int)fread(tBuffer1,1,aNumOfBytes,tFile1File);
   int tReadResult2  = (int)fread(tBuffer2,1,aNumOfBytes,tFile2File);

   //--------------------------------------------------------------
   // Compare read sizes

   if (tReadResult1 != tReadResult2)
   {
      tSameFlag = false;
      tGoing = false;
   }

   //--------------------------------------------------------------
   // Compare file content

   if (tGoing)
   {
      for (int i = 0; i < tReadResult1; i++)
      {
         if (tBuffer1[i] != tBuffer2[i]) 
         {
            tSameFlag = false;
            tGoing = false;
            break;
         }
      }
   }

   //--------------------------------------------------------------
   // Done

   // Close files
   fclose(tFile1File);
   fclose(tFile2File);

   // Free
   free((void*)tBuffer1);
   free((void*)tBuffer2);

   return tSameFlag;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* my_stringLLU(char* aString,unsigned long long aValue)
{
   char tString1[24];
   char tString2[24];
   char tString3[24];
   char tString4[24];
   memset(tString2, '.',23);
   memset(tString3, '.',23);
   memset(tString4, '.',23);
   tString2[23]=0;
   tString3[23]=0;
   tString4[23]=0;

   sprintf(tString1, "%llu",aValue);

   int tLen1=(int)strlen(tString1);
   int tLen2=0;
   int tLen3=0;
   int tLen4=0;
   int j=0;
   char tSkip = ',';

   for (int i = 0; i< tLen1; i++)
   {
      j = tLen1-i-1;
      tString2[j] = tString1[i];
   }
   tLen2=tLen1;
   tString2[tLen2]=0;

   j=0;
   for (int i = 0; i< tLen2; i++)
   {
      tString3[j] = tString2[i];
      j++;
      if (i % 3 == 2) tString3[j++]=tSkip;
   }
   if (tString3[j-1]==tSkip)j--;

   tLen3=j;
   if (tLen3 >= 0) tString3[tLen3]=0;

   for (int i = 0; i< tLen3; i++)
   {
      j = tLen3-i-1;
      tString4[j] = tString3[i];
   }
   tLen4=tLen3;
   if (tLen4 >= 0) tString4[tLen4]=0;

   strncpy(aString,tString4,tLen4);
   aString[tLen4]=0;

   return aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

char* my_stringLL(char* aString, long long aValue)
{
   char tString1[24];
   char tString2[24];
   char tString3[24];
   char tString4[24];
   memset(tString2, '.', 23);
   memset(tString3, '.', 23);
   memset(tString4, '.', 23);
   tString2[23] = 0;
   tString3[23] = 0;
   tString4[23] = 0;

   sprintf(tString1, "%lld", aValue);

   int tLen1 = (int)strlen(tString1);
   int tLen2 = 0;
   int tLen3 = 0;
   int tLen4 = 0;
   int j = 0;
   char tSkip = ',';

   for (int i = 0; i < tLen1; i++)
   {
      j = tLen1 - i - 1;
      tString2[j] = tString1[i];
   }
   tLen2 = tLen1;
   tString2[tLen2] = 0;

   j = 0;
   for (int i = 0; i < tLen2; i++)
   {
      tString3[j] = tString2[i];
      j++;
      if (i % 3 == 2) tString3[j++] = tSkip;
   }
   if (tString3[j - 1] == tSkip)j--;

   tLen3 = j;
   if (tLen3 >= 0) tString3[tLen3] = 0;

   for (int i = 0; i < tLen3; i++)
   {
      j = tLen3 - i - 1;
      tString4[j] = tString3[i];
   }
   tLen4 = tLen3;
   if (tLen4 >= 0) tString4[tLen4] = 0;

   strncpy(aString, tString4, tLen4);
   aString[tLen4] = 0;

   return aString;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a ipv4 address in host order converted from a string.

unsigned my_ipv4_addr_from_string(const char* aString)
{
   unsigned tA0 = 0;
   unsigned tA1 = 0;
   unsigned tA2 = 0;
   unsigned tA3 = 0;

   if (sscanf(aString, "%u.%u.%u.%u", &tA3, &tA2, &tA1, &tA0) != 4)
   {
      return 0;
   }

   return (tA3 << 24) | (tA2 << 16) | (tA1 << 8) | (tA0);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return a string converted from an ipv4 address in host order.

char* my_string_from_ipv4_addr(char* aBuffer, unsigned aAddress)
{
   sprintf(aBuffer, "%u.%u.%u.%u",
      (aAddress >> 24),
      (aAddress >> 16 & 0xff),
      (aAddress >> 8 & 0xff),
      (aAddress & 0xff));
   return aBuffer;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
void my_halt(char* aString)
{
   printf("HALT$HALT$HALT$HALT$HALT$$$$ %s\n",aString);
   exit(-1);
}

