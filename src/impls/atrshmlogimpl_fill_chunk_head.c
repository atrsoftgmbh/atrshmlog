
#include "../atrshmlog_internal.h"

#include <string.h>

/***************************************************************/

/**
 * \file atrshmlogimpl_fill_chunk_head.c
 */

/** 
 * \n Main code:
 *
 * \brief We get the tid of a thread local
 *
 * test t_get_tid.c
 */

void atrshmlog_fill_chunk_head (volatile const void *i_chunk, atrshmlog_chunk_head_t* c)
{
  char *h = (char*)i_chunk;
  
  memcpy(&c->starttime, h + 0, sizeof(atrshmlog_time_t));
  memcpy(&c->endtime, h + sizeof(atrshmlog_time_t) , sizeof(atrshmlog_time_t));
  

  memcpy(&c->totallen, h + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) , sizeof(atrshmlog_int32_t));
      
  memcpy(&c->eventnumber, h + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t), sizeof(atrshmlog_int32_t));
    
  memcpy(&c->userflag, h + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_time_t) + sizeof(atrshmlog_int32_t) + sizeof(atrshmlog_int32_t), sizeof(atrshmlog_int32_t));

  c->eventflag = *((char*)h + ATRSHMLOGCONTROLDATASIZE - 1);

}

