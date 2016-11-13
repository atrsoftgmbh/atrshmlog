package de.atrsoft.successorofoak.utilities.logging.atrshmlog;

/**
 * \file ATRSHMLOG.java
 *  
 * The implementation of the jni layer
 *
 * We try to keep things simple.
 * so we use the C functions not directly, we use
 * the macros. Then we are somewhat decoupled for
 * the c functions itself.
 * 
 * We try to implemnt the simple interfaces of 
 * c functions as near as possible in the java methods.
 * So a int32 it a int, a int64 or uint 64 is a long.
 * The pointer stuff is handled by a long - 64 bit
 * as a pointer is at least enough in todays CPU 
 * world.
 * We use only some of the enum and define stuff here.
 * Were there is a enum in the c functions we resort to
 * int.
 * The most important changes are :
 * - gettime delivers a long, no internal setting as the GETTIME
 * macro does.
 * - write is overloaded to do the String and byte array thing.
 * - write is also overloaded to do the write range only thing,
 * which handles the problem of writing only a limited number
 * of chars from a string.
 * - the eventflag is used to determine if you want the 
 * write of an UTF or an UCS2 string. but only for the not limited
 * version. The limited is allways an Ucs2 string version.
 * - the read and read fetch use a long array for the parameters.
 * so you have to count the index in the array, and you have to 
 * deliver it in the first place.
 * - get realtime and get inittime use a long array for the 
 * parameters. So you have to deliver it in the first place.
 * - get g counters uses an int array as parameters. 
 * you have to deliver it in the first place.
 * - gettime is also available as a static. This is to test
 * if we gain any speed advantage in having it static.
 * If we can confirm an advantage we will also make the write s
 * static methods.
 * The member versions will still be available - who knows
 * what YOUR platform java thinks of speed ...
 * - the environment variable methods use internal UTF conversion
 * and so should only be used for the well known 7 bit ascii names
 * we have. Keep this in mind when you try the use of setEnvprefix.
 *
 * Test is now done in the ATRSHMLOGTest class. See the same
 * folder.
 *
 * We have still the c function comments in the code, this will
 * change in the next versions to the javadoc thing.
 *
 * The names of the methods are made by hand, but the scheme was
 * to rip of the prefix - its now the classname - and then
 * to do the camel case things with the wordgroups. This means
 * that the camel cas was not made if in the c function a group
 * consisted of more than one word.
 *
 * The main class for usage
 */
public class ATRSHMLOG {
    /**
     * We load the shared lib in classload.
     * Check for librarypath and for the actual
     * architecture to be used.
     * Check your lib directory or build the thing.
     * See doc page java layer
     */
    static {
	System.loadLibrary("atrshmlogjni");
    }

    /**
     * We define the counter index as an enum.
     * 
     * There is an array of ints that act as statistical counters.
     * 
     * You can get the counters with a helper.
     * 
     * To interpret the content you can use this enum.
     */
    static enum atrshmlog_counter {
	/**
	 *   We deliver in the get function the actual click time low part 
	 */
	atrshmlog_counter_time_low ( 0),

	/**
	 * We deliver in the get function the actual click time high part 
	 */
	atrshmlog_counter_time_high (1),

	/**
	 * The number of calls to @ref atrshmlog_attach() 
	 */
	atrshmlog_counter_attach (2),

	/**
	 *  The number of calls to @ref atrshmlog_il_get_raw_buffers() 
	 */
	atrshmlog_counter_get_raw (3),
	
	/**
	 * The number of calls to @ref atrshmlog_free() 
	 */
	atrshmlog_counter_free (4),

	/**
	 * The number of calls to @ref atrshmlog_alloc() 
	 */
	atrshmlog_counter_alloc (5),

	/**
	 * The number of calls to @ref atrshmlog_dispatch_buffer() 
	 */
	atrshmlog_counter_dispatch (6),
	
	/**
	 * The number of calls to @ref atrshmlog_transfer_mem_to_shm() 
	 */
	atrshmlog_counter_mem_to_shm (7),

	/**
	 * When @ref atrshmlog_transfer_mem_to_shm() actually starts to transfer 
	 */
	atrshmlog_counter_mem_to_shm_doit (8),

	/**
	 * When @ref atrshmlog_transfer_mem_to_shm() runs into a full shm buffer system and has to wait 
	 */
	atrshmlog_counter_mem_to_shm_full (9),

	/**
	 * The number of calls to @ref atrshmlog_create_slave() 
	 */
	atrshmlog_counter_create_slave (10),

	/**
	 * The number of calls to @ref atrshmlog_stop() 
	 */
	atrshmlog_counter_stop (11),

	/**
	 * The number of calls to @ref atrshmlog_write0() 
	 */
	atrshmlog_counter_write0 (12),

	/**
	 * When @ref atrshmlog_write0() exits because of error 
	 */
	atrshmlog_counter_write0_abort1 (13),
	/**
	 * When @ref atrshmlog_write0() exits because of error 
	 */
	atrshmlog_counter_write0_abort2 (14),

	/**
	 * When @ref atrshmlog_write0() exits because of error 
	 */
	atrshmlog_counter_write0_abort3 (15),

	/**
	 * When @ref atrshmlog_write0() exits because of error 
	 */
	atrshmlog_counter_write0_abort4 (16),

	/**
	 * When @ref atrshmlog_write0() exits because strategy discard 
	 */
	atrshmlog_counter_write0_discard (17),

	/** 
	 * When @ref atrshmlog_write0() waits because strategy  
	 */
	atrshmlog_counter_write0_wait (18),

	/**
	 * When @ref atrshmlog_write0() waits because strategy  
	 */
	atrshmlog_counter_write0_adaptive (19),

	/**
	 * When @ref atrshmlog_write0() waits because strategy  
	 */
	atrshmlog_counter_write0_adaptive_fast (20),

	/**
	 * When @ref atrshmlog_write0() waits because strategy  
	 */
	atrshmlog_counter_write0_adaptive_very_fast (21),

	/**
	 * When @ref atrshmlog_write0() exits because safeguard error 
	 */
	atrshmlog_counter_write_safeguard (22),

	/**
	 * When @ref atrshmlog_write0() exits because safeguard error 
	 */
	atrshmlog_counter_write_safeguard_shm (23),

	/**
	 * The number of calls to @ref atrshmlog_write1() 
	 */
	atrshmlog_counter_write1 (24),

	/**
	 * When @ref atrshmlog_write1() exits because of error 
	 */
	atrshmlog_counter_write1_abort1 (25),

	/**
	 * When @ref atrshmlog_write1() exits because of error 
	 */
	atrshmlog_counter_write1_abort2 (26),

	/**
	 * When @ref atrshmlog_write1() exits because of error 
	 */
	atrshmlog_counter_write1_abort3  (27),

	/**
	 * When @ref atrshmlog_write1() exits because of error 
	 */
	atrshmlog_counter_write1_abort4  (28),

	/**
	 * When @ref atrshmlog_write1() exits because strategy discard 
	 */
	atrshmlog_counter_write1_discard  (29),

	/**
	 * When @ref atrshmlog_write1() waits because strategy 
	 */
	atrshmlog_counter_write1_wait (30),

	/**
	 * When @ref atrshmlog_write1() waits because strategy 
	 */
	atrshmlog_counter_write1_adaptive (31),

	/**
	 * When @ref atrshmlog_write1() waits because strategy 
	 */
	atrshmlog_counter_write1_adaptive_fast (32),

	/**
	 * When @ref atrshmlog_write1() waits because strategy 
	 */
	atrshmlog_counter_write1_adaptive_very_fast(33),

	/**
	 * When @ref atrshmlog_write1() exits because of error 
	 */
	atrshmlog_counter_write1_abort5 (34),

	/**
	 * When @ref atrshmlog_write1() exits because of error 
	 */
	atrshmlog_counter_write1_abort6 (35),

	/**
	 * When @ref atrshmlog_write1() exits because of error 
	 */
	atrshmlog_counter_write1_abort7 (36),

	/**
	 * The number of calls to @ref atrshmlog_write2() 
	 */
	atrshmlog_counter_write2 (37),

	/**
	 * When @ref atrshmlog_write2() exits because of error 
	 */
	atrshmlog_counter_write2_abort1 (38),

	/**
	 * When @ref atrshmlog_write2() exits because of error 
	 */
	atrshmlog_counter_write2_abort2  (39),

	/**
	 * When @ref atrshmlog_write2() exits because of error 
	 */
	atrshmlog_counter_write2_abort3  (40),

	/**
	 * When @ref atrshmlog_write2() exits because of error 
	 */
	atrshmlog_counter_write2_abort4  (41),

	/**
	 * When @ref atrshmlog_write2() exits because strategy discard 
	 */
	atrshmlog_counter_write2_discard  (42),

	/**
	 * When @ref atrshmlog_write2() waits because strategy 
	 */
	atrshmlog_counter_write2_wait (43),

	/**
	 * When @ref atrshmlog_write2() waits because strategy 
	 */
	atrshmlog_counter_write2_adaptive (44),

	/**
	 * When @ref atrshmlog_write2() waits because strategy 
	 */
	atrshmlog_counter_write2_adaptive_fast (45),

	/**
	 * When @ref atrshmlog_write2() waits because strategy 
	 */
	atrshmlog_counter_write2_adaptive_very_fast(46),

	/**
	 * When @ref atrshmlog_write2() exits because of error 
	 */
	atrshmlog_counter_write2_abort5 (47),

	/**
	 * When @ref atrshmlog_write2() exits because of error 
	 */
	atrshmlog_counter_write2_abort6 (48),

	/**
	 * When @ref atrshmlog_write2() exits because of error 
	 */
	atrshmlog_counter_write2_abort7 (49),

	/**
	 * The number of calls to @ref atrshmlog_set_f_list_buffer_slave_count() 
	 */
	atrshmlog_counter_set_slave_count (50),

	/**
	 * The number of calls to @ref atrshmlog_set_clock_id() 
	 */
	atrshmlog_counter_set_clock_id (51),

	/**
	 * The number of calls to @ref atrshmlog_set_f_list_buffer_slave_run_off()
	 */
	atrshmlog_counter_slave_off (52),

	/**
	 * The number of calls to @ref atrshmlog_set_event_locks_max() 
	 */
	atrshmlog_counter_set_event_locks (53),

	/**
	 * The number of calls to @ref atrshmlog_set_buffer_infosize() 
	 */
	atrshmlog_counter_set_buffer_size (54),

	/**
	 * The number of calls to @ref atrshmlog_set_wait_for_slaves_on() 
	 */
	atrshmlog_counter_set_wait_slaves_on (55),

	/**
	 * The number of calls to @ref atrshmlog_set_wait_for_slaves_off() 
	 */
	atrshmlog_counter_set_wait_slaves_off (56),

	/**
	 * The number of calls to @ref atrshmlog_set_f_list_buffer_slave_wait() 
	 */
	atrshmlog_counter_set_slave_wait (57),

	/**
	 * The number of calls to @ref atrshmlog_set_prealloc_buffer_count() 
	 */
	atrshmlog_counter_set_prealloc_count (58),

	/**
	 * The number of calls to @ref atrshmlog_set_thread_fence() 
	 */
	atrshmlog_counter_set_thread_fence (59),

	/**
	 * The number of calls to @ref atrshmlog_create() 
	 */
	atrshmlog_counter_create (60),

	/**
	 * The @ref atrshmlog_create() exits because of error 
	 */
	atrshmlog_counter_create_abort1 (61),

	/**
	 * The @ref atrshmlog_create() exits because of error 
	 */
	atrshmlog_counter_create_abort2 (62),

	/**
	 * The @ref atrshmlog_create() exits because of error 
	 */
	atrshmlog_counter_create_abort3 (63),

	/**
	 * The @ref atrshmlog_create() exits because of error 
	 */
	atrshmlog_counter_create_abort4 (64),

	/**
	 * The number of calls to @ref atrshmlog_delete() 
	 */
	atrshmlog_counter_delete (65),

	/**
	 * The number of calls to @ref atrshmlog_cleanup_locks() 
	 */
	atrshmlog_counter_cleanup_locks (66),

	/**
	 * The number of calls to @ref atrshmlog_init_shm_log() 
	 */
	atrshmlog_counter_init_shm  (67),

	/**
	 * The number of calls to @ref atrshmlog_read() 
	 */
	atrshmlog_counter_read  (68),

	/**
	 * The @ref atrshmlog_read() transfers a buffer 
	 */
	atrshmlog_counter_read_doit (69),

	/**
	 * The number of calls to @ref atrshmlog_read_fetch() 
	 */
	atrshmlog_counter_read_fetch (70),

	/**
	 * The  @ref atrshmlog_read_fetch() transfers a buffer 
	 */
	atrshmlog_counter_read_fetch_doit (71),

	/**
	 * The number of calls to @ref atrshmlog_verify() 
	 */
	atrshmlog_counter_verify  (72),

	/**
	 * The number of calls to @ref atrshmlog_set_logging_process_on() 
	 */
	atrshmlog_counter_logging_process_on (73),

	/**
	 * The number of calls to @ref atrshmlog_set_logging_process_off() 
	 */
	atrshmlog_counter_logging_process_off (74),

	/**
	 * The number of calls to @ref atrshmlog_set_strategy() 
	 */
	atrshmlog_counter_set_strategy (75),

	/**
	 * The number of calls to @ref atrshmlog_set_strategy_process() 
	 */
	atrshmlog_counter_set_strategy_process (76),

	/**
	 * The number of calls to @ref atrshmlog_set_event_on() 
	 */
	atrshmlog_counter_set_event (77),

	/**
	 * The number of calls to @ref atrshmlog_set_event_off() 
	 */
	atrshmlog_counter_set_env_prefix (78),

	/**
	 * The number of calls to @ref atrshmlog_exit_cleanup() 
	 */
	atrshmlog_counter_exit_cleanup (79),

	/**
	 * The number of calls to @ref atrshmlog_flush() 
	 */
	atrshmlog_counter_flush (80),

	/**
	 * The number of calls to @ref atrshmlog_set_logging_process_off_final() 
	 */
	atrshmlog_counter_logging_process_off_final (81),

	/**
	 * The number of calls to @ref  atrshmlog_turn_logging_off() 
	 */
	atrshmlog_counter_turn_logging_off (82),

	/**
	 * The number of calls to @ref  atrshmlog_init_buffers_in_advance_on() 
	 */
	atrshmlog_counter_init_in_advance_on (83),
	
	/**
	 * The number of calls to @ref  atrshmlog_init_buffers_in_advance_on() 
	 */
	atrshmlog_counter_init_in_advance_off (84),

	/**
	 * The number of calls to @ref  atrshmlog_reuse_thread_buffers() 
	 */
	atrshmlog_counter_reuse_thread_buffers (85);

    	final int  err_code;
    	
    	private atrshmlog_counter(int err_code) {
	    this.err_code = err_code;
	}

	public static atrshmlog_counter fromCode(int logresult) {
	    for (atrshmlog_counter e :  values()) {
		if (e.err_code == logresult) {
		    return e;
		}
	    }
			
	    // default
	    return atrshmlog_counter_time_low;
	}
        
    };


    /**
     * The error codes as enums for the better usage.
     * The module delivers them in an int, so you can 
     * use the enum after conversion.
     * To circumvent a performance penalty you sould
     * do this after a real error is encountered. 
     * Real error is a value thats not 0.
     */
    static enum atrshmlog_error {

	/**
	 * Operation was successful, no error.
	 */
        atrshmlog_error_ok (0),

	/**
	 * Generic error 1 happend.
	 */
	atrshmlog_error_error (-1),

	/**
	 * Generic error 2 happend. 
	 */
	atrshmlog_error_error2 (-2),

	/**
	 * Generic error 3 happend.
	 */
	atrshmlog_error_error3 (-3),

	/**
	 * Generic error 4 happend.
	 */
	atrshmlog_error_error4 (-4),

	/**
	 * Generic error 5 happend.
	 */
	atrshmlog_error_error5 (-5),


	/**
	 * Buffer list is NULL in atrshmlog_il_connect_buffers_list. 
	 */
	atrshmlog_error_connect_1 (-11),

	/**
	 * Buffer count is negative in atrshmlog_il_connect_buffers_list. 
	 */
        atrshmlog_error_connect_2 (-12),

	/**
	 * The init was not successful in atrshmlog_init_thread_local.
	 */
        atrshmlog_error_init_thread_local_1 (-21),

	/**
	 * The shared memory is not connected in atrshmlog_transfer_mem_to_shm. 
	 */
        atrshmlog_error_mem_to_shm_1 (-31),

	/**
	 * The transfer buffer is NULL in atrshmlog_transfer_mem_to_shm. 
	 */
	atrshmlog_error_mem_to_shm_2 (-32),

	/**
	 * The size is 0, no operation in atrshmlog_transfer_mem_to_shm. 
	 */
        atrshmlog_error_mem_to_shm_3 (31),

	/**
	 * The maximum size is exceeded in atrshmlog_transfer_mem_to_shm for the buffer itself.
	 */
        atrshmlog_error_mem_to_shm_4 (-33),

	/**
	 * The logging was off for the thread in atrshmlog_transfer_mem_to_shm.
	 */
        atrshmlog_error_mem_to_shm_5 (-34),

	/**
	 * The safeguard was corrupt in atrshmlog_transfer_mem_to_shm. 
	 */
        atrshmlog_error_mem_to_shm_6 (-35),

	/**
	 * The logging was off for the system in area for atrshmlog_transfer_mem_to_shm. 
	 */
        atrshmlog_error_mem_to_shm_7 (32),

	/**
	 * The logging was final off in atrshmlog_transfer_mem_to_shm. 
	 */
        atrshmlog_error_mem_to_shm_8 (33),

	/**
	 * The attach was already done in atrshmlog_attach. 
	 */
	atrshmlog_error_attach_1 (41),

	/**
	 * Could not find a valid environment or file approch in atrshmlog_attach. 
	 */
        atrshmlog_error_attach_2 (-41),

	/**
	 * The cleanup could not sucessful made atexit in atrshmlog_attach. 
	 */
        atrshmlog_error_attach_3 (-42),

	/**
	 * The flagfile seems to be corrupt in atrshmlog_attach. 
	 */
        atrshmlog_error_attach_4 (-43),

	/**
	 * No flagfile option for this level in atrshmlog_attach. 
	 */
        atrshmlog_error_attach_5 (-44),

	/**
	 * The cleanup could not sucessful made atexit in atrshmlog_attach. 
	 */
        atrshmlog_error_attach_6 (-45),

	/**
	 * Buffer allocation failure in atrshmlog_init_in_write. 
	 */
        atrshmlog_error_init_in_write_1 (-51),

	/**
	 * Eventnumber negative in atrshmlog_write0. 
	 */
        atrshmlog_error_write0_1 (-61),

	/**
	 * Eventnumber too big in atrshmlog_write0.
	 */
        atrshmlog_error_write0_2 (-62),

	/**
	 * Logging off for the thread in atrshmlog_write0. 
	 */
        atrshmlog_error_write0_3 (-63),

	/**
	 * Buffer full discard in atrshmlog_write0. 
	 */
        atrshmlog_error_write0_4 (61),

	/**
	 * Logging off final in atrshmlog_write0. 
	 */
        atrshmlog_error_write0_5 (62),

	/**
	 * Logging off in area in atrshmlog_write0. 
	 */
	atrshmlog_error_write0_6 (63),

	/**
	 * Safeguard corrupt in atrshmlog_write0. 
	 */
        atrshmlog_error_write0_7 (-64),

	/**
	 * Area safeguard corrupt in atrshmlog_write0. 
	 */
        atrshmlog_error_write0_8 (-65),

	/**
	 * Logging off in area  atrshmlog_write0. 
	 */
	atrshmlog_error_write0_9 (64),

	/**
	 * Size payload is negative in atrshmlog_write1. 
	 */
        atrshmlog_error_write1_1 (-71),

	/**
	 * Eventnumber negative in atrshmlog_write1. 
	 */
        atrshmlog_error_write1_2 (-72),

	/**
	 * Eventnumber too big in atrshmlog_write1. 
	 */
        atrshmlog_error_write1_3 (-73),

	/**
	 * Payload too big for logging in atrshmlog_write1. 
	 */
        atrshmlog_error_write1_4 (-74),

	/**
	 * Logging off for the thread in atrshmlog_write1. 
	 */
        atrshmlog_error_write1_5 (-75),

	/**
	 * Buffer full discard in  atrshmlog_write1. 
	 */
        atrshmlog_error_write1_6 (71),

	/**
	 * Logging off final in atrshmlog_write1. 
	 */
        atrshmlog_error_write1_7 (72),

	/**
	 * Logging off in area in atrshmlog_write1.
	 */
        atrshmlog_error_write1_8 (73),

	/**
	 * Payload too big for logging in atrshmlog_write1.
	 */
        atrshmlog_error_write1_9 (-76),

	/**
	 * Safeguard corrupt in atrshmlog_write1. 
	 */
        atrshmlog_error_write1_10 (-77),

	/**
	 * Area safeguard corrupt in atrshmlog_write1. 
	 */
        atrshmlog_error_write1_11 (-78),

	/**
	 * Logging off in area in atrshmlog_write1. 
	 */
        atrshmlog_error_write1_12 (74),

	/**
	 * Size payload is negative in atrshmlog_write2. 
	 */
        atrshmlog_error_write2_1 (-81),

	/**
	 * Eventnumber negative in atrshmlog_write2.
	 */
        atrshmlog_error_write2_2 (-82),

	/**
	 * Eventnumber too big in atrshmlog_write2. 
	 */
        atrshmlog_error_write2_3 (-83),

	/**
	 * Payload too big for logging in atrshmlog_write2. 
	 */
        atrshmlog_error_write2_4 (-84),

	/**
	 * Logging off for the thread in atrshmlog_write2. 
	 */
        atrshmlog_error_write2_5 (-85),

	/**
	 * Buffer full discard in atrshmlog_write2. 
	 */
        atrshmlog_error_write2_6 (81),

	/**
	 * Logging off final in  atrshmlog_write2. 
	 */
        atrshmlog_error_write2_7 (82),

	/**
	 * Logging off in area in  atrshmlog_write2. 
	 */
        atrshmlog_error_write2_8 (83),

	/**
	 * Payload too big for logging in atrshmlog_write2. 
	 */
        atrshmlog_error_write2_9 (-86),

	/**
	 * Safeguard corrupt in atrshmlog_write2. 
	 */
        atrshmlog_error_write2_10 (-87),

	/**
	 * Area safeguard corrupt in atrshmlog_write2. 
	 */
        atrshmlog_error_write2_11 (-88),

	/**
	 * Logging off in area in  atrshmlog_write2. 
	 */
        atrshmlog_error_write2_12 (84),

	/**
	 * Area not connected in atrshmlog_get_area_version. 
	 */
        atrshmlog_error_area_version_1 (-91),

	/**
	 * Area not connected in atrshmlog_get_area_count. 
	 */
        atrshmlog_error_area_count_1 (-101),

	/**
	 * Area not connected in atrshmlog_set_area_ich_habe_fertig.
	 */
        atrshmlog_error_area_ich_habe_fertig_1 (-111),

	/**
	 * Index out of range in atrshmlog_get_event. 
	 */
        atrshmlog_error_get_event_1 (-121),

	/**
	 * Area not connected in atrshmlog_get_logging. 
	 */
        atrshmlog_error_get_logging_1 (131),

	/**
	 * Area logging flag off in atrshmlog_get_logging. 
	 */
        atrshmlog_error_get_logging_2 (132),

	/**
	 * Logging final off in atrshmlog_get_logging. 
	 */
        atrshmlog_error_get_logging_3 (133),

	/**
	 * Process logging off in atrshmlog_get_logging.
	 */
        atrshmlog_error_get_logging_4 (134),

	/**
	 * Ipc key out of range in atrshmlog_create. 
	 */
        atrshmlog_error_create_1 (-141),

	/**
	 * Buffer count to low in  atrshmlog_create. 
	 */
        atrshmlog_error_create_2 (-142),

	/**
	 * Buffer count to high in  atrshmlog_create. 
	 */
        atrshmlog_error_create_3 (-143),

	/**
	 * Connect failed in low level call in atrshmlog_create. 
	 */
        atrshmlog_error_create_4 (-144),

	/**
	 * Area is NULL in atrshmlog_init_shm_log. 
	 */
        atrshmlog_error_init_shm_1 (-151),

	/**
	 * Shared memory id diffrent for process and area in atrshmlog_init_shm_log. 
	 */
        atrshmlog_error_init_shm_2 (-152),

	/**
	 * Low level init failed in atrshmlog_init_shm_log. 
	 */
        atrshmlog_error_init_shm_3 (-153),

	/**
	 * Area is NULL in atrshmlog_read. 
	 */
        atrshmlog_error_read_1 (-161),

	/**
	 * Buffer index negative in atrshmlog_read. 
	 */
        atrshmlog_error_read_2 (-162),

	/**
	 * Buffer index too big in atrshmlog_read. 
	 */
        atrshmlog_error_read_3 (-163),

	/**
	 * Area safeguard corrupt in atrshmlog_read. 
	 */
        atrshmlog_error_read_4 (-164),

	/**
	 * Buffer size in shared memory too big in atrshmlog_read.
	 */
        atrshmlog_error_read_5 (-165),

	/**
	 * Area is NULL in atrshmlog_read_fetch. 
	 */
        atrshmlog_error_read_fetch_1 (-171),

	/**
	 * Area safeguard corrupt in atrshmlog_read_fetch. 
	 */
        atrshmlog_error_read_fetch_2 (-172),

	/**
	 * No buffer to be processed in atrshmlog_read_fetch.
	 */
        atrshmlog_error_read_fetch_3 (171),

	/**
	 * ABuffer size in shared memory too big in atrshmlog_read_fetch.
	 */
        atrshmlog_error_read_fetch_4 (-173),

	/**
	 * Buffer has size 0 in shared memory too big in atrshmlog_read_fetch. 
	 */
        atrshmlog_error_read_fetch_5 (172),

	/**
	 * Area not attached in atrshmlog_verify.
	 */
        atrshmlog_error_verify_1 (-181),

	/**
	 * Shared memory id differs to process shared memory id in atrshmlog_verify. 
	 */
        atrshmlog_error_verify_2 (-182),

	/**
	 * Area safeguard corrupted in atrshmlog_verify. 
	 */
        atrshmlog_error_verify_3 (-183),

	/**
	 * Version mismatch for area and module code in atrshmlog_verify. 
	 */
	atrshmlog_error_verify_4 (-184),
	
	/**
	 * Area buffer state corrupt in atrshmlog_verify. 
	 */
        atrshmlog_error_verify_5 (-185),

	/**
	 * Buffer safeguard corrupted in atrshmlog_verify. 
	 */
        atrshmlog_error_verify_6 (-186);
    	
    	final int  err_code;
    	
    	private atrshmlog_error(int err_code) {
	    this.err_code = err_code;
	}

	public static atrshmlog_error fromCode(int logresult) {
	    if (logresult == atrshmlog_error_ok.err_code) {
		return atrshmlog_error_ok;
	    }
	    for (atrshmlog_error e :  values()) {
		if (e.err_code == logresult) {
		    return e;
		}
	    }
			
	    // default
	    return atrshmlog_error_error;
	}
        
    };
    
    
    /**
     * We have these strategy options in waiting for the log 
     * to have a free buffer .
     *
     * So we need to hold these in the process
     * and in the thread scope.
     *
     * See the doc for info.
     */
    static enum atrshmlog_strategy {
    
	/**
	 * We discard the log 
	 */
	atrshmlog_strategy_discard (0),
    
	/**
	 * We spin loop till one is free
	 */
	atrshmlog_strategy_spin_loop (1),
    
	/**
	 * We wait for a defined time in loop
	 */
	atrshmlog_strategy_wait (2),
    
	/**
	 * We use an adaptiv approch with the slavetime
	 */
	atrshmlog_strategy_adaptive (3),

	/**
	 * We use an adaptive approch with double speed.
	 */
	atrshmlog_strategy_adaptive_fast (4),
    
	/**
	 * We use an adaptive approch with ten times  speed.
	 */
	atrshmlog_strategy_adaptive_very_fast (5);


    	final int  err_code;
    	
    	private atrshmlog_strategy(int err_code) {
	    this.err_code = err_code;
	}

	public static atrshmlog_strategy fromCode(int logresult) {
	    for (atrshmlog_strategy e :  values()) {
		if (e.err_code == logresult) {
		    return e;
		}
	    }
			
	    // default
	    return atrshmlog_strategy_wait;
	}
        
    };

    

    /**
     * the eventflag for point in time logging and c string
     * payload
     */
    public final static int EVENT_POINT_IN_TIME_C = 64 + 16; 

    /**
     * the eventflag for interval in time logging and c string
     * payload
     */
    public final static int EVENT_INTERVAL_IN_TIME_C = 64 + 9;

    /**
     * the eventflag for point in time logging and ucs2
     * payload
     */
    public final static int EVENT_POINT_IN_TIME_UCS = 96 + 16; 

    /**
     * the eventflag for interval in time logging and ucs2
     * payload
     */
    public final static int EVENT_INTERVAL_IN_TIME_UCS = 96 + 9;

    /**********************************************************/
    /* methods */
    /**********************************************************/
    

    
    /** 
     * We connect to the shm buffer. Only connect, no use or checking.
     * 
     * We also do all the major initialisation stuff here, use all the 
     * environment variables and the flag files in case we do the login thing..
     *
     * So this is a simple but very important function.
     * 
     * All is covered inside.
     *
     * @return
     * - Zero if all went well.
     * - Negative if no connect was possible.
     * - Positive if we were already connected.
     */
    public native int attach();

    /** 
     * Get the click time into the time variable.
     *
     * We use implicit an assignment, so you dont have to type all this many 
     * by yourself.
     * This is hard wired to the low level macro, so you can either
     * change here or in that macro if you need.
     * And we do the check for logging.
     * On my box a low level time get is 40 clicks, so a check for
     * lets say 2 clicks is worth the thing.
     * We can reduce register polution and cache pollution by 
     * doing this if a time is not needed.. 
     *
     * @param eventnumber
     * The event for this gettime.
     *
     * @return
     * The value of the time.
     */
    public native long gettime(int eventnumber);

    /**
     * for investigation only. are statics faster ?
     *
     * please check this and give me a feedback for your platform
     *
     * in case they are the writes will be made static too.
     */
    public static native long sgettime(int eventnumber);

    /** 
     * We write an entry, no payload.
     *
     * This is the first of the three write functions. It makes the write
     * to the internal buffer for the thread.
     *
     * We have no additinal payload beside the standard infos.
     *
     *  @param  i_eventnumber
     * A 32 bit event number. This should not be overused, check for 
     * the event flag limit. You should hold this stable, meaning
     * it should identify the spot of logging.
     * So you can swith it on or off via events. And it identifies 
     * the spot where you log.
     *
     *  @param  i_eventflag
     * The flag to identify the kind of logging.
     * Its a well known fact that we use normally an interval. So the 
     * flag is indead a char and it is hten a big I ....
     * If you need a instant info - like an additional info or so - you 
     * normally use no interval. This is then a point in time entry, so I use a
     * big P for this.
     * Anything else is left for the user. But you are limited to a char, and 
     * it is a BAD THING t switch to unprintable values ....
     * Update: a small i and a small p is used in the Java jni layer.
     * So dont use them. The converter knows little about it,
     * but it is at least aware of it and trys its best.
     *
     *  @param  i_userflag
     * This is a 32 integer and its totally for the user.
     * So you can put inside as an info what you think is so important 
     * that it must come out.
     * Because it is the ONLY user info that is free of limitations
     * beside the size use it wisely - I suggest you use it together with
     * the event to make some kind of combined has map key and then 
     * use it to give you rough 10000 * 4 billion possible strings as 
     * real info ( if this is not enough you have to use write 1 or write 2 ...)
     * which is a lot of informafion for many people ...
     *
     *  @param  i_starttime
     * a click time. 
     *
     *  @param  i_endtime
     * a click time.
     * Side note: per convention the convert use a starttime and a endtime to 
     * do some calculation to interpret this as the start of the log entry
     * and end of the log entra. IT calcs the diffrence as the duration.
     * And it try to calc the real times with a linear approch from the 
     * time points init and last and the values for start and end assuming
     * they were taken with the same click counter - i.e. it claculates
     * an interpretes real time assuming you got the tick time 
     * and put it into start and ende...
     * So dont play games with those. They are used and if you insist you can
     * even corrupt the converter by using some nonsence data.
     *
     * @return
     * - 0 for a working log write
     * - else for an error or a suppressed one
     */

    public native int write(int eventnumber,
			    int eventflag,
			    int userflag,
			    long starttime,
			    long endtime);
    
    /** 
     * We write an entry, payload is a String
     *
     * This is the first of the three write functions. It makes the write
     * to the internal buffer for the thread.
     *
     * We have no additinal payload beside the standard infos.
     *
     *  @param  i_eventnumber
     * A 32 bit event number. This should not be overused, check for 
     * the event flag limit. You should hold this stable, meaning
     * it should identify the spot of logging.
     * So you can swith it on or off via events. And it identifies 
     * the spot where you log.
     *
     *  @param  i_eventflag
     * The flag to identify the kind of logging.
     * Its a well known fact that we use normally an interval. So the 
     * flag is indead a char and it is hten a big I ....
     * If you need a instant info - like an additional info or so - you 
     * normally use no interval. This is then a point in time entry, so I use a
     * big P for this.
     * Anything else is left for the user. But you are limited to a char, and 
     * it is a BAD THING to switch to unprintable values ....
     * Update: a small i and a small p is used in the Java jni layer.
     * So dont use them. The converter knows little about it,
     * but it is at least aware of it and trys its best.
     * Update of Update: the converter now knows at least a bit
     * and so we now use the small i and p for java strings
     * and the big I and P switch internal to UTF conversion.
     *
     * So we now have a write for technicals,
     * one for Strings as utf or as  ucs2,
     * one for ucs and a limited write, 
     * and one at last a byte array 
     * 
     *  @param  i_userflag
     * This is a 32 integer and its totally for the user.
     * So you can put inside as an info what you think is so important 
     * that it must come out.
     * Because it is the ONLY user info that is free of limitations
     * beside the size use it wisely - I suggest you use it together with
     * the event to make some kind of combined has map key and then 
     * use it to give you rough 10000 * 4 billion possible strings as 
     * real info ( if this is not enough you have to use write 1 or write 2 ...)
     * which is a lot of informafion for many people ...
     *
     *  @param  i_starttime
     * a click time. 
     *
     *  @param  i_endtime
     * a click time.
     * Side note: per convention the convert use a starttime and a endtime to 
     * do some calculation to interpret this as the start of the log entry
     * and end of the log entra. IT calcs the diffrence as the duration.
     * And it try to calc the real times with a linear approch from the 
     * time points init and last and the values for start and end assuming
     * they were taken with the same click counter - i.e. it claculates
     * an interpretes real time assuming you got the tick time 
     * and put it into start and ende...
     * So dont play games with those. They are used and if you insist you can
     * even corrupt the converter by using some nonsence data.
     *
     * @return
     * - 0 for a working log write
     * - else for an error or a suppressed one
     */
    public native int write(int eventnumber,
			    int eventflag,
			    int userflag,
			    long starttime,
			    long endtime,
			    String payload);

    /** 
     * We write an entry, one buffer as additional payload.
     * the string is transfered with the number of chars in length
     *
     * We have to do something we cannot solve with a payload of
     * only the userflag and the event alone. So we add an info.
     * Its a buffer of memory, and we can put inside whatever we want.
     * It is defined by the pointer and the size only.
     * You can embed whatever you want. Its transfered binary, so
     * you can use embedded zeros, unicode chars, structs ...
     * The only problem is that binary data will corrupt the converter.
     * But not the reader. So its up to you to use a self taylored 
     * converter if binary data is in place.
     * If you ONLY use printable char data you can live with the 
     * standard converter.
     * But its up to you if you need more - then take the converter code
     * and hack it to your needs.
     * Its only up to you.
     *
     *  @param  i_eventnumber
     * A 32 bit event number. This should not be overused, check for 
     * the event flag limit. You should hold this stable, meaning
     * it should identify the spot of logging.
     * So you can swith it on or off via events. And it identifies 
     * the spot where you log.
     *
     *  @param  i_eventflag
     * The flag to identify the kind of logging.
     * Its a well known fact that we use normally an interval. So the 
     * flag is indead a char and it is then a big I ....
     * If you need a instant info - like an additional info or so - you 
     * normally use no interval. This is then a point in time entry, so I use a
     * big P for this.
     * Anything else is left for the user. But you are limited to a char, and 
     * it is a BAD THING t switch to unprintable values ....
     * Update: a small i and a small p is used in the Java jni layer.
     * So dont use them. The converter knows little about it,
     * but it is at least aware of it and trys its best.
     *
     *  @param  i_userflag
     * This is a 32 integer and its totally for the user.
     * So you can put inside as an info what you think is so important 
     * that it must come out.
     * Because it is the ONLY user info that is free of limitations
     * beside the size use it wisely - I suggest you use it together with
     * the event to make some kind of combined has map key and then 
     * use it to give you rough 10000 * 4 billion possible strings as 
     * real info 
     * which is a lot of informafion for many people ...
     * And because you use write1 there is more info in the additional payload.
     * 
     *  @param  i_starttime
     *  a click time. 
     *
     *  @param  i_endtime
     *  a click time.
     * Side note: per convention the convert use a starttime and a endtime to 
     * do some calculation to interpret this as the start of the log entry
     * and end of the log entra. IT calcs the diffrence as the duration.
     * And it try to calc the real times with a linear approch from the 
     * time points init and last and the values for start and end assuming
     * they were taken with the same click counter - i.e. it claculates
     * an interpretes real time assuming you got the tick time 
     * and put it into start and ende...
     * So dont play games with those. They are used and if you insist you can
     * even corrupt the converter by using some nonsence data.
     *
     *  @param  i_local
     * Points to the data that will be embedded as additional payload 
     * by memcpy it.
     *
     *  @param  i_size
     * The size of the embedded payload starting at i_local interpreted as
     * an adress in memory. size 0 is allowed.
     *
     * @return
     * - 0 for a working log write
     * - else for an error or a suppressed one
     */
    public native int write(int eventnumber,
			    int eventflag,
			    int userflag,
			    long starttime,
			    long endtime,
			    String payload,
			    int lengthLimit);

    /** 
     * We write an entry, one buffer as additional payload.
     * We use a byte array here.
     *
     * We have to do something we cannot solve with a payload of
     * only the userflag and the event alone. So we add an info.
     * Its a buffer of memory, and we can put inside whatever we want.
     * It is defined by the pointer and the size only.
     * You can embed whatever you want. Its transfered binary, so
     * you can use embedded zeros, unicode chars, structs ...
     * The only problem is that binary data will corrupt the converter.
     * But not the reader. So its up to you to use a self taylored 
     * converter if binary data is in place.
     * If you ONLY use printable char data you can live with the 
     * standard converter.
     * But its up to you if you need more - then take the converter code
     * and hack it to your needs.
     * Its only up to you.
     *
     *  @param  i_eventnumber
     * A 32 bit event number. This should not be overused, check for 
     * the event flag limit. You should hold this stable, meaning
     * it should identify the spot of logging.
     * So you can swith it on or off via events. And it identifies 
     * the spot where you log.
     *
     *  @param  i_eventflag
     * The flag to identify the kind of logging.
     * Its a well known fact that we use normally an interval. So the 
     * flag is indead a char and it is then a big I ....
     * If you need a instant info - like an additional info or so - you 
     * normally use no interval. This is then a point in time entry, so I use a
     * big P for this.
     * Anything else is left for the user. But you are limited to a char, and 
     * it is a BAD THING t switch to unprintable values ....
     * Update: a small i and a small p is used in the Java jni layer.
     * So dont use them. The converter knows little about it,
     * but it is at least aware of it and trys its best.
     *
     *  @param  i_userflag
     * This is a 32 integer and its totally for the user.
     * So you can put inside as an info what you think is so important 
     * that it must come out.
     * Because it is the ONLY user info that is free of limitations
     * beside the size use it wisely - I suggest you use it together with
     * the event to make some kind of combined has map key and then 
     * use it to give you rough 10000 * 4 billion possible strings as 
     * real info 
     * which is a lot of informafion for many people ...
     * And because you use write1 there is more info in the additional payload.
     * 
     *  @param  i_starttime
     *  a click time. 
     *
     *  @param  i_endtime
     *  a click time.
     * Side note: per convention the convert use a starttime and a endtime to 
     * do some calculation to interpret this as the start of the log entry
     * and end of the log entra. IT calcs the diffrence as the duration.
     * And it try to calc the real times with a linear approch from the 
     * time points init and last and the values for start and end assuming
     * they were taken with the same click counter - i.e. it claculates
     * an interpretes real time assuming you got the tick time 
     * and put it into start and ende...
     * So dont play games with those. They are used and if you insist you can
     * even corrupt the converter by using some nonsence data.
     *
     *  @param  i_local
     * Points to the data that will be embedded as additional payload 
     * by memcpy it.
     *
     *  @param  i_size
     * The size of the embedded payload starting at i_local interpreted as
     * an adress in memory. size 0 is allowed.
     *
     * @return
     * - 0 for a working log write
     * - else for an error or a suppressed one
     */
    public native int write(int eventnumber,
			    int eventflag,
			    int userflag,
			    long starttime,
			    long endtime,
			    byte[] payload);


    /**
     * We deliver the max index of the statistics buffer
     *
     * @return 
     * Maximum index of the statistics buffer.
     */
    public native int getStatisticsMaxIndex();

    /**
     * We deliver the statistics counter.
     *
     * Its up to you to have enough space for them.
     * So use the get max index to do the right allocation.
     *
     * @param o_target
     * The start adress of an int32 array for the counters.
     *
     * @return 
     * - 0 for ok
     * - not zero for error
     */
    public native int getStatistics(int[] o_target);

    /**
     * We wait nanoseconds
     *
     * @param i_nanos
     * The nanosecs till 2^31
     *
     * @return
     * void
     */
    public native void sleepNanos(int i_nanos);
  
    /** 
     * We set the prefix for name lookup in the program. 
     *
     * @param i_prefix
     * Start of the new prefix.
     *
     * @return
     * void
     */
    public native  void setEnvPrefix (String i_prefix);

    /** 
     * We get the actual name of the environment variable prefix.
     *
     * @return 
     * Points to the used prefix.
     */  
    public native String getEnvPrefix();


    /** 
     * We get the env variable value of prefix and the supported suffix.
     *
     * @param i_suffix
     * Points to the suffix after the prefix for the variable name.
     *
     * @return
     * - Points to the value of the variable.
     * - Zero if no variable defined.
     */
    public native String getEnv(String i_suffix); 


    /** 
     * We get the value of the environment variable holding the shmid in text.
     *
     * @return
     * - Points to the shmid variable value
     * - Zero if no variable defined.
     */  
    public native String getEnvShmid();

    /** 
     * We get the suffix for the shmid environment variable.
     *
     * @return
     * Points to the c string.
     */
    public native String getEnvIdSuffix();
  
    /** 
     * The version of the buffer system .
     *
     * If we change anything of the buffer layout we have to change this too.
     * Its for layout compatible processing of buffers. 
     * From the internal shm buffer tot eh readers to the converters.
     * Any change you need is a new layout. Even a simple additional number.
     * 
     * @return
     * Number of the major version of the shmlog.
     */   
    public native int getVersion();

    /**
     * The functionality of the version of the log.
     *
     * We can have different versions here if only the interface
     * is up compatible and the buffer layout is the same.
     * For example on can have additional fences. Or additional
     * sleeps in processing.
     * 
     * @return
     * Number of the minor version of the shmlog.
     */  
    public native int getMinorVersion();


    /**
     *  \brief This is the patch level, it inform of important changes in functionality 
     * for removing errors from the log.
     *
     * So no additional functionality, no new fences or new waits.
     * But some bugfix...
     *
     * @return
     * Number of the patch version of the shmlog.
     */  
    public native int getPatchVersion();


    /** 
     * Get the event flag maximum index number
     *
     * @return
     * Number of the max event + 1 
     */
    public native int getEventLocksMax();


    /** 
     * Set the event flags maximum index number.
     *
     * This can be done to make the buffer bigger.
     * We normally live with a fixed static buffer and 10000 events.
     * In case of instrumentalization it can be too small.
     * Then we have to set it before the first use - best before 
     * attach if any happens.
     * Or simply change the defien and recompile.
     *
     * @param i_max
     * Number of the new maximum event plus 1.
     *
     * @return
     * Number of the old max event + 1 
     */
    public native int setEventLocksMax(int i_max);

    /**
     * Deliver the value of the event  - its normally 0 or 1.
     *
     * 0 mean to ignore the things based on this event.
     * 1 mean to do the things...
     * All others are normally errors ... 
     * Be sure not to use more than max for the event number.
     *
     * @param i_index
     * Number of the event that is set to 1.
     *
     * @return
     * void
     */
    public native int getEvent(int i_index);

    /**
     * Helper to set an event from the program.
     *
     * @param i_index
     * Number of the event that is set to 1.
     *
     * @return
     * Old event
     */  
    public native int setEventOn(int i_index);


    /**
     * Helper to clear an event from the program.
     *
     * @param i_index
     * Number of the event that is set to 0.
     *
     * @return
     * old event
     */
    public native int setEventOff(int i_index);


    /**
     * Deliver logging state for the program.
     *
     * @return
     * Number of the logging flag for the process. 
     * Zero is logging.
     * not zero is not logging.
     */
    public native int  getLogging();
  
    /**
     * Switch the logging for the process on .
     *
     * @return
     * The old number of the logging flag.
     */  
    public native int  setLoggingProcessOn();

    /**
     * Switch the logging for the process off.
     *
     * @return
     * The old number of the logging flag.
     */
    public native int setLoggingProcessOff();

    /**
     * Switch the logging for the process off finally.
     *
     * @return
     * Old flag.
     */
    public native int setLoggingProcessOffFinal();

    /**
     * Deliver the shm id of the process.
     *
     * This is the variable shmid.
     *
     * @return
     * Number of the shared memory id.
     */  
    public native int getShmid();

    /**
     * The area buffer in shm start adress.
     *
     * @return
     * Points to the shared memory area.
     */
    public native long getArea();

    /**
     * The value of the flag in shm.
     *
     * @param i_area
     * Points to the start of the shared memory area.
     *
     * @return
     * The number of the flag. Zero means we are running, Not zero we are 
     * no longer running the log.
     */
    public native int getAreaIchHabeFertig(long i_area);

    /**
     * The value of the flag of the shm area
     * and set a new value.
     *
     * @param i_area
     * Points to the start of the shared memory area.
     *
     * @param i_flag
     * The new flag , 0 means go on, not 0 means stop
     *
     * @return
     * The number of buffers in the area.
     */
    public native int setAreaIchHabeFertig(long i_area, int i_flag);

    /**
     * The count of buffers in the shm log area.
     *
     * @param i_area
     * Points to the start of the shared memory area.
     *
     * @return
     * The number of the version in the shared memory area.
     */
    public native int getAreaCount(long i_area);

    /**
     * The version of the log area in shm.
     *
     * @param i_area
     * Points to the start of the shared memory area.
     *
     * @return
     * The number of the version in the shared memory area.
     */
    public native int getAreaVersion(long i_area);

    /**
     * The maximum size of log buffer.
     *
     * @return
     * The number of the maximum size of log buffers.
     */
    public native int getBufferMaxSize();

    /**
     * We get the actual set info size for a log buffer.
     *
     * @return
     * Number of the actual size for log buffers.
     */
    public native int getBufferSize();
    
    /**
     * We get the old size and set the new size if between 0 and 
     * logbuffer_size.
     *
     * @param i_size
     * The new number f bytes for log buffers.
     *
     * @return
     * The old number of bytes for log buffers.
     */
    public native int setBufferSize(int i_size);


    /**
     * Get the count of slave threads that the logging use.
     *
     * @return
     * The number of threads running the function for slave proc.
     */ 
    public native int getFListBufferSlaveCount();


    /**
     * Set the number of logging slave threads.
     *
     * This has to be before we start the threads. 
     * This means you have to do it before attach.
     *
     * @param i_count
     * The new count of slave threads to start.
     *
     * @return
     * The old number of slave threads to start.
     */
    public native int setFListBufferSlaveCount(int i_count);

    /**
     * Get the used clock id for the timing info from real time clock.
     *
     * @return
     * The id of the clock to use in get clock call.
     */
    public native int getClockId();

    /**
     * Set the clock id for the gettime call.
     *
     * @param i_id
     * The id for the get clock call.
     *
     * @return
     * The old id for the get clock call.
     */
    public native int setClockId(int i_id);

    /**
     * Shut off the slave threads that are not working right now.
     *
     * This will not terminate threads.
     * It only sets the slave exit to on, so a slave from its 
     * wake up directly exits, does not do any more work.
     * Actually running slaves are not affected, they do their work
     * for the actual buffer and then terminate.
     *
     * @return
     * void
     */
    public native  void setFListBufferSlaveRunOff();

    /**
     * This sets the flag to wait for slaves count down to 0
     * in the atexit.
     *
     * @return
     * The old flag.
     */
    public native int setWaitForSlavesOn();


    /**
     * This switches the waiting flag off in the atexit.
     *
     * @return
     * The old flag.
     */
    public native int setWaitForSlavesOff();

    /**
     * This is the flag value for waiting in atexit for slaves down.
     *
     * @return
     * The flag for the wait for slaves.
     */
    public native int getWaitForSlaves();

    /**
     * Get the wait time in nanoseconds for the slave .
     *
     * @return
     * The number of nanos the slave sleeps when nothing has to be done.
     */
    public native int getFListBufferSlaveWait();

    /**
     * Set the wait time for the slaves in nanoseconds.
     *
     * @param i_wait_nanos
     * The number of nanos the slave has to sleep when nothing has
     * to be done.
     *
     * @return
     * The old number of nanos the slave had to wait.
     */
    public native int setFListBufferSlaveWait(int i_wait_nanos);

    /** 
     * Count of buffers ready for acquire.
     *
     * @return
     * The number of buffers for logging in the threads.
     */
    public native int getAcquireCount();

    /**
     * Get count of preallocate buffers in one alloc.
     *
     * @return
     * The number of buffers a dyn alloc count gets in one malloc
     * from the dyn memory area.
     */
    public native int getPreallocBufferCount();

    /**
     * Set count of buffers in prealloc in alloc.
     *
     * @param i_count
     * The new number of buffers the alloc fetches from dyn memory
     * area in one malloc.
     *
     * @return
     * The old number of buffers fetched in one malloc.
     */
    public native int setPreallocBufferCount(int i_count);

    /**
     * Initial time for the module .
     *
     * @param o_target
     * The internal time struct with the inittime of the process.
     * this is here an array of at least two long values.
     *
     * @return 
     * - 0 for ok
     * - non zero for error
     */
    public native int getInittime(long[] o_target);

    /**
     * Initial click before get inittime.
     *
     * @return
     * The time for the click before the get clock call to fill inittime.
     */
    public native long getInittimeTscBefore();

    /**
     * Initial click after get initttime.
     *
     * @return
     * The time for the click after the get clock call to fill inittime.
     */
    public native long getInittimeTscAfter();

    /**
     * Get highest id for buffers.
     *
     * @return
     * get the id of the last buffer initialized.
     */
    public native int getBufferId();

    /**
     * We stop logging for the calling thread.
     *
     * @return
     * void
     */
    public native  void stop();

    /**
     * We flush the buffers for the calling thread.
     *
     * @return
     * void
     */
    public native  void flush();

    /**
     * Set the strategy for this thread.
     *
     * @param i_strategy
     * - 0: discard
     * - 1: spin loop
     * - 2: wait
     * - 3: adaptive
     * - 4: adaptive fast
     *
     * @return
     * The old strategy value 
     */
    public native int setStrategy(int i_strategy);

    /** 
     *  We get the strategy for this thread
     *
     * @return
     * The value 
     */
    public native int getStrategy();

    /**
     * Set the strategy for the process
     *
     * @param i_strategy
     * - 0: discard
     * - 1: spin loop
     * - 2: wait
     * - 3: adaptive
     * - 4: adaptive fast
     *
     * @return
     * The old strategy value 
     */
    public native int setStrategyProcess(int i_strategy);

    /** 
     *  We get the strategy for the process
     *
     * @return
     * The value 
     */
    public native int getStrategyProcess();

    /**
     * We can start a slave thread with it.
     *
     * @return
     * The return code of the used function to start the thread.
     */
    public native int createSlave();

    /** 
     * We decrement the slave count.
     * 
     * we deliver the old one and decrement. till 0.
     * this is for the case you kill slaves.
     *
     * @return
     * old count
     */
    public native int decrementSlaveCount();
    
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence1(int i_switch);

    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence1();
  
  
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence2(int i_switch);
  
    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence2();
  
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence3(int i_switch);
  
    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence3();
  
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence4(int i_switch);
  
    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence4();
  
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence5(int i_switch);
  
    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence5();
  
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence6(int i_switch);

    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence6();
  
  
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence7(int i_switch);

    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence7();
  
    
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence8(int i_switch);
  
    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence8();
  
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence9(int i_switch);
  
    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence9();
  
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence10(int i_switch);

    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence10();
  
  
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence11(int i_switch);

    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence11();
  
  
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence12(int i_switch);

    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence12();
  
    /**
     * We switch a fence on or off
     *
     * @return
     * The old flag for fence.
     */
    public native int setThreadFence13(int i_switch);

    /**
     *  We get a fence on or off flag
     *
     * @return
     * The old flag for fence.
     */
    public native int getThreadFence13();
  

    /**
     * WE get the real time
     *
     * @param o_target
     * The real time in our format
     * this uses a long array of at least 2 values.
     *
     * @return      
     * - 0 for ok
     * - non zero for error
     */
    public native int getRealtime(long[] o_target);

    /**
     * We get the thread locals adress of a thread
     *
     * @return
     * the adress of the thread locals
     */
    public native long getThreadLocalsAdress();

    /**
     * \brief We get the thread tid 
     *
     * \return 
     * The thread tid
     */
    public native long getTid();

    /**
     * We switch the thread off and dispatch its buffers
     *
     * @param i_thread_locals
     * The adress of the thread locals
     *
     */
    public native void turnLoggingOff(long i_thread_locals);

    /**
     * we turn the init in advance on
     *
     * @return
     * The old flag
     */
    public native int setInitBuffersInAdvanceOn();
  
    /**
     *  we turn the init in advance off
     *
     * @return
     * The old flag
     */
    public native int setInitBuffersInAdvanceOff();

    /**
     *  we get the init in advance flag
     *
     * @return
     * The flag
     */
    public native int getInitBuffersInAdvance();

    /** 
     *  We get the next element on slave list
     *
     * If we deliver a 0 we start from top.
     * 
     * @param i_thread_local
     * Pointer to thread local or NULL
     *
     * @return
     * - top if parameter NULL
     * - next if parameter is thread local of slave
     */
    public native long getNextSlaveLocal(long i_thread_local);

    /** 
     *  We get the tid of a thread local
     *
     * @param i_thread_local
     * Pointer to a thread local or NULL
     *
     * @return
     * - 0 if pointer is NULL
     * - tid
     */
    public native long getThreadLocalTid (long i_thread_local);
  
    /** 
     *  We remove the save from the list of slaves
     *
     * @param i_thread_local
     * Pointer to thread local or NULL
     *
     * @return
     * - 0 ok
     * - 1 list was empty
     * - non zero error
     */
    public native int removeSlaveViaLocal(long i_thread_local);

    /** 
     *  We make reuse of buffers of a dead thread
     *
     * @param i_tid
     * Our tid for the thread
     *
     * @return
     * The number of found buffers
     */
    public native int reuseThreadBuffers(long i_tid);

    /** 
     * We verify the buffer is inited and structural ok .
     *
     * @return
     * - Zero if ok.
     * - Positive for minor error.
     * - Negative for major error.
     */
    public native int verify();

    /**
     * We get the click time from here.
     *
     * This is used in later versions, for now its a start.
     * We adress here the possibility to use code for 
     * similar architecture on diffrent systems.
     * So we need to choose a low level function by id.
     *
     * @return
     * The 64 bit  click time 
     */
    public native long  getClicktime();


    /** 
     * We read a buffer and write it to a local memory area .
     *
     * We do all checks too.
     *
     * We use the lock algorithm via state for this, not the f list in shm.
     *
     * This is an obsolet function, but if you insist to transfer buffers
     * to the shm after stop logging and you dont want to play games
     * with the a and f list in shm you can pull them off with this one.
     * It was the thing used before the s and a list came to the shm.
     *
     * @param i_area
     * Points to an shm area
     *
     *  @param i_index_buffer
     * The index of the buffer to check.
     *
     *  @param o_target
     * Points to the start of a buffer big enough to hold the shm buffer
     * if any is present.
     *
     *  @param o_target_length
     * Points to an int32 that holds the length of the shm buffer 
     * after we fetched one. A 0 indicates that we didnt get one.
     *
     *  @param  o_pid
     * Points to a pid that is set to the pid from shm buffer if
     * we get a real buffer.
     *
     *  @param  o_tid
     * Points to a tid that holds the tid for the shm buffer if we really
     * got a buffer.
     *
     *  @param  o_inittime
     * Points to a time struct that holds the inittime from the process
     * that has written the shm buffer.
     *
     *  @param  o_inittimetsc_before
     * Points to a click time that was fetched before we got the inittime.
     * With the after click we have a so called point in time triple.
     *
     *  @param  o_inittimetsc_after
     * Points to a click time that was fetched after we got the inittime. 
     * With the before click we have a so called point in time triple.
     *
     *  @param  o_lasttime
     * Points to a time struct that holds the final time that we got before
     * the buffer was placed into the shm.
     *
     *  @param  o_lasttimetsc_before
     * Points to a click time that was fetched before we got the lasttime.
     * With the after click we have a so called point in time triple.
     *
     *  @param  o_lasttimetsc_after
     * Points to a click time that was fetched after we got the lasttime.
     * With the before click we have a so called point in time triple.
     *
     *  @param  o_difftimetransfer
     * Points to a click time that holds the difference between the
     * start of the transfer and the end, mainly the memcpy time
     * for the buffer.
     *
     *  @param  o_starttransfer
     * Points to a click time that holds the start of the transfer from
     * the client thread to the shm. This is used with the lasttime before 
     * to calculate the transfer time in the client for the buffer into
     * the shm buffer.
     *
     *  @param  o_acquiretime
     * Points to a click time that holds the difference from the 
     * acquire call of the thread to get the buffer from the a list.
     * This can be the simple pop time or in worst cse the dyn memory
     * and initialization time for the pre allocated buffers.
     *
     *  @param  o_id
     * Points to an int32 that holds the buffer id from the 
     * transfered client buffer which was given in the initialization
     * for the buffer.
     *
     * @return
     * - Negative if an error occured.
     * - Zero if a buffer could be fetched.
     * - Positiv if the call didnt fetch a buffer. In this case the 
     *   buffer length is 0 and all other values are meaningless.
     */
    public native int read(long i_area,
			   int i_index_buffer,
			   byte[] o_target,
			   long[] o_params
			   /* o_target_length,  index 0
			      o_pid, index 1
			      o_tid, index 2
			      o_inittime, nsec index 3
                              o_inittime, sec index 4
			      o_inittimetsc_before, index 5
			      o_inittimetsc_after, index 6
			      o_lasttime, nsec index 7 
			      o_lasttime, sec index 8
			      o_lasttimetsc_before, index 9
			      o_lasttimetsc_after, index 10
			      o_difftimetransfer, index 11
			      o_starttransfer, index 12
			      o_acquiretime, index 13
			      o_id index 14
and the number of the dispatch and 18 statistics
			   */);

    /** 
     * We read a buffer and write it to a local memory area. 
     *
     * We do all checks too.
     *
     * We get it from the shmf list. We dont wait. If there is no
     * buffer we return quite.
     * 
     * @param i_area
     * Points to an shm area
     *
     *  @param o_index_buffer
     * Points to an int32 to hold the index of the shm buffer if we succeed
     * and really get a buffer.
     *
     *  @param o_target
     * Points to the start of a buffer big enough to hold the shm buffer
     * if any is present.
     *
     *  @param o_target_length
     * Points to an int32 that holds the length of the shm buffer 
     * after we fetched one. A 0 indicates that we didnt get one.
     *
     *  @param  o_pid
     * Points to a pid that is set to the pid from shm buffer if
     * we get a real buffer.
     *
     *  @param  o_tid
     * Points to a tid that holds the tid for the shm buffer if we really
     * got a buffer.
     *
     *  @param  o_inittime
     * Points to a time struct that holds the inittime from the process
     * that has written the shm buffer.
     *
     *  @param  o_inittimetsc_before
     * Points to a click time that was fetched before we got the inittime.
     * With the after click we have a so called point in time triple.
     *
     *  @param  o_inittimetsc_after
     * Points to a click time that was fetched after we got the inittime. 
     * With the before click we have a so called point in time triple.
     *
     *  @param  o_lasttime
     * Points to a time struct that holds the final time that we got before
     * the buffer was placed into the shm.
     *
     *  @param  o_lasttimetsc_before
     * Points to a click time that was fetched before we got the lasttime.
     * With the after click we have a so called point in time triple.
     *
     *  @param  o_lasttimetsc_after
     * Points to a click time that was fetched after we got the lasttime.
     * With the before click we have a so called point in time triple.
     *
     *  @param  o_difftimetransfer
     * Points to a click time that holds the difference between the
     * start of the transfer and the end, mainly the memcpy time
     * for the buffer.
     *
     *  @param  o_starttransfer
     * Points to a click time that holds the start of the transfer from
     * the client thread to the shm. This is used with the lasttime before 
     * to calculate the transfer time in the client for the buffer into
     * the shm buffer.
     *
     *  @param  o_acquiretime
     * Points to a click time that holds the difference from the 
     * acquire call of the thread to get the buffer from the a list.
     * This can be the simple pop time or in worst cse the dyn memory
     * and initialization time for the pre allocated buffers.
     *
     *  @param  o_id
     * Points to an int32 that holds the buffer id from the 
     * transfered client buffer which was given in the initialization
     * for the buffer.
     *
     * @return
     * - Negative if an error occured.
     * - Zero if a buffer could be fetched.
     * - Positiv if the call didnt fetch a buffer. In this case the 
     *   buffer length is 0 and all other values are meaningless.
     */
    public native int readFetch(long i_area,
				byte[] o_target,
				long[] o_params
				/* 
				   o_index_buffer, index 0
				   o_target_length, index 1
				   o_pid, index 2
				   o_tid, index 3
				   o_inittime, nsec index 4
				   o_inittime, sec index 5
				   o_inittimetsc_before, index 6
				   o_inittimetsc_after, index 7
				   o_lasttime, nsec index 8 
				   o_lasttime, sec index 9 
				   o_lasttimetsc_before, index 10 
				   o_lasttimetsc_after, index 11
				   o_difftimetransfer, index 12
				   o_starttransfer, index 13 
				   o_acquiretime, index 14
				   o_id index 15
and the number of the dispatch and 18 statistics
				*/);

    /** 
     * We create the shm  area.
     *
     * @param i_key
     * The shmkey of the system.
     *
     * @param i_count
     * The number of buffers for this area.
     *
     * @return
     * - the shmid for success ( value greater 0 )
     * - negative for error
     */
    public native int create(int i_key,
			     int i_count);

    /** 
     * We destroy the shm 
     *
     * @param i_shmid
     * The shm id of the system
     *
     * @return
     * The error code of the delete
     */
    public native int delete( int i_shmid);

    /** 
     * We have to clean up the locks after error condition 
     *
     * @param i_area
     * Points to the start of the area.
     *
     * @return
     * void
     */
    public native  void cleanupLocks(long i_area);

    /** 
     * We initialize the buffer after it is attached 
     *
     * @param i_area
     * Points to the start of the area.
     *
     * @param i_count_buffers
     * The number of buffers for the shm are.
     */
    public native int initShmLog(long i_area,
				 int i_count_buffers);



    /**
     * We can fetch a byte from shared memory
     * 
     * This is only for jni needed helper.
     *
     * @param i_area
     * The long holdin the area adress
     *
     * @param i_offset
     * the offset in the area
     *
     * @return 
     * The byte value at the location in the area
     *
     */
    public native int peek(long i_area,
			    int i_offset);

    /**
     * We can store a byte in shared memory
     * 
     * This is only for jni needed helper.
     *
     * @param i_area
     * The long holdin the area adress
     *
     * @param i_offset
     * the offset in the area
     *
     * @param i_value
     * the new value 
     *
     * @return 
     * The old byte value at the location in the area 
     *
     */
    public native int poke(long i_area,
			    int i_offset,
			    byte i_value);

    
}

/* end of file */
