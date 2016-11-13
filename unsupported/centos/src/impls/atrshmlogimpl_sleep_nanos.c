
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_sleep_nanos.c
 */

/**
 * \n Main code:
 *
 * \brief We wait nanoseconds.
 * 
 * This is done by  loop because at least on my box
 * I found that longer intervals did not work as expected.
 * So we use a loop for wait of 100000 nanos and then the rest..
 */
void atrshmlog_sleep_nanos(atrshmlog_int32_t i_nanos)
{

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC  == 1
  const int timeframe = 100000;

  if (i_nanos < 0)
    i_nanos = 1;

  int limit = i_nanos / timeframe;
  int rest = i_nanos % timeframe;
  
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = timeframe;
  
  for (int i = 0; i < limit; i++) 
    nanosleep(&ts, NULL);

  if (rest > 0)
    {
      ts.tv_nsec = rest;

      nanosleep(&ts, NULL);
    }
#endif

#if   ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1

  // careful. we try this at least, but there is on this OS
  // no easy way to get it done. so we hope the best
  // if you get a very diffrent picture ask billy boy
  // and his gang to make it 
  const int timeframe = 100000;

  if (i_nanos < 0)
    i_nanos = 1;

  int limit = i_nanos / timeframe;
  int rest = i_nanos % timeframe;
  
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = timeframe;
  
  for (int i = 0; i < limit; i++) 
    nanosleep(&ts, NULL);

  if (rest > 0)
    {
      ts.tv_nsec = rest;

      nanosleep(&ts, NULL);
    }

#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1

# if ATRSHMLOG_USE_NANOSLEEP == 1
  // there is always hope that the guys
  // made it for the pthread lib right.
  // so you can try this with -pthread
  // if it does not work try the millis approch
  const int timeframe = 100000;

  if (i_nanos < 0)
    i_nanos = 1;

  int limit = i_nanos / timeframe;
  int rest = i_nanos % timeframe;
  
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = timeframe;
  
  for (int i = 0; i < limit; i++) 
    nanosleep(&ts, NULL);

  if (rest > 0)
    {
      ts.tv_nsec = rest;

      nanosleep(&ts, NULL);
    }


# else  
  // sorry. but the OS does simply not support that.
  int millis = i_nanos / 1000000;

  Sleep(millis);
  
# endif

#endif
}  
