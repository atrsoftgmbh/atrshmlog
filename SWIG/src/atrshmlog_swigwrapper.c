/*
 * We need a real code thing here for the perl wrapper to link to
 */

/** \file atrshmlog_swigwrapper.c
 * \brief We implement the functions hat cannot be used directly from the interface.
 *
 * So this is the gettime, read write etc.
 */

#include "atrshmlog.h"

#include <string.h>
  

atrshmlog_time_t atrshmlog_gettime(atrshmlog_int32_t event)
{
  atrshmlog_time_t result;

  ATRSHMLOG_GETTIME(event,result);

  return result;
}


atrshmlog_ret_t atrshmlog_write_binary(const atrshmlog_int32_t i_eventnumber,
				       const atrshmlog_int32_t i_eventflag,
				       const atrshmlog_int32_t i_userflag,
				       atrshmlog_time_t i_starttime,
				       atrshmlog_time_t i_endtime,
				       const char* i_local,
				       const atrshmlog_int32_t i_size)
{
  return atrshmlog_write1(i_eventnumber,
			  i_eventflag,
			  i_userflag,
			  i_starttime,
			  i_endtime,
			  i_local,
			  i_size);
}

atrshmlog_ret_t atrshmlog_write(const atrshmlog_int32_t i_eventnumber,
				const atrshmlog_int32_t i_eventflag,
				const atrshmlog_int32_t i_userflag,
				atrshmlog_time_t i_starttime,
				atrshmlog_time_t i_endtime,
				const char* i_local)
{
  return atrshmlog_write1(i_eventnumber,
			  i_eventflag,
			  i_userflag,
			  i_starttime,
			  i_endtime,
			  i_local,
			  strlen(i_local));
}


void atratrshmlog_get_inittime(long* o_seconds, long *o_nanos)
{
  atrshmlog_internal_time_t result = atrshmlog_get_inittime();

  *o_seconds = result.tv_sec;
  *o_nanos = result.tv_nsec;
}

void atratrshmlog_get_realtime(long* o_seconds, long* o_nanos)
{
  atrshmlog_internal_time_t result = atrshmlog_get_realtime();

  *o_seconds = result.tv_sec;
  *o_nanos = result.tv_nsec;
}

void atratrshmlog_get_statistics(
				 int* o_s00,
				 int* o_s01,
				 int* o_s02,
				 int* o_s03,
				 int* o_s04,
				 int* o_s05,
				 int* o_s06,
				 int* o_s07,
				 int* o_s08,
				 int* o_s09,
				 int* o_s10,
				 int* o_s11,
				 int* o_s12,
				 int* o_s13,
				 int* o_s14,
				 int* o_s15,
				 int* o_s16,
				 int* o_s17,
				 int* o_s18,
				 int* o_s19,
				 int* o_s20,
				 int* o_s21,
				 int* o_s22,
				 int* o_s23,
				 int* o_s24,
				 int* o_s25,
				 int* o_s26,
				 int* o_s27,
				 int* o_s28,
				 int* o_s29,
				 int* o_s30,
				 int* o_s31,
				 int* o_s32,
				 int* o_s33,
				 int* o_s34,
				 int* o_s35,
				 int* o_s36,
				 int* o_s37,
				 int* o_s38,
				 int* o_s39,
				 int* o_s40,
				 int* o_s41,
				 int* o_s42,
				 int* o_s43,
				 int* o_s44,
				 int* o_s45,
				 int* o_s46,
				 int* o_s47,
				 int* o_s48,
				 int* o_s49,
				 int* o_s50,
				 int* o_s51,
				 int* o_s52,
				 int* o_s53,
				 int* o_s54,
				 int* o_s55,
				 int* o_s56,
				 int* o_s57,
				 int* o_s58,
				 int* o_s59,
				 int* o_s60,
				 int* o_s61,
				 int* o_s62,
				 int* o_s63,
				 int* o_s64,
				 int* o_s65,
				 int* o_s66,
				 int* o_s67,
				 int* o_s68,
				 int* o_s69,
				 int* o_s70,
				 int* o_s71,
				 int* o_s72,
				 int* o_s73,
				 int* o_s74,
				 int* o_s75,
				 int* o_s76,
				 int* o_s77,
				 int* o_s78,
				 int* o_s79,
				 int* o_s80,
				 int* o_s81,
				 int* o_s82,
				 int* o_s83,
				 int* o_s84,
				 int* o_s85,
				 int* o_s86,
				 int* o_s87,
				 int* o_s88,
				 int* o_s89,
				 int* o_s90,
				 int* o_s91,
				 int* o_s92,
				 int* o_s93,
				 int* o_s94,
				 int* o_s95,
				 int* o_s96,
				 int* o_s97,
				 int* o_s98,
				 int* o_s99)
{
  // e have 85 for now and i doubt it will clime much more ...
  atrshmlog_int32_t h[100];

  for (int i = 0; i < 100; i++)
    h[i] = 0;
  
  atrshmlog_get_statistics(h);

  *o_s00 = h[0];
  *o_s01 = h[1];
  *o_s02 = h[2];
  *o_s03 = h[3];
  *o_s04 = h[4];
  *o_s05 = h[5];
  *o_s06 = h[6];
  *o_s07 = h[7];
  *o_s08 = h[8];
  *o_s09 = h[9];
  *o_s10 = h[10];
  *o_s11 = h[11];
  *o_s12 = h[12];
  *o_s13 = h[13];
  *o_s14 = h[14];
  *o_s15 = h[15];
  *o_s16 = h[16];
  *o_s17 = h[17];
  *o_s18 = h[18];
  *o_s19 = h[19];
  *o_s20 = h[20];
  *o_s21 = h[21];
  *o_s22 = h[22];
  *o_s23 = h[23];
  *o_s24 = h[24];
  *o_s25 = h[25];
  *o_s26 = h[26];
  *o_s27 = h[27];
  *o_s28 = h[28];
  *o_s29 = h[29];
  *o_s30 = h[30];
  *o_s31 = h[31];
  *o_s32 = h[32];
  *o_s33 = h[33];
  *o_s34 = h[34];
  *o_s35 = h[35];
  *o_s36 = h[36];
  *o_s37 = h[37];
  *o_s38 = h[38];
  *o_s39 = h[39];
  *o_s40 = h[40];
  *o_s41 = h[41];
  *o_s42 = h[42];
  *o_s43 = h[43];
  *o_s44 = h[44];
  *o_s45 = h[45];
  *o_s46 = h[46];
  *o_s47 = h[47];
  *o_s48 = h[48];
  *o_s49 = h[49];
  *o_s50 = h[50];
  *o_s51 = h[51];
  *o_s52 = h[52];
  *o_s53 = h[53];
  *o_s54 = h[54];
  *o_s55 = h[55];
  *o_s56 = h[56];
  *o_s57 = h[57];
  *o_s58 = h[58];
  *o_s59 = h[59];
  *o_s60 = h[60];
  *o_s61 = h[61];
  *o_s62 = h[62];
  *o_s63 = h[63];
  *o_s64 = h[64];
  *o_s65 = h[65];
  *o_s66 = h[66];
  *o_s67 = h[67];
  *o_s68 = h[68];
  *o_s69 = h[69];
  *o_s70 = h[70];
  *o_s71 = h[71];
  *o_s72 = h[72];
  *o_s73 = h[73];
  *o_s74 = h[74];
  *o_s75 = h[75];
  *o_s76 = h[76];
  *o_s77 = h[77];
  *o_s78 = h[78];
  *o_s79 = h[79];
  *o_s80 = h[80];
  *o_s81 = h[81];
  *o_s82 = h[82];
  *o_s83 = h[83];
  *o_s84 = h[84];
  *o_s85 = h[85];
  *o_s86 = h[86];
  *o_s87 = h[87];
  *o_s88 = h[88];
  *o_s89 = h[89];
  *o_s90 = h[90];
  *o_s91 = h[91];
  *o_s92 = h[92];
  *o_s93 = h[93];
  *o_s94 = h[94];
  *o_s95 = h[95];
  *o_s96 = h[96];
  *o_s97 = h[97];
  *o_s98 = h[98];
  *o_s99 = h[99];

}

_Thread_local static char *read_buffer = 0; 


void atratrshmlog_read (volatile const void* i_area, 
			int i_index,
			int *o_ret,
			char *o_read_buffer, 
			int *o_length,		      
			long *o_pid,		     
			long *o_tid ,			
			long* o_inittime_seconds,	
			int* o_inittime_nanos,		   
			long* o_inittimetsc_before,	   
			long* o_inittimetsc_after,	   
			long* o_lasttime_seconds,		
			int* o_lasttime_nanos,			
			long* o_lasttimetsc_before,		
			long* o_lasttimetsc_after,		
			long* o_difftimetransfer,		
			long* o_starttransfer,			
			long* o_acquiretime,			
			int* o_id,				
			int* o_number_dispatched,		
			int* o_counter_write0,			
			int* o_counter_write0_discard,		
			int* o_counter_write0_wait,		
			int* o_counter_write0_adaptive,		
			int* o_counter_write0_adaptive_fast,	
			int* o_counter_write0_adaptive_very_fast, 
			int* o_counter_write1,			
			int* o_counter_write1_discard,		
			int* o_counter_write1_wait,		
			int* o_counter_write1_adaptive,		
			int* o_counter_write1_adaptive_fast,	
			int* o_counter_write1_adaptive_very_fast, 
			int* o_counter_write2,			
			int* o_counter_write2_discard,		
			int* o_counter_write2_wait,		
			int* o_counter_write2_adaptive,	       
			int* o_counter_write2_adaptive_fast,	
			int* o_counter_write2_adaptive_very_fast)
{
  atrshmlog_ret_t ret;
  atrshmlog_int32_t length;			  
  atrshmlog_pid_t pid;	    
  atrshmlog_tid_t tid;
  atrshmlog_internal_time_t inittime;
  atrshmlog_time_t inittimetsc_before;
  atrshmlog_time_t inittimetsc_after;
  atrshmlog_internal_time_t lasttime;
  atrshmlog_time_t lasttimetsc_before;
  atrshmlog_time_t lasttimetsc_after;
  atrshmlog_time_t difftimetransfer;
  atrshmlog_time_t starttransfer;
  atrshmlog_time_t acquiretime;		  
  atrshmlog_int32_t id;
  atrshmlog_int32_t number_dispatched;
  atrshmlog_int32_t counter_write0;
  atrshmlog_int32_t counter_write0_discard;
  atrshmlog_int32_t counter_write0_wait;
  atrshmlog_int32_t counter_write0_adaptive;
  atrshmlog_int32_t counter_write0_adaptive_fast;
  atrshmlog_int32_t counter_write0_adaptive_very_fast;
  atrshmlog_int32_t counter_write1;
  atrshmlog_int32_t counter_write1_discard;
  atrshmlog_int32_t counter_write1_wait;
  atrshmlog_int32_t counter_write1_adaptive;
  atrshmlog_int32_t counter_write1_adaptive_fast;
  atrshmlog_int32_t counter_write1_adaptive_very_fast;
  atrshmlog_int32_t counter_write2;			
  atrshmlog_int32_t counter_write2_discard;
  atrshmlog_int32_t counter_write2_wait;
  atrshmlog_int32_t counter_write2_adaptive;
  atrshmlog_int32_t counter_write2_adaptive_fast;
  atrshmlog_int32_t counter_write2_adaptive_very_fast;

  *o_length = 0;

  if (read_buffer == 0)
    read_buffer = malloc(1000000);
  
  ret = ATRSHMLOG_READ(i_area,
		       i_index,
		       read_buffer,
		       &length,
		       &pid,
		       &tid,
		       &inittime,
		       &inittimetsc_before,
		       &inittimetsc_after,
		       &lasttime,
		       &lasttimetsc_before,
		       &lasttimetsc_after,
		       &difftimetransfer,
		       &starttransfer,
		       &acquiretime,
		       &id,
		       &number_dispatched,
		       &counter_write0,
		       &counter_write0_discard,
		       &counter_write0_wait,
		       &counter_write0_adaptive,
		       &counter_write0_adaptive_fast,
		       &counter_write0_adaptive_very_fast,
		       &counter_write1,
		       &counter_write1_discard,
		       &counter_write1_wait,
		       &counter_write1_adaptive,
		       &counter_write1_adaptive_fast,
		       &counter_write1_adaptive_very_fast,
		       &counter_write2,
		       &counter_write2_discard,
		       &counter_write2_wait,
		       &counter_write2_adaptive,
		       &counter_write2_adaptive_fast,
		       &counter_write2_adaptive_very_fast);
  

  if (ret < 0)
    {
      *o_ret = ret;
      return;
    }

  if( ret > 0)
    {
      *o_ret = ret;
      return;
    }

  // ok. we have data
  *o_ret = 0;
  *o_length = length;

  if (length > 0)
    {
      memcpy(o_read_buffer, read_buffer, 1000000);

      *o_pid = pid;	    
      *o_tid = tid; 		       
      *o_inittime_seconds = inittime.tv_sec;	       
      *o_inittime_nanos = inittime.tv_nsec;
      *o_inittimetsc_before = inittimetsc_before;
      *o_inittimetsc_after = inittimetsc_after;
      *o_lasttime_seconds = lasttime.tv_sec;
      *o_lasttime_nanos = lasttime.tv_nsec;
      *o_lasttimetsc_before = lasttimetsc_before;
      *o_lasttimetsc_after = lasttimetsc_after;
      *o_difftimetransfer = difftimetransfer;
      *o_starttransfer = starttransfer;
      *o_acquiretime = acquiretime;
      *o_id = id;
      *o_number_dispatched = number_dispatched;
      *o_counter_write0 = counter_write0;
      *o_counter_write0_discard =counter_write0_discard;
      *o_counter_write0_wait = counter_write0_wait;
      *o_counter_write0_adaptive = counter_write0_adaptive;
      *o_counter_write0_adaptive_fast = counter_write0_adaptive_fast;
      *o_counter_write0_adaptive_very_fast = counter_write0_adaptive_very_fast;
      *o_counter_write1 = counter_write1;
      *o_counter_write1_discard = counter_write1_discard;
      *o_counter_write1_wait = counter_write1_wait;
      *o_counter_write1_adaptive = counter_write1_adaptive;
      *o_counter_write1_adaptive_fast = counter_write1_adaptive_fast;
      *o_counter_write1_adaptive_very_fast = counter_write1_adaptive_very_fast;
      *o_counter_write2 = counter_write2;
      *o_counter_write2_discard = counter_write2_discard;
      *o_counter_write2_wait = counter_write2_wait;
      *o_counter_write2_adaptive = counter_write2_adaptive;
      *o_counter_write2_adaptive_fast = counter_write2_adaptive_fast;
      *o_counter_write2_adaptive_very_fast = counter_write2_adaptive_very_fast;
    }
}


void atratrshmlog_read_fetch(volatile const void* i_area, 
			     int* o_ret,
			     int *o_index, 
			     char *o_read_buffer, 
			     int *o_length,			  
			     long *o_pid,	    
			     long *o_tid ,		       
			     long* o_inittime_seconds,	       
			     int* o_inittime_nanos,		 
			     long* o_inittimetsc_before,	  
			     long* o_inittimetsc_after,		  
			     long* o_lasttime_seconds,		  
			     int* o_lasttime_nanos,		  
			     long* o_lasttimetsc_before,	  
			     long* o_lasttimetsc_after,		  
			     long* o_difftimetransfer,		  
			     long* o_starttransfer,		  
			     long* o_acquiretime,		  
			     int* o_id,				  
			     int* o_number_dispatched,		  
			     int* o_counter_write0,		    
			     int* o_counter_write0_discard,	    
			     int* o_counter_write0_wait,	     
			     int* o_counter_write0_adaptive,		
			     int* o_counter_write0_adaptive_fast,	
			     int* o_counter_write0_adaptive_very_fast,	
			     int* o_counter_write1,			
			     int* o_counter_write1_discard,		
			     int* o_counter_write1_wait,		
			     int* o_counter_write1_adaptive,		
			     int* o_counter_write1_adaptive_fast,	
			     int* o_counter_write1_adaptive_very_fast,	
			     int* o_counter_write2,			
			     int* o_counter_write2_discard,		
			     int* o_counter_write2_wait,		
			     int* o_counter_write2_adaptive,		
			     int* o_counter_write2_adaptive_fast,	
			     int* o_counter_write2_adaptive_very_fast)
{
  atrshmlog_ret_t ret;
  atrshmlog_int32_t index; 
  atrshmlog_int32_t length;			  
  atrshmlog_pid_t pid;	    
  atrshmlog_tid_t tid;
  atrshmlog_internal_time_t inittime;
  atrshmlog_time_t inittimetsc_before;
  atrshmlog_time_t inittimetsc_after;
  atrshmlog_internal_time_t lasttime;
  atrshmlog_time_t lasttimetsc_before;
  atrshmlog_time_t lasttimetsc_after;
  atrshmlog_time_t difftimetransfer;
  atrshmlog_time_t starttransfer;
  atrshmlog_time_t acquiretime;		  
  atrshmlog_int32_t id;
  atrshmlog_int32_t number_dispatched;
  atrshmlog_int32_t counter_write0;
  atrshmlog_int32_t counter_write0_discard;
  atrshmlog_int32_t counter_write0_wait;
  atrshmlog_int32_t counter_write0_adaptive;
  atrshmlog_int32_t counter_write0_adaptive_fast;
  atrshmlog_int32_t counter_write0_adaptive_very_fast;
  atrshmlog_int32_t counter_write1;
  atrshmlog_int32_t counter_write1_discard;
  atrshmlog_int32_t counter_write1_wait;
  atrshmlog_int32_t counter_write1_adaptive;
  atrshmlog_int32_t counter_write1_adaptive_fast;
  atrshmlog_int32_t counter_write1_adaptive_very_fast;
  atrshmlog_int32_t counter_write2;			
  atrshmlog_int32_t counter_write2_discard;
  atrshmlog_int32_t counter_write2_wait;
  atrshmlog_int32_t counter_write2_adaptive;
  atrshmlog_int32_t counter_write2_adaptive_fast;
  atrshmlog_int32_t counter_write2_adaptive_very_fast;

  *o_length = 0;

  if (read_buffer == 0)
    read_buffer = malloc(1000000);
  
  ret = ATRSHMLOG_READ_FETCH(i_area,
			     &index,
			     read_buffer,
			     &length,
			     &pid,
			     &tid,
			     &inittime,
			     &inittimetsc_before,
			     &inittimetsc_after,
			     &lasttime,
			     &lasttimetsc_before,
			     &lasttimetsc_after,
			     &difftimetransfer,
			     &starttransfer,
			     &acquiretime,
			     &id,
			     &number_dispatched,
			     &counter_write0,
			     &counter_write0_discard,
			     &counter_write0_wait,
			     &counter_write0_adaptive,
			     &counter_write0_adaptive_fast,
			     &counter_write0_adaptive_very_fast,
			     &counter_write1,
			     &counter_write1_discard,
			     &counter_write1_wait,
			     &counter_write1_adaptive,
			     &counter_write1_adaptive_fast,
			     &counter_write1_adaptive_very_fast,
			     &counter_write2,
			     &counter_write2_discard,
			     &counter_write2_wait,
			     &counter_write2_adaptive,
			     &counter_write2_adaptive_fast,
			     &counter_write2_adaptive_very_fast);


  if (ret < 0)
    {
      *o_ret = ret;
      return;
    }

  if( ret > 0)
    {
      *o_ret = ret;
      return;
    }

  // ok. we have data
  *o_ret = 0;
  *o_index = index;
  *o_length = length;

  if (length > 0)
    {
      memcpy(o_read_buffer, read_buffer, 1000000);

      *o_pid = pid;	    
      *o_tid = tid; 		       
      *o_inittime_seconds = inittime.tv_sec;	       
      *o_inittime_nanos = inittime.tv_nsec;
      *o_inittimetsc_before = inittimetsc_before;
      *o_inittimetsc_after = inittimetsc_after;
      *o_lasttime_seconds = lasttime.tv_sec;
      *o_lasttime_nanos = lasttime.tv_nsec;
      *o_lasttimetsc_before = lasttimetsc_before;
      *o_lasttimetsc_after = lasttimetsc_after;
      *o_difftimetransfer = difftimetransfer;
      *o_starttransfer = starttransfer;
      *o_acquiretime = acquiretime;
      *o_id = id;
      *o_number_dispatched = number_dispatched;
      *o_counter_write0 = counter_write0;
      *o_counter_write0_discard =counter_write0_discard;
      *o_counter_write0_wait = counter_write0_wait;
      *o_counter_write0_adaptive = counter_write0_adaptive;
      *o_counter_write0_adaptive_fast = counter_write0_adaptive_fast;
      *o_counter_write0_adaptive_very_fast = counter_write0_adaptive_very_fast;
      *o_counter_write1 = counter_write1;
      *o_counter_write1_discard = counter_write1_discard;
      *o_counter_write1_wait = counter_write1_wait;
      *o_counter_write1_adaptive = counter_write1_adaptive;
      *o_counter_write1_adaptive_fast = counter_write1_adaptive_fast;
      *o_counter_write1_adaptive_very_fast = counter_write1_adaptive_very_fast;
      *o_counter_write2 = counter_write2;
      *o_counter_write2_discard = counter_write2_discard;
      *o_counter_write2_wait = counter_write2_wait;
      *o_counter_write2_adaptive = counter_write2_adaptive;
      *o_counter_write2_adaptive_fast = counter_write2_adaptive_fast;
      *o_counter_write2_adaptive_very_fast = counter_write2_adaptive_very_fast;
    }
}


