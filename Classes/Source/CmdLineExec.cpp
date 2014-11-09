#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "prnPrint.h"
#include "risContainers.h"

#include "someClass1.h"
#include "someClass2.h"
#include "someClass3.h"

#include "CmdLineExec.h"


using namespace Some;

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
   if(aCmd->isCmd("GO6"    ))  executeGo6(aCmd);
   if(aCmd->isCmd("GO7"    ))  executeGo7(aCmd);
}

//******************************************************************************

void CmdLineExec::myCall(int aX)
{
   Prn::print(0,0,"CmdLineExec::myCall %d",aX);
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,101);

   gClass1.mMyCallPointer(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,201);

   gClass1.mMyCallPointer = Class1::MyCallPointer(this,&CmdLineExec::myCall);

   gClass1.mMyCallPointer(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,301);

   gClass1.configure(Class1::MyCallPointer(this,&CmdLineExec::myCall));

   gClass1.mMyCallPointer(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,401);

   gClass2.mMyCallback(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,501);

   gClass3.mMyInterface3->interfaceCall3(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,601);

   BaseInterface3* tFace = new Interface3<Class3>(&gClass3,&Class3::myCall);
   tFace->interfaceCall3(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeGo7(Ris::CmdLineCmd* aCmd)
{
   typedef void (CmdLineExec::*VoidMethod)();
   typedef void (*VoidFunction)();

   Prn::print(0,0,"int          %d",sizeof(int));
   Prn::print(0,0,"double       %d",sizeof(double));
   Prn::print(0,0,"void*        %d",sizeof(void*));
   Prn::print(0,0,"VoidMethod   %d",sizeof(VoidMethod));
   Prn::print(0,0,"VoidFunction %d",sizeof(VoidFunction));


}

