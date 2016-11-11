
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_create.c
 */

/** 
 * \n Main code:
 *
 * \brief We create the shm .
 *
 * On error we deliver a -x, else a valid positive shmid.
 *
 * This uses the key from external source, normally a parameter
 * to the program for a number.
 *
 * Key is a 32 bit value.
 */
int atrshmlog_create(const atrshmlog_key_t i_key,
		     const int i_count)
{
  ATRSHMLOGSTAT(atrshmlog_counter_create);
  
#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
  
  if (i_key == 0 || i_key  == IPC_PRIVATE) {
    ATRSHMLOGSTAT(  atrshmlog_counter_create_abort1);
    
    return atrshmlog_error_create_1;
  }

#endif
  
#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1
  
  if (i_key == 0 || i_key  == IPC_PRIVATE) {
    ATRSHMLOGSTAT(  atrshmlog_counter_create_abort1);
    
    return atrshmlog_error_create_1;
  }

#endif
  
#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
  
  if (i_key < 1 || i_key > 32) {
    ATRSHMLOGSTAT(  atrshmlog_counter_create_abort1);
    
    return atrshmlog_error_create_1;
  }

#endif
  
  /* At least ATRSHMLOGBUFFER_MINCOUNT .. */
  if (i_count < ATRSHMLOGBUFFER_MINCOUNT) {
    ATRSHMLOGSTAT(  atrshmlog_counter_create_abort2);
    
    return atrshmlog_error_create_2;
  }
  
  /* Max ATRSHMLOGBUFFER_MAXCOUNT buffers */
  if (i_count > ATRSHMLOGBUFFER_MAXCOUNT) {
    ATRSHMLOGSTAT(  atrshmlog_counter_create_abort3);
    
    return atrshmlog_error_create_3;
  }
  
  /* We calc the buffer size.
   * Its a fixed part for our infos and a variabel count of
   * buffers.
   * Some systems dont have much memory for a shm buffer, 
   * some have...
   * So simply try to check for the size that is possible with the
   * double number check ...
   */

  const int save = 256 ;
  
  size_t wantedsize = sizeof (atrshmlog_area_t) + (i_count - ATRSHMLOGBUFFER_MINCOUNT) * sizeof( atrshmlog_buffer_t) + (i_count * ATRSHMLOGBUFFER_INFOSIZE + save);

#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
  

  int shmflg = IPC_CREAT | IPC_EXCL;

  shmflg |= ATRSHMLOG_ACCESS; 

  
  int result_shmget = shmget(i_key, wantedsize, shmflg);

#endif

#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1

  int shmflg = IPC_CREAT | IPC_EXCL;

  shmflg |= ATRSHMLOG_ACCESS; 

  int result_shmget = shmget(i_key, wantedsize, shmflg);

#endif


#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
  

  int already;

  int result_shmget = atrshmlog_create_mapped_file(i_key, wantedsize, &already);

#endif
  
  if (result_shmget == -1)
    {
      ATRSHMLOGSTAT(  atrshmlog_counter_create_abort4);
    
      return atrshmlog_error_create_4;
    }

  return result_shmget;
}

