
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_buffer_size.c
 */

/**
 * \n Main code:
 *
 * \brief This is the actual limit, can be smaller than max.
 *
 * If this is set high
 * its good to have memory for loging,
 * but if the logging fills buffers the memory of the
 * buffers become a problem to cache write.
 *
 * So in case of many threads we reduce the size to get
 * the memory still into the caches...
 *
 * At least we can try this for small things.
 */
atrshmlog_ret_t atrshmlog_get_buffer_size(void)
{
  return atrshmlog_buffer_infosize;
}

