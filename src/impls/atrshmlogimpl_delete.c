
#include "../atrshmlog_internal.h"


#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1 \
  || ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1 \
  || ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1 \
  || ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1 \
  || ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1
/** The shm stuff */
# include <sys/shm.h>

#endif


/*******************************************************************/

/**
 * \file atrshmlogimpl_delete.c
 */

/** 
 * \n Main code:
 *
 * \brief We destroy the shm 
 *
 * test t_delete.c
 */
atrshmlog_ret_t atrshmlog_delete(const int i_shmid)
{
  ATRSHMLOGSTAT(atrshmlog_counter_delete);

  while (atomic_load(&atrshmlog_base_ptr_use_flag) > 0)
    ATRSHMLOG_SLEEP_NANOS(atrshmlog_slave_to_shm_wait);

  atrshmlog_base_ptr = 0;
  
#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
  
  int shmctl_result = shmctl(i_shmid, IPC_RMID, (struct shmid_ds *)0);

#endif
  

#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1
  
  int shmctl_result = shmctl(i_shmid, IPC_RMID, (struct shmid_ds *)0);

#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
  // there is no disconnect. its all ref counted. so this is a dummy
  
  int shmctl_result = 0;
#endif
  
#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1
  
  int shmctl_result = shmctl(i_shmid, IPC_RMID, (struct shmid_ds *)0);

#endif
  
#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1
  
  int shmctl_result = shmctl(i_shmid, IPC_RMID, (struct shmid_ds *)0);

#endif
  
#if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1
  
  int shmctl_result = shmctl(i_shmid, IPC_RMID, (struct shmid_ds *)0);

#endif
  
  return shmctl_result;
}

