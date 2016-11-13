//
// atrshmlog test program for c++ code
//
// we use the C header, use the thing and bind via lib
//


/** \file atrshmlogtest03.C
 * \brief We are a C++ test programm with multi threading.
 */

#include "atrshmlog.h"


#include <iostream>

#include <chrono>

#include <thread>

#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <future>


#include <vector>

#include <new>

#include <unistd.h>

using namespace std::chrono;

/**
 * this is the count of int additions
 *
 * will be set from parameter for the worker threads
 */
long mainloopcount = 8;


/**
 * the thread execution function.
 */
long f(int i_loops, int th)
{
  volatile int i = 0;

  atrshmlog_time_t log_starttime;

  auto starttime = system_clock::now();


  volatile int h = 0;
  
  while (i_loops > 0)
    {

      ATRSHMLOG_GETTIME(0, log_starttime);

      const int event = th;
      const char eventflag = 'I';

      int userevent = i;

      i++;
      
      for (int k = 0; k < mainloopcount; k++)
	h = h + 1;

      i_loops--;

      
      
      atrshmlog_time_t log_endtime;
      
      ATRSHMLOG_GETTIME(0, log_endtime);

      ATRSHMLOG_WRITE(event,
		     eventflag,
		     userevent,
		     log_starttime,
		     log_endtime,
		     "hello world",
		     sizeof("hello world") - 1
		     );
    }

  auto delta = duration_cast<std::chrono::nanoseconds>(system_clock::now() - starttime);

  return (long)delta.count();
}

/**
 * the total number of nanosecs for the threads
 */
long  totalcount = 0;

/**
 * the number of nanoseconds fot the slowest thread
 */
long  maxcount = 0;


/**
 * WE dispatch the threads.
 */
void dispatcher(int i_count, int i_seconds)
{
  using namespace std;
  
  using Task_type = long (int, int); // our task is f ...

  using Future_type = future<long>;
  
  using vp_type = std::vector< packaged_task<Task_type> > ;

  using vf_type = std::vector<Future_type>;

  using vt_type = std::vector <std::thread*>;

  std::cout << "dispatcher has count " << i_count << " and looptime " << i_seconds << std::endl;
  
  // we hold the packages here
  vp_type vp;

  // we hold the futures here
  vf_type vf;

  // we hold the task memory pointers here
  // because copy of a task is at least dangerous we only hold the adress
  // given y new.
  vt_type the_tasks;

  int limit = i_count;
  
  // we create the packages and futures
  for (int i = 0; i < limit; i++)
    {
      vp.emplace_back(packaged_task<Task_type> (f));

      vf.emplace_back(Future_type(vp[i].get_future()));
    }

  std::cout << "packages made " << std::endl;

  // we create the tasks, which means also to run the task
  for (int i = 0 ; i < limit; i++)
    {
      the_tasks.push_back(new thread{move(vp[i]), i_seconds, i + 1}); 
    }

  // we dont need the packages any more
  vp.clear();

  // hack we wait for this thread now for 10 secounds

  sleep(10);
  
  // we wait for the end of the task by using join
  for (int i = 0; i < limit; i++)
    {
      std::thread* p = the_tasks[i];

      p->join();

      delete p;
    }

  // we can now savely destroy the tasks
  the_tasks.clear();

  totalcount = 0;
  maxcount = 0;
  // we get our results from the futures
  for (int i = 0; i < limit; i++)
    {
      long r = vf[i].get();
      
      std::cout <<  i << " " <<  r << std::endl;

      totalcount += r;

      if (maxcount < r) maxcount = r;
    }

  std::cout << "threads delivered futures " << totalcount << " " << maxcount << std::endl;

}


/**
 * we connect and then start multiple threads and do a simple counter loop.
 * We log in every thread in parallel.
 * then we start one thread to do this all in one for 
 * making a refernece for the whole work.
 */
int main (int argc, char* argv[])
{

  std::cout << "shm log check for c++ code and threads" << std::endl;
  

  ATRSHMLOG_ATTACH();

  int result_logging = ATRSHMLOG_GET_LOGGING();
  
  if (result_logging != 0)
    {
      std::cout << "check logging " <<  result_logging << std::endl;
    }

  
  atrshmlog_time_t starttime;
  atrshmlog_time_t endtime;

  ATRSHMLOG_GETTIME(0, starttime);
    
  for (int i = 0; i < 64 ; i++)
    {
      const int event = 888;
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

  volatile int hugo = -3;
      
  for (int k = 0; k < (1000 * 1000) ; k++ )
    for (int j = 0; j < (1000 * 5); j++)
      hugo += 3;
      
  std::cout << "start of threads." << std::endl;

  int taskcount = 10;
  int waittime = 10;

  if (argc > 1)
    {
      long new_taskcount = strtol(argv[1], NULL, 10);

      if (new_taskcount > 0 and new_taskcount < 300000)
	taskcount = new_taskcount;
    }

  if (argc > 2)
    {
      long new_time = strtol(argv[2], NULL, 10);

      if (new_time > 0 and new_time < 2000000000)
	waittime = new_time;
    }

  if (argc > 3)
    {
      long loopcount = strtol(argv[3], NULL, 10);

      if (loopcount > 8 and loopcount < 2000000000)
	mainloopcount = loopcount;
    }

  
  dispatcher(taskcount, waittime);


  long d = f(waittime * taskcount, 999);

  std::cout << "main " << d << std::endl;

  std::cout << (double)totalcount / d << " " << (double)d / maxcount << std::endl;
  std::cout << "end of test." << std::endl;
  {
    
  /* we give out the statistics */


  atrshmlog_int32_t maxindex = atrshmlog_get_statistics_max_index();


  atrshmlog_int32_t *dynstat = (atrshmlog_int32_t *)calloc(sizeof(atrshmlog_int32_t), maxindex + 1);

  atrshmlog_get_statistics(dynstat);

  for (int i = 0; i <= maxindex; ++i)
    {
      printf("atrshmlogstat %i %ld\n", i, (long)dynstat[i]);
    }
  }
  return 0;
}
