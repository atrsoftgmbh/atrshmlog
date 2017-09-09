
#include "../atrshmlog_internal.h"


#include <stdlib.h>

/*******************************************************************/

/**
 * \file atrshmlogimpl_init_via_env.c
 */

/**
 * \brief We help the init to set a value from an environment variable.
 * 
 * \param i_suffix
 * The Suffix of the variable name after the env prefix.
 *
 * \param v
 * Points to an int that gets the value.
 *
 * \param i_min
 * Minimum allowed number to set.
 *
 * \param i_max
 * Maximum allowed number to set.
 *
 * \return 
 * void
 *
 * test t_attach.c
 */
void atrshmlog_init_via_env(const char *i_suffix,
				   int *v,
				   int i_min,
				   int i_max)
{
  const char *p = atrshmlog_get_env(i_suffix);

  if (p)
    {
#if ATRSHMLOGDEBUG == 1
      printf("ATRSHMLOG%s %s\n", i_suffix, p);
#endif
      long val = strtol(p, NULL, 10);

      if (val >= i_min && val <= i_max)
	*v = val;
      
    }		      
}

