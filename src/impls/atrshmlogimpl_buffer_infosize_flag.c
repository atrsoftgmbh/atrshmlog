
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_buffer_infosize_flag.c
 */

/**
 * \n Main code:
 *
 * \brief The size of buffers in char for the malloc. 
 * 
 * If you want to reduce
 * memory use its better to reduce the buffer size to half or quater ...
 *
 * By change of the define ATRSHMLOGBUFFER_INFOSIZE....
 *
 * But you can also use a dynamic approch, its only a problem to
 * get bigger after a reduce, so be warned that you can reduce 
 * memory usage this way by setting it to a smaller value.
 *
 * As always with mt systems : too much too big is a performance
 * killer, so to make it suiteable in size is an option at least
 * worth a try. 
 * 
 * But the major memory should come from the
 * preallocated static, so check this first ....
 * 
 * There is an env in attach to set it external.
 * It works only for the dyn allocated memory.
 */
_Alignas(128) int atrshmlog_buffer_infosize = ATRSHMLOGBUFFER_INFOSIZE;

