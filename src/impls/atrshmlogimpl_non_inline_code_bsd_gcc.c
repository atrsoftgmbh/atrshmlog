
#include "../atrshmlog_internal.h"

/************************************************************************/

/**
 * \file atrshmlogimpl_non_inline_code_bsd_clang.c
 */

// non inline code

/*
 * get time fast. we use the tsc approch her.
 * later on we will have to adapt this for
 * POWER
 * SPARC
 * MIPS
 * ITANIUM
 * ARM
 * and any platform that is in need for the atrshmlog
 */

#if ATRSHMLOG_INLINE_TSC_CODE == 1

/* this is done in the header */

#else

# if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1 


/********************************************************************/

/** 
 * \n Main code:
 *
 * The parallel version in modern cpu 
 */
uint64_t atrshmlog_get_tsc_par_amd64_gnu(void)
{
  uint32_t hi, lo;

  __asm volatile
    ("rdtscp" : "=a" (lo), "=d" (hi));

  return ((uint64_t)hi << 32) | lo;
}


/********************************************************************/

/** 
 * \n Main code:
 *
 * The not so modern combination of fence and call 
 */
uint64_t atrshmlog_get_tsc_fence_amd64_gnu(void)
{
  uint32_t hi, lo;

  __asm volatile
    ("lfence");
  
  __asm volatile
    ("rdtsc" : "=a" (lo), "=d" (hi));

  return ((uint64_t)hi << 32) | lo;
}


/********************************************************************/

/** 
 * \n Main code:
 *
 * The simple version, no check for fences 
 */
uint64_t atrshmlog_get_tsc_amd64_gnu(void)
{
  uint32_t hi, lo;

  __asm volatile
    ("rdtsc" : "=a" (lo), "=d" (hi));

  return ((uint64_t)hi << 32) | lo;
}


/********************************************************************/

/** 
 * \n Main code:
 *
 * The well know dummy 
 */
uint64_t atrshmlog_get_tsc_null_amd64_gnu(void)
{
  return 0;
}
# endif


#endif

/*******************************************************************/

/**
 * \n Main code:
 *
 * This is os dependent. 
 * We use a static here that can be set at attach or from the 
 * program. So be sure you know what time
 * you want
 */
#if ATRSHMLOG_INLINE_GETTIME == 1

/* this is done in the header */

#else

# if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1

/** 
 * \n Main code:
 *
 * We get the internal tsc time according to clock id
 */
atrshmlog_time_t atrshmlog_get_clicktime(void)
{
  if (atrshmlog_clock_id == 3)
    {
      return atrshmlog_get_tsc_par_amd64_gnu();
    }

  if (atrshmlog_clock_id == 2)
    {
      return atrshmlog_get_tsc_fence_amd64_gnu();
    }

  if (atrshmlog_clock_id == 1)
    {
      return atrshmlog_get_tsc_amd64_gnu();
    }

  return atrshmlog_get_tsc_null_amd64_gnu();
}
# endif


#endif
