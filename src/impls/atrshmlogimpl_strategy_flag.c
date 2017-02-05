
#include "../atrshmlog_internal.h"

/********************************************************************/

/**
 * \file atrshmlogimpl_strategy_flag.c
 */

/**
 * \n Main code:
 *
 * \brief The strategy to handle buffer switching for the process.
 *
 * There are several strategies defined as enum atrshmlog_strategy
 *
 * We use them when we switch buffers and hit the worst case,
 * all full.
 *
 * So we can discard, spin loop, wait loop, and choose for one
 * of the adaptive strategies here.
 *
 * This is the process value, it is a prototype for the threads.
 * So you have to set it diffrent in the thread if you have
 * a thread doing something different.
 *
 * For starters we wait loop.
 */
_Alignas(128) int atrshmlog_buffer_strategy = atrshmlog_strategy_wait;

