
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_get_realtime.c
 */

/**
 * \n Main code:
 *
 * \brief  We get the real time 
 */
atrshmlog_internal_time_t atrshmlog_get_realtime(void)
{
  atrshmlog_internal_time_t result;
  
#if ATRSHMLOG_USE_CLOCK_GETTIME == 1
  struct timespec hugotime ;
      
  clock_gettime(CLOCK_REALTIME, &hugotime);

  result.tv_sec = hugotime.tv_sec;
  result.tv_nsec = hugotime.tv_nsec;
#endif

#if ATRSHMLOG_USE_WIN_FILETIME == 1

  FILETIME t;

  GetSystemTimeAsFileTime(&t);

  DWORD low;
  DWORD high;
  low = t.dwLowDateTime;
  high = t.dwHighDateTime;
  result.tv_sec = high;
  result.tv_sec *= 100;
  
  long long l = low;
  l *= 100;
  result.tv_nsec = l % 1000000000;
  result.tv_sec += l / 1000000000;
#endif

#if ATRSHMLOG_USE_GETTIMEOFDAY == 1
  struct timeval t;
  // no, we dont use it struct timezone tz;

  gettimeofday(&t, NULL);
  result.tv_sec = t.tv_sec;
  result.tv_nsec = t.tv_usec * 1000;
  
#endif
  
  return result;
}

