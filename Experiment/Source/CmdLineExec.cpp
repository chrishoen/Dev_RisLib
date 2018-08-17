#include "stdafx.h"

#include "CmdLineExec.h"

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
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
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
//******************************************************************************
//******************************************************************************

class TestRecord
{
public:

   int mCode1;
   int mCode2;
   int mCode3;
   int mCode4;

   TestRecord()
   {
      mCode1=0;
      mCode2=0;
      mCode3=0;
      mCode4=0;
   }
};

//******************************************************************************

class BaseItem
{
public:

   int mRecordType;
   void* mRecord;

   BaseItem()
   {
      mRecordType = 0;
      mRecord = 0;
   }
};

//******************************************************************************

class TestItem : public BaseItem
{
public:

   TestItem()
   {
      mRecordType = 1001;
      mRecord = 0;
   }

   TestItem(TestRecord* aRecord)
   {
      mRecordType = 1001;
      mRecord = (void*)aRecord;
   }

   void set(TestRecord* aRecord)
   {
      mRecord = (void*)aRecord;
   }

};


//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
   TestRecord* tTestRecord1 = new TestRecord;
   BaseItem*   tItem = new TestItem(tTestRecord1);

   int tN1 = tItem->mRecordType;

   TestItem tTestItem(tTestRecord1);
   BaseItem tBaseItem = tTestItem;


}

