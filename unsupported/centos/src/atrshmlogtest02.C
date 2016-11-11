//
// atrshmlog test program for c++ code
//
// we use the C header, use the thing and bind via lib
//

/** \file atrshmlogtest02.C
 * \brief We are a C++ test program.
 */

#include "atrshmlog.h"


#include <iostream>

#include <thread>

#include <unistd.h>

/**
 * we connect and then write some stuff to the log.
 * Simple test program.
 */
int main (int argc, char* argv[])
{
  const char *shmid_ptr = ATRSHMLOG_GET_ENV_SHMID();

  
  if (shmid_ptr == (void*)0)
    {
      std::cout << "usage: " << argv[0] << " [ ... ]\n"
		<< "ENVIRONMENT VARIABLE NOT SET !!!\n"
		<< ATRSHMLOG_GET_ENV_PREFIX() << "_ID"<< std::endl
		<< "logsystem version is " << ATRSHMLOG_GET_VERSION() << std::endl;
      std::exit(1);
    }

  std::cout << "shm log check for c++ code " << std::endl;
  

  ATRSHMLOG_ATTACH();

  int result_logging = ATRSHMLOG_GET_LOGGING();
  if (result_logging != 0)
    {
      std::cout << "check error " <<  result_logging << std::endl;

      exit(1);
    }

  
  atrshmlog_time_t starttime;
  atrshmlog_time_t endtime;

  ATRSHMLOG_GETTIME(0, starttime);
    
  for (int i = 0; i < 32 ; i++)
    {
      const int event = 0;
      const char eventflag = 'I';
      
      const int userevent = i;
      
      ATRSHMLOG_GETTIME(0, endtime);

      ATRSHMLOG_WRITE(event,
		     eventflag,
		     userevent,
		     starttime,
		     endtime,
		     "hallo world",
		     sizeof("hallo world") - 1
		     );
    }


  std::cout << " end of test." << std::endl;

  return 0;
}
