
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_inittimes_flag.c
 */

/**
 * \n Main code:
 *
 * \brief The init time values.
 *
 * Our time is meatured in tsc clicks.
 *
 * So we make one corresponding gettime and use two tsc 
 * to cornerstone this gettime.
 *
 * The medium of the two tsc is then assumed the real tsc for the gettime
 * which is of course wrong, but will reduce error somehow.
 *
 * With one from program start and one from buffer full we have a rough
 * estimation for the calc of real time for a tsc.
 *
 * Its better than trying to calc on a hypothetical base
 * but even this can go wrong if the machine is not
 * GOOD ENOUGH 
 * for holding tsc nice.
 *
 * The calculation for the ral time approximation is done in the convert.
 * So we dont do any of that in the module.
 */
_Alignas(128) atrshmlog_internal_time_t atrshmlog_inittime;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The click before we get the real time.
 */
_Alignas(128) atrshmlog_time_t atrshmlog_inittimetsc_before;


/********************************************************************/

/**
 * \n Main code:
 *
 * \brief The click after we get the ral time.
 */
_Alignas(128) atrshmlog_time_t atrshmlog_inittimetsc_after;

