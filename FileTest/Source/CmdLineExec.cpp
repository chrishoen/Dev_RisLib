#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_functions.h"
#include "prnPrint.h"
#include "risTextFile.h"

#include "CmdLineExec.h"

using namespace std;
//******************************************************************************
CmdLineExec::CmdLineExec()
{
}
//******************************************************************************
void CmdLineExec::reset()
{
}
//******************************************************************************
void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);
}


//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Ris::LogFileWriter tWriter;
   tWriter.open("C:\\MyLib\\Data\\log_file_101.txt");
   tWriter.write("%d %d",101,102);
   tWriter.write("%d %d",201,202);
   tWriter.close();
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Ris::CsvFileWriter tWriter;
   tWriter.open("C:\\MyLib\\Data\\file_101.csv");
   tWriter.writeN(2,101.1,102.2);
   tWriter.writeN(2,201.1,202.2);
   tWriter.write (301.1,302.2);
   tWriter.write (401.1,302.2);
   tWriter.close();
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

class MyClass1
{
public:

   MyClass1()
   {
      mX1[0] = 101;
      mX2[0] = 102;
   }

   enum {MaxSize = 100};
   int  mX1[MaxSize];

   static const int cMaxSize = 100;
   int  mX2[cMaxSize];

   void myCall(int aX)
   {
      const int* tPtr1 = &cMaxSize;
      int* tPtr2 = (int*)&cMaxSize;
   }

};

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

