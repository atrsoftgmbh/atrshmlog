
#include "../atrshmlog_internal.h"


/**
 * \file atrshmlogimpl_buffer_id_flag.c
 */

/**
 * \n Main code:
 *
 * \brief The actual last given id for a buffer.
 *
 * This starts with the preallocated and is raising with every dyn
 * allocated buffer in the connect function.
 * So in theory they are diffrent but after the connect should be same.
*/
volatile int atrshmlog_buffer_id = ATRSHMLOGBUFFER_PREALLOCED_COUNT;

