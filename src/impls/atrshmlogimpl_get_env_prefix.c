
#include "../atrshmlog_internal.h"

#include <string.h>

#include <stdlib.h>

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_env_prefix.c
 */

/**
 * \n Main code:
 *
 * \brief Get the name of the environment variable.
 * 
 * We can have - in future - multiple buffers to log to.
 *
 * So to make life simple we have a default buffer value,
 * this is ATRSHMLOG_ID in the environment.
 * 
 * If you need more buffers and in concurrency you can do that 
 * by overwriting ATRSHMLOG_ID in it...
 * 
 * But sometimes its not that simple, 
 * then you can use the prefix part via ATRSHMLOG.
 *
 * This means you dont have to know the value, its already
 * set somewhere else and you only switch the prefix to
 * something diffrent. 
 *
 * You can use 128 chars as the new prefix
 * that replace ATRSHMLOG in the lookups....
 * 
 * Every program starts its logging with either 
 * a getenv or a attach (which itself starts with a getenv.
 *
 * So we use inside the getenv the following function
 * that constructs the name of the prefix dynamically
 * for the first time it is run. 
 *
 * And after this it is never
 * changing the given value any more...
 *
 * ATRSHMLOG=hugo 
 *
 * is a perfectly working option....
 *
 * This changes the prefix for all of the getenv and file lookups 
 * yes, the files too ... 
 *
 * So you can set it somehow and then use it even in the case the thing is
 * a logging shell (to be onest, i could not achive this, i leave it as
 * an option for someone better in configuring the system ) 
 *
 * test t_get_env_prefix.c
 */
const char* atrshmlog_get_env_prefix(void)
{
  static char buff[ATRSHMLOG_PREFIX_SIZE];

  static int once = 0;

  if (once == 0)
    {
      ++once;

      if (once == 1)
        {
	  /* already set via program ? */
#if ATRSHMLOG_USE_SAFER_COPY == 1
	  if (*atrshmlog_prefix_name_buffer == '\0')
	    strlcpy(atrshmlog_prefix_name_buffer, ATRSHMLOG_ENV_PREFIX,ATRSHMLOG_PREFIX_SIZE);

#else
	  if (*atrshmlog_prefix_name_buffer == '\0')
	    strcpy(atrshmlog_prefix_name_buffer, ATRSHMLOG_ENV_PREFIX);

#endif
	  
	  if (getenv(ATRSHMLOG_ENV_PREFIX))
	    {
	      int len = strlen(getenv(ATRSHMLOG_ENV_PREFIX));

	      if(len > 0)
		{
		  memcpy(atrshmlog_prefix_name_buffer, getenv(ATRSHMLOG_ENV_PREFIX), (len < ( ATRSHMLOG_PREFIX_SIZE / 2)) ? len : ( ATRSHMLOG_PREFIX_SIZE / 2));
		  atrshmlog_prefix_name_buffer[(len < ( ATRSHMLOG_PREFIX_SIZE / 2)) ? len : ( ATRSHMLOG_PREFIX_SIZE / 2) ] = '\0';
		}
	    }

#if ATRSHMLOG_USE_SAFER_COPY == 1 
	  strlcpy(buff, atrshmlog_prefix_name_buffer, ATRSHMLOG_PREFIX_SIZE);

#else
	  strcpy(buff, atrshmlog_prefix_name_buffer);
#endif
	}
    }
  
  return buff;
}

