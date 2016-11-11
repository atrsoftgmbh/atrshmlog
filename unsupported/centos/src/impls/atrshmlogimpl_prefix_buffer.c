
#include "../atrshmlog_internal.h"



/********************************************************************/

/**
 * \file atrshmlogimpl_prefix_buffer.c
 */

/**
 * \n Main code:
 *
 * \brief The prefix for all used environment variables.
 *
 * We hold here the prefix for all of our environment variables
 * and files so we can simply switch from one to another by 
 * changing the major prefix name.
 *
 * This is done by setting the ATRSHMLOG variable before 
 * attaching / getting the env value ...
 *
 * Dont change this if you dont need to (for a corporate env name
 * you can use the setter in advance of the attach ).
 *
 * Size is some chars big, so its ok for now.
 *
 * Use it as it is a read only after the attach.
 *
 * If you dont do it it is set in the first call in attach.
 */
char atrshmlog_prefix_name_buffer[ATRSHMLOG_PREFIX_SIZE];

