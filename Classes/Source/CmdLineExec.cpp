#include "stdafx.h"

#include "risContainers.h"

#include "someClass1.h"
#include "someClass3.h"
#include "someClass4.h"
#include "someClass5.h"

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
   if(aCmd->isCmd("GO1"    ))  executeGo1  (aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2  (aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3  (aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4  (aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5  (aCmd);
   if(aCmd->isCmd("GO6"    ))  executeGo6  (aCmd);
   if(aCmd->isCmd("GO7"    ))  executeGo7  (aCmd);
   if(aCmd->isCmd("GO11"   ))  executeGo11 (aCmd);
   if(aCmd->isCmd("GO12"   ))  executeGo12 (aCmd);
   if(aCmd->isCmd("GO13"   ))  executeGo13 (aCmd);
}

//******************************************************************************

void CmdLineExec::myCall(int aX)
{
   Prn::print(0,"CmdLineExec::myCall %d",aX);
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,101);

   Class1<Traits1> tClass1;
   tClass1.test1();
   tClass1.test2();
   tClass1.parent1();
   tClass1.parent2();
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Class5<Traits1> tClass;
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

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0,"");

   Prn::print(0,"new Class3B");
   Class3B* t3B = new Class3B;
   Prn::print(0,"");

   Prn::print(0,"new Class3A");
   Class3A* t3A = new Class3A;
   Prn::print(0,"");

   Prn::print(0,"delete Class3B");
   delete t3B;
   Prn::print(0,"");

   Prn::print(0,"delete Class3A");
   delete t3A;
   Prn::print(0,"");
}

//******************************************************************************

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo7(Ris::CmdLineCmd* aCmd)
{
   typedef void (CmdLineExec::*VoidMethod)();
   typedef void (*VoidFunction)();

   Prn::print(0,"int          %d",sizeof(int));
   Prn::print(0,"double       %d",sizeof(double));
   Prn::print(0,"void*        %d",sizeof(void*));
   Prn::print(0,"VoidMethod   %d",sizeof(VoidMethod));
   Prn::print(0,"VoidFunction %d",sizeof(VoidFunction));


}

//******************************************************************************
Class4A const cClass4A;
Class4A& rClass4A = gClass4A;
Class4A const& crClass4A = gClass4A;

inline Class4A const& getTestClass4A()
{
   return cClass4A;
}

inline Class4A const& getTestClass4A2()
{
   Class4A const& constrefClass4A = gClass4A;
   return constrefClass4A;
}

void CmdLineExec::executeGo11(Ris::CmdLineCmd* aCmd)
{
   cClass4A.constMethod();
   rClass4A.mutableMethod();
   crClass4A.constMethod();
   getTestClass4A().constMethod();
   getTestClass4A2().constMethod();
   getClass4A().constMethod();
}

//******************************************************************************

void CmdLineExec::executeGo12(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo13(Ris::CmdLineCmd* aCmd)
{
}

