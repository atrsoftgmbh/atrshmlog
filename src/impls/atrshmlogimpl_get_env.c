
#include "../atrshmlog_internal.h"

/*******************************************************************/

/**
 * \file atrshmlogimpl_get_env.c
 */

/**
 * \n Main code:
 *
 * \brief Get the environment value for the env var with the suffix.
 *
 * The prefix is set from attach, from the PREFIX Environment Var or
 * with set envprefix.
 */
const char* atrshmlog_get_env(const char* i_suffix)
{
  char buff[ATRSHMLOG_PREFIX_SIZE] ;

  if (strlen(i_suffix) < (ATRSHMLOG_PREFIX_SIZE / 2 - 2) && strlen(i_suffix) > 0)
    {
#if ATRSHMLOG_USE_SAFER_COPY == 1
      strlcpy(buff, atrshmlog_get_env_prefix(), ATRSHMLOG_PREFIX_SIZE);
      strlcat(buff, i_suffix, ATRSHMLOG_PREFIX_SIZE);
#else
      strcat(strcpy(buff, atrshmlog_get_env_prefix()), i_suffix);
#endif
      return getenv(buff);
    }

  return NULL;
}

