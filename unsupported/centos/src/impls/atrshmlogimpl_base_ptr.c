
#include "../atrshmlog_internal.h"

/***************************************************************/


/**
 * \file atrshmlogimpl_base_ptr.c
 */

/** 
 * \n Main code:
 *
 * \brief This is the base address of the used shm buffer .
 *
 * This is read to determine if the logging is attached
 * to a buffer. If not it is NULL .
 * So this is a read only for all users .
 *
 * Only reason to make it available it to speed up the macros.
 * Ok, its posible to use inline functions today, but they are
 * problematic if you try to hide such a thing as the pointer - 
 * you come from hiding it to the fact that the  impl of your
 * inline has to be known anyway. So its not a real better solution ..
 *
 * Simply use it as its declared : volatile , never cache it
 * never change it - use it like its const ... 
 *
 * Use it as it where readonly.
 */
volatile const void* atrshmlog_base_ptr = 0;

