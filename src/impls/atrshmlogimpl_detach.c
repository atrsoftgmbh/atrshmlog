
#include "../atrshmlog_internal.h"


#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1 \
  || ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1 \
  || ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1 \
  || ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1 \
  || ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1
/** The shm stuff */
# include <sys/shm.h>

#endif


/**
 * \file atrshmlogimpl_detach.c
 */


/**
 * \n Main code:
 *
 * \brief The detach from shm function
 *
 * We first switch the base ptr to 0 
 *
 * then we detach physical the shm
 *
 * logging makes no sense then, but we don't
 * change the flags. 
 *
 * We clean also the attach once so we are able
 * to attach again after that.
 *
 * test t_detach.c
 */
atrshmlog_ret_t atrshmlog_detach(void)
{
  ATRSHMLOGSTAT(atrshmlog_counter_detach);

  atrshmlog_ret_t result = 0;
  
  if (atrshmlog_attach_once != 0)
    {
      void* p = (void*)atrshmlog_base_ptr;
      
      while (atomic_load(&atrshmlog_base_ptr_use_flag) > 0)
	 ATRSHMLOG_SLEEP_NANOS(atrshmlog_slave_to_shm_wait);

      atrshmlog_base_ptr = 0;

      if (p != NULL)
	{
#if ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC == 1
  
	  int shmctl_result = shmdt(p);

#endif
  

#if ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC == 1
  
	  int shmctl_result = shmdt(p);

#endif

#if ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC == 1
	  // there is no disconnect. its all ref counted. so this is a dummy
  
	  int shmctl_result = atrshmlog_detach_mapped_file(p); 
#endif
  
#if ATRSHMLOG_PLATFORM_BSD_AMD64_CLANG == 1
  
	  int shmctl_result = shmdt(p);

#endif
  
#if ATRSHMLOG_PLATFORM_BSD_AMD64_GCC == 1
  
	  int shmctl_result = shmdt(p);

#endif
  
#if ATRSHMLOG_PLATFORM_SOLARIS_X86_64_GCC == 1
  
	  int shmctl_result = shmdt(p);

#endif
	  result  = shmctl_result;
	}

      // we can now again attach. 
      atrshmlog_attach_once = 0;

      // we switch of the loging till we reattach 
      atrshmlog_logging_process = 0;
    }
  
  return result;
}

