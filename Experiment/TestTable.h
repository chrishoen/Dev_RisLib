#pragma once
/*==============================================================================

Table class templates that can be used by the command line file classes.
One or two dimensional table classes of int,double,string.

==============================================================================*/

#include "stdlib.h"
#include "stdio.h"
#include <array>
#include <type_traits>

#include "my_functions.h"
#include "risCmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// One dimesnional table.

template<typename ElementType,size_t MaxRows>
class TestTable : public std::array<ElementType, MaxRows>
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Last row number that was read from the file.
   int  mRows;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   TestTable()
   {
      reset();
   }

   // Reset variables.
   void reset()
   {
      foo2(101);
      foo3(101);

   }



   // foo2 overload is enabled via a parameter
   template<class T>
   void foo2(T t, typename std::enable_if<std::is_same<T,int>::value >::type* = 0)
   {
      printf("foo2 int\n");
   }

   // foo3 overload is enabled via a template parameter
   template<class T,
      typename std::enable_if<std::is_same<T,int>::value>::type* = nullptr >
   void foo3(T t) // note, function signature is unmodified
   {
      printf("foo2 int\n");
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************




