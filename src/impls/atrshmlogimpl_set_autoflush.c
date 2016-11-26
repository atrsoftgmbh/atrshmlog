
#include "../atrshmlog_internal.h"

/**********************************************************/

/**
 * \file atrshmlogimpl_set_autoflush.c
 */

/**
 * \n Main code:
 *
 * \brief We set a new autoflush for this thread.
 *
 * \param i_flag
 *
 * \return
 * - negativ error
 * - old value
 *
 * test t_set_autoflush.c
 */
atrshmlog_ret_t atrshmlog_set_autoflush(int i_flag)
{
  atrshmlog_g_tl_t* g  = (atrshmlog_g_tl_t* )atrshmlog_get_thread_locals_adress();

#if ATRSHMLOG_THREAD_LOCAL == 0

  // this can happen if pthread specific is in
  if (g == NULL)
    return atrshmlog_error_set_autoflush_1;

#endif
  
  int old = g->autoflush;


  g->autoflush = i_flag ? 1 : 0;

  return old;
}

