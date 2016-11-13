
#include "../atrshmlog_internal.h"

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
 */
void atrshmlog_init_via_file(const char *i_suffix,
				    int *v,
				    int i_min,
				    int i_max)
{
  char buff[256];
  
  strcat(strcat(strcpy(buff, atrshmlog_prefix_name_buffer), i_suffix),".TXT");
		    
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
	      sprintf(buff, "%s%s=%ld", atrshmlog_prefix_name_buffer, i_suffix, new_max);
	      putenv(buff);
	    }
	}
      
      fclose(f);
    }
}


