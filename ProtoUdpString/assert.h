#pragma once


inline void assert(bool b)
{
  if (!b)
  {
    printf("ASSERT\n");
    exit(-99);
  }
}
