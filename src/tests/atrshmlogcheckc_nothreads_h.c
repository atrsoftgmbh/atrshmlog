#include <stdio.h>


#ifdef __STDC_NO_ATOMICS__
#define autch 1
#else
#define autch 0

#include <stdatomic.h>

#endif

#ifdef __STDC_NO_THREADS__
#define threadsautch 1
#else
#define threadsautch 0

//#include <threads.h>
_Thread_local static int a = 0;

#endif

int main ()
{
  printf("hello atomic world\n%d\n%d\n", autch, threadsautch);

  return 0;
}

