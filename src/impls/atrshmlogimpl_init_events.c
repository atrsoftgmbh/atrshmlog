
#include "../atrshmlog_internal.h"


/*******************************************************************/

/**
 * \file atrshmlogimpl_init_events.c
 */


/** 
 * \n Main code:
 *
 * \brief helper :  init the events
 * 
 * \param i_use_file
 * The flag to decide if we use the env variables or the flag files.
 *
 * \return 
 * void
 */
void atrshmlog_init_events(const int i_use_file)
{
  /* 
   * Switch on or off determination.
   * If this is null we switch off individual.
   * That means the event flags are on by default.
   * If we do set this it means we invert. 
   * The flags are off by default
   * so we have to switch them on individual
   */
  const char *evn = NULL;

  /*
   * This is the list of on and off switchers.
   * The main logic is determined by event null - see above
   * This is a list of events to be shut off in default mode
   * Is evn set - the thing is turned on instead.
   * So we can default the buffer to one of the 
   * two logical states and then switch individual off or on
   * counterwise.
   */
  const char *onoff = NULL;

  /* 
   * Helperbuffer for names with prefixes
   * Ok, this is dirty
   * I know
   * But i dont want to use dyn memory in here.
   * So i use a prefix_name_buffer that is some bytes and 
   * this local buffers... 
   */
  char buff[255];

  /* 
   * If we are a login shell, there are no env variables.
   * So in this case we use files
   * This is done here in case the flag is given
   * any thing else needs only the env vars 
   * See for the code in the case the env vars are used, then check the code
   * here. 
   * There is one diffrence, the use of the env for onoff
   * and the file is different - the file does not use a : as a 
   * delimiter.
   * Beside this it works the same - or should at least..
   */
      
  if (i_use_file)
    {
#if ATRSHMLOG_LEVEL > 0
      
#if ATRSHMLOG_FLAVOUR == 3 || ATRSHMLOG_FLAVOUR == 4
      strlcpy(buff,atrshmlog_prefix_name_buffer, 255);
      strlcat(buff, ATRSHMLOGEVENTNULLSUFFIX, 255);
      strlcat(buff, ".TXT", 255);
#else
      strcat(strcat(strcpy(buff,atrshmlog_prefix_name_buffer), ATRSHMLOGEVENTNULLSUFFIX),".TXT");
#endif 
      FILE *f = fopen(buff, "r");

      if (f)
	{
	  fclose(f);
	  evn = "on";
	}

      int flag = (evn == NULL) ? 1 : 0; /* on or off ... */

      memset((void*)atrshmlog_event_locks, flag, atrshmlog_event_locks_max);

#if ATRSHMLOG_FLAVOUR == 3 || ATRSHMLOG_FLAVOUR == 4
      strlcpy(buff, atrshmlog_prefix_name_buffer, 255);
      strlcat(buff, ATRSHMLOGEVENTONOFFSUFFIX, 255);
      strlcat(buff, ".TXT", 255);
#else
      strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), ATRSHMLOGEVENTONOFFSUFFIX),".TXT");
#endif

      f = fopen(buff, "r");

      if (f)
	{
	  long lindex;
      
	  flag = flag ? 0 : 1;

	  while (!feof(f) && !ferror(f)) {

	    if (1 == fscanf(f, "%ld", &lindex))
	      {
		long ev = lindex;

		if (ev < 0 || ev >= atrshmlog_event_locks_max)
		  ;
		else
		  ((atrshmlog_event_t*)atrshmlog_event_locks)[(int)ev] = flag;
	      }
	  }
      
	  fclose(f);
	}

#endif
      
      return;
    }

  /* 
   * Ok. We do not use a file, so we are not a login shell - or
   * we simply do not log at all. 
   * In that case we never reach this.
   * In the case we dont use a flag file we try to check the env
   */

  evn = atrshmlog_get_env(ATRSHMLOGEVENTNULLSUFFIX);

  onoff =  atrshmlog_get_env(ATRSHMLOGEVENTONOFFSUFFIX);

  int flag = (evn == NULL) ? 1 : 0; /* on or off ... */
  
  memset((void*)atrshmlog_event_locks, flag, atrshmlog_event_locks_max);

  /* The buffer is initialized. We can now switch on or off */

  /* 
   * We use a : separated string. 
   * Every event number is in the string
   * delimited by : or begin or end of the string.
   *
   * So we simply iterate from : to : to end and then have the number.
   *
   * If number is not in range we simply ignore it 
   *
   * Example ATRSHMLOG_EVENT_ONOFF=1:2:3:13:25:4711
   *
   * This will try to switch of the flags for event 1, 2 , 3, 13, 25 and 4711
   * (ok, we have a buffer of 10 k so this should be possible ... 
   * please dont abuse  - you only hurt yourself )
   *
   * If the env is set the thing turns upside down.
   * Then the buffer flag are all 0 and we turn these events on... 
   */
  if (onoff)
    {
      int limit = strlen(onoff);
      int j;
      int ende = 0;
      long ev;
      char* ptr;

      char buff[255];

      /* We invert the flag value meaning */
      flag = flag ? 0 : 1;
      
      for (j = 0; j < limit; j = ende + 1)
	{
	  for (ende = j;
	       onoff[ende] != '\0'
		 && onoff[ende] != ':'; 
	       ende++)
	    ;

	  // We handle the last separately
	  if (onoff[ende] == '\0')
	    break;

	  // Ignore an empty one
	  if ((ende - j) == 0)
	    continue;

	  // Ok. Copy candidate to helper
	  
	  int sz = ende - j ;

	  if (sz > 250)
	    sz = 0; // We ignore this one
	  else 
	    memcpy(buff, onoff + j, sz);

	  buff[sz + 1] = '\0';
	  
	  ev = strtol(buff, &ptr, 10);

	  if ((ptr - buff) != (ende - j))
	    {
	      ;  // We ignore non numbers 
	    }
	  else
	    {
	      if (ev < 0 || ev >= atrshmlog_event_locks_max)
		;
	      else
		((atrshmlog_event_t*)atrshmlog_event_locks)[(int)ev] = flag;
	    }
	}
      
      // We ignore empty entry
      if ((ende - j) == 0)
	return;

      if (onoff[j] != '\0')
	{
	  ev = strtol(onoff + j, &ptr, 10);

	  if ((ptr - (onoff + j)) != (ende - j))
	    {
	      ;  // We ignore non numbers 
	    }
	  else
	    {
	      if (ev < 0 || ev >= atrshmlog_event_locks_max)
		;
	      else
		((atrshmlog_event_t*)atrshmlog_event_locks)[(int)ev] = flag;
	    }
	}
    }
}

