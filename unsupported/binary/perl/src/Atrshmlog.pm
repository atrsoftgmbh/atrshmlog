# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

package Atrshmlog;
use base qw(Exporter);
use base qw(DynaLoader);
package Atrshmlogc;
bootstrap Atrshmlog;
package Atrshmlog;
@EXPORT = qw();

# ---------- BASE METHODS -------------

package Atrshmlog;

sub TIEHASH {
    my ($classname,$obj) = @_;
    return bless $obj, $classname;
}

sub CLEAR { }

sub FIRSTKEY { }

sub NEXTKEY { }

sub FETCH {
    my ($self,$field) = @_;
    my $member_func = "swig_${field}_get";
    $self->$member_func();
}

sub STORE {
    my ($self,$field,$newval) = @_;
    my $member_func = "swig_${field}_set";
    $self->$member_func($newval);
}

sub this {
    my $ptr = shift;
    return tied(%$ptr);
}


# ------- FUNCTION WRAPPERS --------

package Atrshmlog;

*attach = *Atrshmlogc::attach;
*get_statistics_max_index = *Atrshmlogc::get_statistics_max_index;
*write0 = *Atrshmlogc::write0;
*sleep_nanos = *Atrshmlogc::sleep_nanos;
*set_env_prefix = *Atrshmlogc::set_env_prefix;
*get_env_prefix = *Atrshmlogc::get_env_prefix;
*get_env = *Atrshmlogc::get_env;
*get_env_shmid = *Atrshmlogc::get_env_shmid;
*get_env_id_suffix = *Atrshmlogc::get_env_id_suffix;
*get_version = *Atrshmlogc::get_version;
*get_minor_version = *Atrshmlogc::get_minor_version;
*get_patch_version = *Atrshmlogc::get_patch_version;
*get_event_locks_max = *Atrshmlogc::get_event_locks_max;
*set_event_locks_max = *Atrshmlogc::set_event_locks_max;
*get_event = *Atrshmlogc::get_event;
*set_event_on = *Atrshmlogc::set_event_on;
*set_event_off = *Atrshmlogc::set_event_off;
*get_logging = *Atrshmlogc::get_logging;
*set_logging_process_on = *Atrshmlogc::set_logging_process_on;
*set_logging_process_off = *Atrshmlogc::set_logging_process_off;
*set_logging_process_off_final = *Atrshmlogc::set_logging_process_off_final;
*get_shmid = *Atrshmlogc::get_shmid;
*get_area = *Atrshmlogc::get_area;
*get_area_ich_habe_fertig = *Atrshmlogc::get_area_ich_habe_fertig;
*set_area_ich_habe_fertig = *Atrshmlogc::set_area_ich_habe_fertig;
*get_area_count = *Atrshmlogc::get_area_count;
*get_area_version = *Atrshmlogc::get_area_version;
*get_buffer_max_size = *Atrshmlogc::get_buffer_max_size;
*get_buffer_size = *Atrshmlogc::get_buffer_size;
*set_buffer_size = *Atrshmlogc::set_buffer_size;
*get_f_list_buffer_slave_count = *Atrshmlogc::get_f_list_buffer_slave_count;
*set_f_list_buffer_slave_count = *Atrshmlogc::set_f_list_buffer_slave_count;
*get_clock_id = *Atrshmlogc::get_clock_id;
*set_clock_id = *Atrshmlogc::set_clock_id;
*set_f_list_buffer_slave_run_off = *Atrshmlogc::set_f_list_buffer_slave_run_off;
*set_wait_for_slaves_on = *Atrshmlogc::set_wait_for_slaves_on;
*set_wait_for_slaves_off = *Atrshmlogc::set_wait_for_slaves_off;
*get_wait_for_slaves = *Atrshmlogc::get_wait_for_slaves;
*get_f_list_buffer_slave_wait = *Atrshmlogc::get_f_list_buffer_slave_wait;
*set_f_list_buffer_slave_wait = *Atrshmlogc::set_f_list_buffer_slave_wait;
*get_acquire_count = *Atrshmlogc::get_acquire_count;
*get_prealloc_buffer_count = *Atrshmlogc::get_prealloc_buffer_count;
*set_prealloc_buffer_count = *Atrshmlogc::set_prealloc_buffer_count;
*get_inittime_tsc_before = *Atrshmlogc::get_inittime_tsc_before;
*get_inittime_tsc_after = *Atrshmlogc::get_inittime_tsc_after;
*get_buffer_id = *Atrshmlogc::get_buffer_id;
*stop = *Atrshmlogc::stop;
*flush = *Atrshmlogc::flush;
*set_strategy = *Atrshmlogc::set_strategy;
*get_strategy = *Atrshmlogc::get_strategy;
*set_strategy_process = *Atrshmlogc::set_strategy_process;
*get_strategy_process = *Atrshmlogc::get_strategy_process;
*create_slave = *Atrshmlogc::create_slave;
*decrement_slave_count = *Atrshmlogc::decrement_slave_count;
*set_thread_fence_1 = *Atrshmlogc::set_thread_fence_1;
*get_thread_fence_1 = *Atrshmlogc::get_thread_fence_1;
*set_thread_fence_2 = *Atrshmlogc::set_thread_fence_2;
*get_thread_fence_2 = *Atrshmlogc::get_thread_fence_2;
*set_thread_fence_3 = *Atrshmlogc::set_thread_fence_3;
*get_thread_fence_3 = *Atrshmlogc::get_thread_fence_3;
*set_thread_fence_4 = *Atrshmlogc::set_thread_fence_4;
*get_thread_fence_4 = *Atrshmlogc::get_thread_fence_4;
*set_thread_fence_5 = *Atrshmlogc::set_thread_fence_5;
*get_thread_fence_5 = *Atrshmlogc::get_thread_fence_5;
*set_thread_fence_6 = *Atrshmlogc::set_thread_fence_6;
*get_thread_fence_6 = *Atrshmlogc::get_thread_fence_6;
*set_thread_fence_7 = *Atrshmlogc::set_thread_fence_7;
*get_thread_fence_7 = *Atrshmlogc::get_thread_fence_7;
*set_thread_fence_8 = *Atrshmlogc::set_thread_fence_8;
*get_thread_fence_8 = *Atrshmlogc::get_thread_fence_8;
*set_thread_fence_9 = *Atrshmlogc::set_thread_fence_9;
*get_thread_fence_9 = *Atrshmlogc::get_thread_fence_9;
*set_thread_fence_10 = *Atrshmlogc::set_thread_fence_10;
*get_thread_fence_10 = *Atrshmlogc::get_thread_fence_10;
*set_thread_fence_11 = *Atrshmlogc::set_thread_fence_11;
*get_thread_fence_11 = *Atrshmlogc::get_thread_fence_11;
*set_thread_fence_12 = *Atrshmlogc::set_thread_fence_12;
*get_thread_fence_12 = *Atrshmlogc::get_thread_fence_12;
*set_thread_fence_13 = *Atrshmlogc::set_thread_fence_13;
*get_thread_fence_13 = *Atrshmlogc::get_thread_fence_13;
*get_thread_locals_adress = *Atrshmlogc::get_thread_locals_adress;
*get_tid = *Atrshmlogc::get_tid;
*turn_logging_off = *Atrshmlogc::turn_logging_off;
*set_init_buffers_in_advance_on = *Atrshmlogc::set_init_buffers_in_advance_on;
*set_init_buffers_in_advance_off = *Atrshmlogc::set_init_buffers_in_advance_off;
*get_init_buffers_in_advance = *Atrshmlogc::get_init_buffers_in_advance;
*get_next_slave_local = *Atrshmlogc::get_next_slave_local;
*get_thread_local_tid = *Atrshmlogc::get_thread_local_tid;
*remove_slave_via_local = *Atrshmlogc::remove_slave_via_local;
*reuse_thread_buffers = *Atrshmlogc::reuse_thread_buffers;
*verify = *Atrshmlogc::verify;
*atrshmlog_get_tsc_par_x86_64_gnu = *Atrshmlogc::atrshmlog_get_tsc_par_x86_64_gnu;
*atrshmlog_get_tsc_fence_x86_64_gnu = *Atrshmlogc::atrshmlog_get_tsc_fence_x86_64_gnu;
*atrshmlog_get_tsc_x86_64_gnu = *Atrshmlogc::atrshmlog_get_tsc_x86_64_gnu;
*atrshmlog_get_tsc_null_x86_64_gnu = *Atrshmlogc::atrshmlog_get_tsc_null_x86_64_gnu;
*get_clicktime = *Atrshmlogc::get_clicktime;
*create = *Atrshmlogc::create;
*delete = *Atrshmlogc::delete;
*cleanup_locks = *Atrshmlogc::cleanup_locks;
*init_shm_log = *Atrshmlogc::init_shm_log;
*gettime = *Atrshmlogc::gettime;
*write_binary = *Atrshmlogc::write_binary;
*write = *Atrshmlogc::write;
*get_inittime = *Atrshmlogc::get_inittime;
*get_realtime = *Atrshmlogc::get_realtime;
*get_statistics = *Atrshmlogc::get_statistics;
*read = *Atrshmlogc::read;
*read_fetch = *Atrshmlogc::read_fetch;
*poke = *Atrshmlogc::poke;
*peek = *Atrshmlogc::peek;

############# Class : Atrshmlog::atrshmlog_internal_time_s ##############

package Atrshmlog::atrshmlog_internal_time_s;
use vars qw(@ISA %OWNER %ITERATORS %BLESSEDMEMBERS);
@ISA = qw( Atrshmlog );
%OWNER = ();
%ITERATORS = ();
*swig_tv_sec_get = *Atrshmlogc::atrshmlog_internal_time_s_tv_sec_get;
*swig_tv_sec_set = *Atrshmlogc::atrshmlog_internal_time_s_tv_sec_set;
*swig_tv_nsec_get = *Atrshmlogc::atrshmlog_internal_time_s_tv_nsec_get;
*swig_tv_nsec_set = *Atrshmlogc::atrshmlog_internal_time_s_tv_nsec_set;
sub new {
    my $pkg = shift;
    my $self = Atrshmlogc::new_atrshmlog_internal_time_s(@_);
    bless $self, $pkg if defined($self);
}

sub DESTROY {
    return unless $_[0]->isa('HASH');
    my $self = tied(%{$_[0]});
    return unless defined $self;
    delete $ITERATORS{$self};
    if (exists $OWNER{$self}) {
        Atrshmlogc::delete_atrshmlog_internal_time_s($self);
        delete $OWNER{$self};
    }
}

sub DISOWN {
    my $self = shift;
    my $ptr = tied(%$self);
    delete $OWNER{$ptr};
}

sub ACQUIRE {
    my $self = shift;
    my $ptr = tied(%$self);
    $OWNER{$ptr} = 1;
}


# ------- VARIABLE STUBS --------

package Atrshmlog;

*ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC = *Atrshmlogc::ATRSHMLOG_PLATFORM_LINUX_X86_64_GCC;
*ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC = *Atrshmlogc::ATRSHMLOG_PLATFORM_CYGWIN_X86_64_GCC;
*ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC = *Atrshmlogc::ATRSHMLOG_PLATFORM_MINGW_X86_64_GCC;
*ATRSHMLOG_LEVEL = *Atrshmlogc::ATRSHMLOG_LEVEL;
*ATRSHMLOG_INLINE_TSC_CODE = *Atrshmlogc::ATRSHMLOG_INLINE_TSC_CODE;
*ATRSHMLOG_INLINE_GETTIME = *Atrshmlogc::ATRSHMLOG_INLINE_GETTIME;
*ATRSHMLOG_USE_PTHREAD = *Atrshmlogc::ATRSHMLOG_USE_PTHREAD;
*ATRSHMLOG_USE_WINTHREAD = *Atrshmlogc::ATRSHMLOG_USE_WINTHREAD;
*ATRSHMLOG_USE_C11_THREAD = *Atrshmlogc::ATRSHMLOG_USE_C11_THREAD;
*ATRSHMLOG_SYSCALL = *Atrshmlogc::ATRSHMLOG_SYSCALL;
*ATRSHMLOG_USE_CLOCK_GETTIME = *Atrshmlogc::ATRSHMLOG_USE_CLOCK_GETTIME;
*ATRSHMLOG_USE_WIN_FILETIME = *Atrshmlogc::ATRSHMLOG_USE_WIN_FILETIME;
*ATRSHMLOG_USE_GETTIMEOFDAY = *Atrshmlogc::ATRSHMLOG_USE_GETTIMEOFDAY;
*ATRSHMLOG_USE_NANOSLEEP = *Atrshmlogc::ATRSHMLOG_USE_NANOSLEEP;
*ATRSHMLOG_USE_SYSCALL_TID = *Atrshmlogc::ATRSHMLOG_USE_SYSCALL_TID;
*ATRSHMLOG_USE_PTHREAD_TID = *Atrshmlogc::ATRSHMLOG_USE_PTHREAD_TID;
*ATRSHMLOG_USE_WINTHREAD_TID = *Atrshmlogc::ATRSHMLOG_USE_WINTHREAD_TID;
*EVENT_POINT_IN_TIME_C = *Atrshmlogc::EVENT_POINT_IN_TIME_C;
*EVENT_POINT_IN_TIME_UCS2 = *Atrshmlogc::EVENT_POINT_IN_TIME_UCS2;
*EVENT_INTERVAL_IN_TIME_C = *Atrshmlogc::EVENT_INTERVAL_IN_TIME_C;
*EVENT_INTERVAL_IN_TIME_UCS2 = *Atrshmlogc::EVENT_INTERVAL_IN_TIME_UCS2;
*atrshmlog_event_locks = *Atrshmlogc::atrshmlog_event_locks;
*atrshmlog_logging_process = *Atrshmlogc::atrshmlog_logging_process;
*atrshmlog_clock_id = *Atrshmlogc::atrshmlog_clock_id;
*atrshmlog_counter_start = *Atrshmlogc::atrshmlog_counter_start;
*atrshmlog_counter_time_low = *Atrshmlogc::atrshmlog_counter_time_low;
*atrshmlog_counter_time_high = *Atrshmlogc::atrshmlog_counter_time_high;
*atrshmlog_counter_attach = *Atrshmlogc::atrshmlog_counter_attach;
*atrshmlog_counter_get_raw = *Atrshmlogc::atrshmlog_counter_get_raw;
*atrshmlog_counter_free = *Atrshmlogc::atrshmlog_counter_free;
*atrshmlog_counter_alloc = *Atrshmlogc::atrshmlog_counter_alloc;
*atrshmlog_counter_dispatch = *Atrshmlogc::atrshmlog_counter_dispatch;
*atrshmlog_counter_mem_to_shm = *Atrshmlogc::atrshmlog_counter_mem_to_shm;
*atrshmlog_counter_mem_to_shm_doit = *Atrshmlogc::atrshmlog_counter_mem_to_shm_doit;
*atrshmlog_counter_mem_to_shm_full = *Atrshmlogc::atrshmlog_counter_mem_to_shm_full;
*atrshmlog_counter_create_slave = *Atrshmlogc::atrshmlog_counter_create_slave;
*atrshmlog_counter_stop = *Atrshmlogc::atrshmlog_counter_stop;
*atrshmlog_counter_write0 = *Atrshmlogc::atrshmlog_counter_write0;
*atrshmlog_counter_write0_abort1 = *Atrshmlogc::atrshmlog_counter_write0_abort1;
*atrshmlog_counter_write0_abort2 = *Atrshmlogc::atrshmlog_counter_write0_abort2;
*atrshmlog_counter_write0_abort3 = *Atrshmlogc::atrshmlog_counter_write0_abort3;
*atrshmlog_counter_write0_abort4 = *Atrshmlogc::atrshmlog_counter_write0_abort4;
*atrshmlog_counter_write0_discard = *Atrshmlogc::atrshmlog_counter_write0_discard;
*atrshmlog_counter_write0_wait = *Atrshmlogc::atrshmlog_counter_write0_wait;
*atrshmlog_counter_write0_adaptive = *Atrshmlogc::atrshmlog_counter_write0_adaptive;
*atrshmlog_counter_write0_adaptive_fast = *Atrshmlogc::atrshmlog_counter_write0_adaptive_fast;
*atrshmlog_counter_write0_adaptive_very_fast = *Atrshmlogc::atrshmlog_counter_write0_adaptive_very_fast;
*atrshmlog_counter_write_safeguard = *Atrshmlogc::atrshmlog_counter_write_safeguard;
*atrshmlog_counter_write_safeguard_shm = *Atrshmlogc::atrshmlog_counter_write_safeguard_shm;
*atrshmlog_counter_write1 = *Atrshmlogc::atrshmlog_counter_write1;
*atrshmlog_counter_write1_abort1 = *Atrshmlogc::atrshmlog_counter_write1_abort1;
*atrshmlog_counter_write1_abort2 = *Atrshmlogc::atrshmlog_counter_write1_abort2;
*atrshmlog_counter_write1_abort3 = *Atrshmlogc::atrshmlog_counter_write1_abort3;
*atrshmlog_counter_write1_abort4 = *Atrshmlogc::atrshmlog_counter_write1_abort4;
*atrshmlog_counter_write1_discard = *Atrshmlogc::atrshmlog_counter_write1_discard;
*atrshmlog_counter_write1_wait = *Atrshmlogc::atrshmlog_counter_write1_wait;
*atrshmlog_counter_write1_adaptive = *Atrshmlogc::atrshmlog_counter_write1_adaptive;
*atrshmlog_counter_write1_adaptive_fast = *Atrshmlogc::atrshmlog_counter_write1_adaptive_fast;
*atrshmlog_counter_write1_adaptive_very_fast = *Atrshmlogc::atrshmlog_counter_write1_adaptive_very_fast;
*atrshmlog_counter_write1_abort5 = *Atrshmlogc::atrshmlog_counter_write1_abort5;
*atrshmlog_counter_write1_abort6 = *Atrshmlogc::atrshmlog_counter_write1_abort6;
*atrshmlog_counter_write1_abort7 = *Atrshmlogc::atrshmlog_counter_write1_abort7;
*atrshmlog_counter_write2 = *Atrshmlogc::atrshmlog_counter_write2;
*atrshmlog_counter_write2_abort1 = *Atrshmlogc::atrshmlog_counter_write2_abort1;
*atrshmlog_counter_write2_abort2 = *Atrshmlogc::atrshmlog_counter_write2_abort2;
*atrshmlog_counter_write2_abort3 = *Atrshmlogc::atrshmlog_counter_write2_abort3;
*atrshmlog_counter_write2_abort4 = *Atrshmlogc::atrshmlog_counter_write2_abort4;
*atrshmlog_counter_write2_discard = *Atrshmlogc::atrshmlog_counter_write2_discard;
*atrshmlog_counter_write2_wait = *Atrshmlogc::atrshmlog_counter_write2_wait;
*atrshmlog_counter_write2_adaptive = *Atrshmlogc::atrshmlog_counter_write2_adaptive;
*atrshmlog_counter_write2_adaptive_fast = *Atrshmlogc::atrshmlog_counter_write2_adaptive_fast;
*atrshmlog_counter_write2_adaptive_very_fast = *Atrshmlogc::atrshmlog_counter_write2_adaptive_very_fast;
*atrshmlog_counter_write2_abort5 = *Atrshmlogc::atrshmlog_counter_write2_abort5;
*atrshmlog_counter_write2_abort6 = *Atrshmlogc::atrshmlog_counter_write2_abort6;
*atrshmlog_counter_write2_abort7 = *Atrshmlogc::atrshmlog_counter_write2_abort7;
*atrshmlog_counter_set_slave_count = *Atrshmlogc::atrshmlog_counter_set_slave_count;
*atrshmlog_counter_set_clock_id = *Atrshmlogc::atrshmlog_counter_set_clock_id;
*atrshmlog_counter_slave_off = *Atrshmlogc::atrshmlog_counter_slave_off;
*atrshmlog_counter_set_event_locks = *Atrshmlogc::atrshmlog_counter_set_event_locks;
*atrshmlog_counter_set_buffer_size = *Atrshmlogc::atrshmlog_counter_set_buffer_size;
*atrshmlog_counter_set_wait_slaves_on = *Atrshmlogc::atrshmlog_counter_set_wait_slaves_on;
*atrshmlog_counter_set_wait_slaves_off = *Atrshmlogc::atrshmlog_counter_set_wait_slaves_off;
*atrshmlog_counter_set_slave_wait = *Atrshmlogc::atrshmlog_counter_set_slave_wait;
*atrshmlog_counter_set_prealloc_count = *Atrshmlogc::atrshmlog_counter_set_prealloc_count;
*atrshmlog_counter_set_thread_fence = *Atrshmlogc::atrshmlog_counter_set_thread_fence;
*atrshmlog_counter_create = *Atrshmlogc::atrshmlog_counter_create;
*atrshmlog_counter_create_abort1 = *Atrshmlogc::atrshmlog_counter_create_abort1;
*atrshmlog_counter_create_abort2 = *Atrshmlogc::atrshmlog_counter_create_abort2;
*atrshmlog_counter_create_abort3 = *Atrshmlogc::atrshmlog_counter_create_abort3;
*atrshmlog_counter_create_abort4 = *Atrshmlogc::atrshmlog_counter_create_abort4;
*atrshmlog_counter_delete = *Atrshmlogc::atrshmlog_counter_delete;
*atrshmlog_counter_cleanup_locks = *Atrshmlogc::atrshmlog_counter_cleanup_locks;
*atrshmlog_counter_init_shm = *Atrshmlogc::atrshmlog_counter_init_shm;
*atrshmlog_counter_read = *Atrshmlogc::atrshmlog_counter_read;
*atrshmlog_counter_read_doit = *Atrshmlogc::atrshmlog_counter_read_doit;
*atrshmlog_counter_read_fetch = *Atrshmlogc::atrshmlog_counter_read_fetch;
*atrshmlog_counter_read_fetch_doit = *Atrshmlogc::atrshmlog_counter_read_fetch_doit;
*atrshmlog_counter_verify = *Atrshmlogc::atrshmlog_counter_verify;
*atrshmlog_counter_logging_process_on = *Atrshmlogc::atrshmlog_counter_logging_process_on;
*atrshmlog_counter_logging_process_off = *Atrshmlogc::atrshmlog_counter_logging_process_off;
*atrshmlog_counter_set_strategy = *Atrshmlogc::atrshmlog_counter_set_strategy;
*atrshmlog_counter_set_strategy_process = *Atrshmlogc::atrshmlog_counter_set_strategy_process;
*atrshmlog_counter_set_event = *Atrshmlogc::atrshmlog_counter_set_event;
*atrshmlog_counter_set_env_prefix = *Atrshmlogc::atrshmlog_counter_set_env_prefix;
*atrshmlog_counter_exit_cleanup = *Atrshmlogc::atrshmlog_counter_exit_cleanup;
*atrshmlog_counter_flush = *Atrshmlogc::atrshmlog_counter_flush;
*atrshmlog_counter_logging_process_off_final = *Atrshmlogc::atrshmlog_counter_logging_process_off_final;
*atrshmlog_counter_turn_logging_off = *Atrshmlogc::atrshmlog_counter_turn_logging_off;
*atrshmlog_counter_init_in_advance_on = *Atrshmlogc::atrshmlog_counter_init_in_advance_on;
*atrshmlog_counter_init_in_advance_off = *Atrshmlogc::atrshmlog_counter_init_in_advance_off;
*atrshmlog_counter_reuse_thread_buffers = *Atrshmlogc::atrshmlog_counter_reuse_thread_buffers;
*atrshmlog_counter_end = *Atrshmlogc::atrshmlog_counter_end;
*atrshmlog_error_ok = *Atrshmlogc::atrshmlog_error_ok;
*atrshmlog_error_error = *Atrshmlogc::atrshmlog_error_error;
*atrshmlog_error_error2 = *Atrshmlogc::atrshmlog_error_error2;
*atrshmlog_error_error3 = *Atrshmlogc::atrshmlog_error_error3;
*atrshmlog_error_error4 = *Atrshmlogc::atrshmlog_error_error4;
*atrshmlog_error_error5 = *Atrshmlogc::atrshmlog_error_error5;
*atrshmlog_error_connect_1 = *Atrshmlogc::atrshmlog_error_connect_1;
*atrshmlog_error_connect_2 = *Atrshmlogc::atrshmlog_error_connect_2;
*atrshmlog_error_init_thread_local_1 = *Atrshmlogc::atrshmlog_error_init_thread_local_1;
*atrshmlog_error_mem_to_shm_1 = *Atrshmlogc::atrshmlog_error_mem_to_shm_1;
*atrshmlog_error_mem_to_shm_2 = *Atrshmlogc::atrshmlog_error_mem_to_shm_2;
*atrshmlog_error_mem_to_shm_3 = *Atrshmlogc::atrshmlog_error_mem_to_shm_3;
*atrshmlog_error_mem_to_shm_4 = *Atrshmlogc::atrshmlog_error_mem_to_shm_4;
*atrshmlog_error_mem_to_shm_5 = *Atrshmlogc::atrshmlog_error_mem_to_shm_5;
*atrshmlog_error_mem_to_shm_6 = *Atrshmlogc::atrshmlog_error_mem_to_shm_6;
*atrshmlog_error_mem_to_shm_7 = *Atrshmlogc::atrshmlog_error_mem_to_shm_7;
*atrshmlog_error_mem_to_shm_8 = *Atrshmlogc::atrshmlog_error_mem_to_shm_8;
*atrshmlog_error_attach_1 = *Atrshmlogc::atrshmlog_error_attach_1;
*atrshmlog_error_attach_2 = *Atrshmlogc::atrshmlog_error_attach_2;
*atrshmlog_error_attach_3 = *Atrshmlogc::atrshmlog_error_attach_3;
*atrshmlog_error_attach_4 = *Atrshmlogc::atrshmlog_error_attach_4;
*atrshmlog_error_attach_5 = *Atrshmlogc::atrshmlog_error_attach_5;
*atrshmlog_error_attach_6 = *Atrshmlogc::atrshmlog_error_attach_6;
*atrshmlog_error_init_in_write_1 = *Atrshmlogc::atrshmlog_error_init_in_write_1;
*atrshmlog_error_write0_1 = *Atrshmlogc::atrshmlog_error_write0_1;
*atrshmlog_error_write0_2 = *Atrshmlogc::atrshmlog_error_write0_2;
*atrshmlog_error_write0_3 = *Atrshmlogc::atrshmlog_error_write0_3;
*atrshmlog_error_write0_4 = *Atrshmlogc::atrshmlog_error_write0_4;
*atrshmlog_error_write0_5 = *Atrshmlogc::atrshmlog_error_write0_5;
*atrshmlog_error_write0_6 = *Atrshmlogc::atrshmlog_error_write0_6;
*atrshmlog_error_write0_7 = *Atrshmlogc::atrshmlog_error_write0_7;
*atrshmlog_error_write0_8 = *Atrshmlogc::atrshmlog_error_write0_8;
*atrshmlog_error_write0_9 = *Atrshmlogc::atrshmlog_error_write0_9;
*atrshmlog_error_write1_1 = *Atrshmlogc::atrshmlog_error_write1_1;
*atrshmlog_error_write1_2 = *Atrshmlogc::atrshmlog_error_write1_2;
*atrshmlog_error_write1_3 = *Atrshmlogc::atrshmlog_error_write1_3;
*atrshmlog_error_write1_4 = *Atrshmlogc::atrshmlog_error_write1_4;
*atrshmlog_error_write1_5 = *Atrshmlogc::atrshmlog_error_write1_5;
*atrshmlog_error_write1_6 = *Atrshmlogc::atrshmlog_error_write1_6;
*atrshmlog_error_write1_7 = *Atrshmlogc::atrshmlog_error_write1_7;
*atrshmlog_error_write1_8 = *Atrshmlogc::atrshmlog_error_write1_8;
*atrshmlog_error_write1_9 = *Atrshmlogc::atrshmlog_error_write1_9;
*atrshmlog_error_write1_10 = *Atrshmlogc::atrshmlog_error_write1_10;
*atrshmlog_error_write1_11 = *Atrshmlogc::atrshmlog_error_write1_11;
*atrshmlog_error_write1_12 = *Atrshmlogc::atrshmlog_error_write1_12;
*atrshmlog_error_write2_1 = *Atrshmlogc::atrshmlog_error_write2_1;
*atrshmlog_error_write2_2 = *Atrshmlogc::atrshmlog_error_write2_2;
*atrshmlog_error_write2_3 = *Atrshmlogc::atrshmlog_error_write2_3;
*atrshmlog_error_write2_4 = *Atrshmlogc::atrshmlog_error_write2_4;
*atrshmlog_error_write2_5 = *Atrshmlogc::atrshmlog_error_write2_5;
*atrshmlog_error_write2_6 = *Atrshmlogc::atrshmlog_error_write2_6;
*atrshmlog_error_write2_7 = *Atrshmlogc::atrshmlog_error_write2_7;
*atrshmlog_error_write2_8 = *Atrshmlogc::atrshmlog_error_write2_8;
*atrshmlog_error_write2_9 = *Atrshmlogc::atrshmlog_error_write2_9;
*atrshmlog_error_write2_10 = *Atrshmlogc::atrshmlog_error_write2_10;
*atrshmlog_error_write2_11 = *Atrshmlogc::atrshmlog_error_write2_11;
*atrshmlog_error_write2_12 = *Atrshmlogc::atrshmlog_error_write2_12;
*atrshmlog_error_area_version_1 = *Atrshmlogc::atrshmlog_error_area_version_1;
*atrshmlog_error_area_count_1 = *Atrshmlogc::atrshmlog_error_area_count_1;
*atrshmlog_error_area_ich_habe_fertig_1 = *Atrshmlogc::atrshmlog_error_area_ich_habe_fertig_1;
*atrshmlog_error_get_event_1 = *Atrshmlogc::atrshmlog_error_get_event_1;
*atrshmlog_error_get_logging_1 = *Atrshmlogc::atrshmlog_error_get_logging_1;
*atrshmlog_error_get_logging_2 = *Atrshmlogc::atrshmlog_error_get_logging_2;
*atrshmlog_error_get_logging_3 = *Atrshmlogc::atrshmlog_error_get_logging_3;
*atrshmlog_error_get_logging_4 = *Atrshmlogc::atrshmlog_error_get_logging_4;
*atrshmlog_error_create_1 = *Atrshmlogc::atrshmlog_error_create_1;
*atrshmlog_error_create_2 = *Atrshmlogc::atrshmlog_error_create_2;
*atrshmlog_error_create_3 = *Atrshmlogc::atrshmlog_error_create_3;
*atrshmlog_error_create_4 = *Atrshmlogc::atrshmlog_error_create_4;
*atrshmlog_error_init_shm_1 = *Atrshmlogc::atrshmlog_error_init_shm_1;
*atrshmlog_error_init_shm_2 = *Atrshmlogc::atrshmlog_error_init_shm_2;
*atrshmlog_error_init_shm_3 = *Atrshmlogc::atrshmlog_error_init_shm_3;
*atrshmlog_error_read_1 = *Atrshmlogc::atrshmlog_error_read_1;
*atrshmlog_error_read_2 = *Atrshmlogc::atrshmlog_error_read_2;
*atrshmlog_error_read_3 = *Atrshmlogc::atrshmlog_error_read_3;
*atrshmlog_error_read_4 = *Atrshmlogc::atrshmlog_error_read_4;
*atrshmlog_error_read_5 = *Atrshmlogc::atrshmlog_error_read_5;
*atrshmlog_error_read_fetch_1 = *Atrshmlogc::atrshmlog_error_read_fetch_1;
*atrshmlog_error_read_fetch_2 = *Atrshmlogc::atrshmlog_error_read_fetch_2;
*atrshmlog_error_read_fetch_3 = *Atrshmlogc::atrshmlog_error_read_fetch_3;
*atrshmlog_error_read_fetch_4 = *Atrshmlogc::atrshmlog_error_read_fetch_4;
*atrshmlog_error_read_fetch_5 = *Atrshmlogc::atrshmlog_error_read_fetch_5;
*atrshmlog_error_verify_1 = *Atrshmlogc::atrshmlog_error_verify_1;
*atrshmlog_error_verify_2 = *Atrshmlogc::atrshmlog_error_verify_2;
*atrshmlog_error_verify_3 = *Atrshmlogc::atrshmlog_error_verify_3;
*atrshmlog_error_verify_4 = *Atrshmlogc::atrshmlog_error_verify_4;
*atrshmlog_error_verify_5 = *Atrshmlogc::atrshmlog_error_verify_5;
*atrshmlog_error_verify_6 = *Atrshmlogc::atrshmlog_error_verify_6;
*atrshmlog_strategy_first = *Atrshmlogc::atrshmlog_strategy_first;
*atrshmlog_strategy_discard = *Atrshmlogc::atrshmlog_strategy_discard;
*atrshmlog_strategy_spin_loop = *Atrshmlogc::atrshmlog_strategy_spin_loop;
*atrshmlog_strategy_wait = *Atrshmlogc::atrshmlog_strategy_wait;
*atrshmlog_strategy_adaptive = *Atrshmlogc::atrshmlog_strategy_adaptive;
*atrshmlog_strategy_adaptive_fast = *Atrshmlogc::atrshmlog_strategy_adaptive_fast;
*atrshmlog_strategy_adaptive_very_fast = *Atrshmlogc::atrshmlog_strategy_adaptive_very_fast;
*atrshmlog_strategy_last = *Atrshmlogc::atrshmlog_strategy_last;
1;
