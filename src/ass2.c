
#include "atrshmlog_internal.h"


/********************************************************************/

/** 
 * \n Main code:
 *
 * The simple version, no check for fences 
 * compile with -S to see how its differ from the 
 * basic dummy version
 */
uint64_t atrshmlog_get_tsc_x86_64_gnu(void)
{
  uint32_t hi, lo;

  __asm volatile
    ("rdtsc" : "=a" (lo), "=d" (hi));


  return ((uint64_t)hi << 32) | lo;
}
