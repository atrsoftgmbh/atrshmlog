
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_delete.c
 */

/** 
 * \n Main code:
 *
 * \brief We destroy the shm 
 */
atrshmlog_ret_t atrshmlog_delete(const int i_shmid)
{
  ATRSHMLOGSTAT(atrshmlog_counter_delete);

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

