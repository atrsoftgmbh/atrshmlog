
#include "../atrshmlog_internal.h"

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

/*******************************************************************/

/**
 * \file atrshmlogimpl_init_via_file.c
 */

/**
 * \brief We help the init to set a value from an file contents.
 * 
 * \param i_suffix
 * The Suffix of the file name after the env prefix.
 *
 * \param v
 * Points to an int that gets the value.
 *
 * \param i_min
 * Minimum allowed number to set.
 *
 * \param i_max
 * Maximum allowed number to set.
 *
 * \return 
 * void
 *
 * test t_attach.c
 */
void atrshmlog_init_via_file(const char *i_suffix,
				    int *v,
				    int i_min,
				    int i_max)
{
  char buff[256];
  
#if ATRSHMLOG_USE_SAFER_COPY == 1 
  strlcpy(buff, atrshmlog_prefix_name_buffer,256);
  strlcat(buff, i_suffix,256);
  strlcat(buff,".TXT",256);

#else 
  strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), i_suffix),".TXT");
#endif
  
  FILE* f= fopen(buff, "r");

  if (f)
    {
      long int new_max ;
			
      if (1 == fscanf(f,"%ld", &new_max))
	{
	  if (new_max >= i_min
	      && new_max <= i_max)
	    {
	      *v = new_max;
	      snprintf(buff, 256, "%s%s=%ld", atrshmlog_prefix_name_buffer, i_suffix, new_max);
	      putenv(buff);
	    }
	}
      
      fclose(f);
    }
}


