#include "stdafx.h"

#include <functional>

#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("GO1"))  executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))  executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))  executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))  executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))  executeGo5(aCmd);
   if (aCmd->isCmd("GO6"))  executeGo6(aCmd);
   if (aCmd->isCmd("GO7"))  executeGo7(aCmd);
   if (aCmd->isCmd("GO8"))  executeGo8(aCmd);
   if (aCmd->isCmd("GO9"))  executeGo9(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

//******************************************************************************
//******************************************************************************
//******************************************************************************

void my_function(int aX)
{
   printf("my_function %d\n", aX);
}

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   void(*tFunction1)(int) = &my_function;
   tFunction1(101);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   typedef void(*FunctionT)(int);
   FunctionT tFunction1 = &my_function;
   tFunction1(101);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   std::function<void(int)> tFunction1 = &my_function;
   tFunction1(101);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   typedef std::function<void(int)> FunctionT;
   FunctionT tFunction1 = &my_function;
   tFunction1(101);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   using namespace std::placeholders;
   typedef std::function<void(int)> FunctionT;
   FunctionT tFunction1 = std::bind(my_function, _1);
   tFunction1(101);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class MyClass
{
public:
   void my_function0()
   {
      printf("my_function\n");
   }
   void my_function1(int aX)
   {
      printf("my_function %d\n", aX);
   }
};


void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
   using namespace std::placeholders;
   typedef std::function<void(int)> FunctionT;

   MyClass tObject;

   FunctionT tFunction1 = std::bind(&MyClass::my_function1, &tObject, _1);
   tFunction1(101);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo7(Ris::CmdLineCmd* aCmd)
{
   using namespace std::placeholders;
   typedef std::function<void(void)> FunctionT;

   MyClass tObject;

   FunctionT tFunction1 = std::bind(&MyClass::my_function0, &tObject);
   tFunction1();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo8(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo9(Ris::CmdLineCmd* aCmd)
{
}

