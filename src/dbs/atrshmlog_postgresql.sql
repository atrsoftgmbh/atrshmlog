--
-- postgres schema objects
-- $Id:$
--
-- we have a sequence for connect the insert data
-- from a bunch of chunks to the process data
--
create sequence atrshmlog_head_seq
       minvalue 1
       no maxvalue
       start with 1
       cache 10;
--
-- We have the head table with all head info
create table atrshmlog_head(head_id bigint not null,  -- the sequence number
       version integer not null,
       totallen integer not null,
       pid bigint not null,
       tid bigint not null,
       bnr integer not null,
       fnr integer not null,
       inittime_sec bigint not null,
       inittime_nsec integer not null,
       inittime_before bigint not null,
       inittime_after bigint not null,
       lasttime_sec bigint not null,
       lasttime_nsec integer not null,
       lasttime_before bigint not null,
       lasttime_after bigint not null,
       difftimetransfer bigint not null,
       starttransfer bigint not null,
       acquiretime bigint not null,
       bid integer not null,
       number_dispatched  integer not null,
       counter_write0 integer not null,
       counter_write0_discard integer not null,
       counter_write0_wait integer not null,
       counter_write0_adaptive integer not null,
       counter_write0_adaptive_fast integer not null,
       counter_write0_adaptive_very_fast integer not null,
       counter_write1 integer not null,
       counter_write1_discard integer not null,
       counter_write1_wait integer not null,
       counter_write1_adaptive integer not null,
       counter_write1_adaptive_fast integer not null,
       counter_write1_adaptive_very_fast integer not null,
       counter_write2 integer not null,
       counter_write2_discard integer not null,
       counter_write2_wait integer not null,
       counter_write2_adaptive integer not null,
       counter_write2_adaptive_fast integer not null,
       counter_write2_adaptive_very_fast integer not null
       );
--
-- We have an index for id
create index atrshmlog_head_001_ind on atrshmlog_head(head_id);
--
--
-- We have the logging table for cstring
-- 
create table atrshmlog_log(head_id bigint not null, -- link to head
       pid bigint not null,	   
       tid bigint not null,
       startclick bigint not null,
       endclick bigint not null,
       deltaclick bigint not null,
       startreal bigint not null,
       endreal bigint not null,
       deltareal bigint not null,
       logevent integer not null,
       logflag char(1) not null,
       userflag integer not null,
       payload text not null
       );
--
-- We have a foreign key, so an index is needed here too
create index atrshmlog_log_001_ind on atrshmlog_log(head_id);
--
--
-- We have the logging table for ucs2 strings
-- 
create table atrshmlog_log_ucs2(head_id bigint not null, -- link to head
       pid bigint not null,			   
       tid bigint not null,			   
       startclick bigint not null,		   
       endclick bigint not null,		   
       deltaclick bigint not null,
       startreal bigint not null,
       endreal bigint not null,
       deltareal bigint not null,
       logevent integer not null,
       logflag char(1) not null,
       userflag integer not null,
       payload bytea not null
       );
--
-- We have a foreign key, so an index is needed here too
create index atrshmlog_log_ucs2_001_ind on atrshmlog_log_ucs2(head_id);
--
-- end of file
