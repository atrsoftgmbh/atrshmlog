
#include "../atrshmlog_internal.h"

/********************************************************************/

/**
 * \file atrshmlogimpl_slave_wait_st_wait_flag.c
 */

/**
 * \n Main code:
 *
 * \brief We wait this long in a strategy wait  situation when our buffers are full
 */
_Alignas(128) int atrshmlog_strategy_wait_wait_time = ATRSHMLOG_STRATEGY_WAIT_WAIT_TIME_DEFAULT;
