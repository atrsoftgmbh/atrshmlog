--
-- mariadb schema objects
-- $Id:$
--
-- we have no sequence.
-- so we use instead the autoincerement and last id
--
-- We have the head table with all head info
create table atrshmlog_head(head_id bigint not null AUTO_INCREMENT PRIMARY KEY,  -- the sequence number
       version int not null,
       totallen int not null,
       pid bigint not null,
       tid bigint not null,
       bnr int not null,
       fnr int not null,
       inittime_sec bigint not null,
       inittime_nsec int not null,
       inittime_before bigint not null,
       inittime_after bigint not null,
       lasttime_sec bigint not null,
       lasttime_nsec int not null,
       lasttime_before bigint not null,
       lasttime_after bigint not null,
       difftimetransfer bigint not null,
       starttransfer bigint not null,
       acquiretime bigint not null,
       bid int not null,
       number_dispatched  int not null,
       counter_write0 bigint not null,
       counter_write0_discard bigint not null,
       counter_write0_wait bigint not null,
       counter_write0_adaptive bigint not null,
       counter_write0_adaptive_fast bigint not null,
       counter_write0_adaptive_very_fast bigint not null,
       counter_write1 bigint not null,
       counter_write1_discard bigint not null,
       counter_write1_wait bigint not null,
       counter_write1_adaptive bigint not null,
       counter_write1_adaptive_fast bigint not null,
       counter_write1_adaptive_very_fast bigint not null,
       counter_write2 bigint not null,
       counter_write2_discard bigint not null,
       counter_write2_wait bigint not null,
       counter_write2_adaptive bigint not null,
       counter_write2_adaptive_fast bigint not null,
       counter_write2_adaptive_very_fast bigint not null
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
       logevent int not null,
       logflag char(1) not null,
       userflag int not null,
       payload mediumtext not null
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
       logevent int not null,
       logflag char(1) not null,
       userflag int not null,
       payload mediumblob not null
       );
--
-- We have a foreign key, so an index is needed here too
create index atrshmlog_log_ucs2_001_ind on atrshmlog_log_ucs2(head_id);
--
