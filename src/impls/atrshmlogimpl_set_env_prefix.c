
#include "../atrshmlog_internal.h"


/******************************************************************/

/**
 * \file atrshmlogimpl_set_env_prefix.c
 */

/** 
 * \n Main code:
 *
 * \brief Set the prefix for all env variable lookups we do in our
 * attach and initialize part.
 *
 * This has to be done before we use any env stuff. 
 * 
 * so you have to do this before you attach.
 *
 * test t_set_env_prefix.c
 */
void atrshmlog_set_env_prefix (const char*i_prefix)
{
  ATRSHMLOGSTAT(atrshmlog_counter_set_env_prefix);

  /* set it if all is ok and if not already set */
  if (i_prefix && *i_prefix != '\0' && *atrshmlog_prefix_name_buffer == '\0')
    {
      const int limit = ( ATRSHMLOG_PREFIX_SIZE / 2 );
      
      int len = strlen(i_prefix);

      if (len > limit )
	len = limit;
      
      memcpy(atrshmlog_prefix_name_buffer, i_prefix, len);
      atrshmlog_prefix_name_buffer[len] = 0;
    }
}

