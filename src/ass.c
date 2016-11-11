
#include "atrshmlog_internal.h"


/********************************************************************/

/** 
 * \n Main code:
 *
 * The simple version, no check for fences 
 * and its a dummy after all... compile with -S 
 * to get a basic function in assembler
 */
uint64_t atrshmlog_get_tsc_x86_64_gnu(void)
{
  uint32_t hi, lo;

  hi =4711;
  lo = 4812;

  return ((uint64_t)hi << 32) | lo;
}
