
#include "../atrshmlog_internal.h"


/********************************************************************/

/**
 * \file atrshmlogimpl_logging_off_final_flag.c
 */

/** 
 * \n Main code:
 *
 * \brief We have a per program switch for final log off .
 *
 * This can be used by the program to stop logging finally.
 *
 * We use this in all loop situations. So it works for the 
 * logging even if you are stuck in a mem to shm thing.
 *
 * But you have still to wait till all threads are awakening
 * and recognise it.
 * 
 * So this works not instantan, but with a little time shift.
 */
_Alignas(128) int atrshmlog_logging_process_off_final = 0;

