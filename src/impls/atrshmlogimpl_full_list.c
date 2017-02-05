
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_full_list.c
 */

/** 
 * \n Main code:
 *
 * \brief This is the anchor for the full list.
 *
 * We use an atomic here, so its safe to do the mt thing.
 * 
 * Its the list of full buffers that must be transfered to shm with the slave
 * threads. 
 * 
 * The slaves check this and then grab a buffer and transfer it
 * to the shm buffers.
 *
 * So we are empty at begining.
 */
_Alignas(128) atomic_intptr_t atrshmlog_tpf = ATOMIC_VAR_INIT(0);


/**
 * \n Main code:
 *
 * \brief We get the buffer full anchor
 *
 * \return
 * Pointer to buffer
 *
 * test  t_reflexion
 */
volatile const void *atrshmlog_get_buffer_full_anchor (void)
{
  volatile const void *result;

  result = (volatile const void *)atomic_load(&atrshmlog_tpf);

  return result;
}

