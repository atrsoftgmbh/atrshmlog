--
-- oracle schema objects
-- $Id:$
--
-- we have a sequence for connect the insert data
-- from a bunch of chunks to the process data
--
create sequence atrshmlog_head_seq
       minvalue 1
       start with 1
       cache 100
       INCREMENT by 1;
--
-- We have the head table with all head info
-- we need to cut of names for oracle here ...
create table atrshmlog_head(head_id number(38,0) not null,  -- the sequence number
       version NUMBER(38,0) not null,
       totallen  NUMBER(38,0) not null,
       pid  NUMBER(38,0) not null,
       tid  NUMBER(38,0) not null,
       bnr  NUMBER(38,0) not null,
       fnr  NUMBER(38,0) not null,
       inittime_sec NUMBER(38,0)  not null,
       inittime_nsec  NUMBER(38,0) not null,
       inittime_before  NUMBER(38,0) not null,
       inittime_after  NUMBER(38,0) not null,
       lasttime_sec  NUMBER(38,0) not null,
       lasttime_nsec  NUMBER(38,0) not null,
       lasttime_before  NUMBER(38,0) not null,
       lasttime_after  NUMBER(38,0) not null,
       difftimetransfer  NUMBER(38,0) not null,
       starttransfer  NUMBER(38,0) not null,
       acquiretime  NUMBER(38,0) not null,
       bid  NUMBER(38,0) not null,
       number_dispatched   NUMBER(38,0) not null,
       counter_write0  NUMBER(38,0) not null,
       counter_write0_discard  NUMBER(38,0) not null,
       counter_write0_wait NUMBER(38,0) not null,
       counter_write0_adaptive NUMBER(38,0) not null,
       counter_write0_adaptive_fast NUMBER(38,0) not null,
       counter_write0_adaptive_very_f NUMBER(38,0) not null,
       counter_write1 NUMBER(38,0) not null,
       counter_write1_discard NUMBER(38,0) not null,
       counter_write1_wait NUMBER(38,0) not null,
       counter_write1_adaptive NUMBER(38,0) not null,
       counter_write1_adaptive_fast NUMBER(38,0) not null,
       counter_write1_adaptive_very_f NUMBER(38,0) not null,
       counter_write2 NUMBER(38,0) not null,
       counter_write2_discard NUMBER(38,0) not null,
       counter_write2_wait NUMBER(38,0) not null,
       counter_write2_adaptive NUMBER(38,0) not null,
       counter_write2_adaptive_fast NUMBER(38,0)  not null,
       counter_write2_adaptive_very_f NUMBER(38,0) not null
       );
--
-- We have an index for id
create index atrshmlog_head_001_ind on atrshmlog_head(head_id);
--
--
-- We have the logging table for cstring
-- 
create table atrshmlog_log(head_id NUMBER(38,0) not null, -- link to head
       pid NUMBER(38,0) not null,	   
       tid NUMBER(38,0) not null,
       startclick NUMBER(38,0) not null,
       endclick NUMBER(38,0) not null,
       deltaclick NUMBER(38,0) not null,
       startreal NUMBER(38,0) not null,
       endreal NUMBER(38,0) not null,
       deltareal NUMBER(38,0) not null,
       logevent NUMBER(38,0) not null,
       logflag char(1) not null,
       userflag NUMBER(38,0) not null,
       len NUMBER(38,0) NOT NULL,
       payload VARCHAR2(4000) null,
       payload_big CLOB NOT NULL
       );
--
-- We have a foreign key, so an index is needed here too
create index atrshmlog_log_001_ind on atrshmlog_log(head_id);
--
--
-- We have the logging table for ucs2 strings
-- 
create table atrshmlog_log_ucs2(head_id NUMBER(38,0) not null, -- link to head
       pid NUMBER(38,0) not null,			   
       tid NUMBER(38,0) not null,			   
       startclick NUMBER(38,0) not null,		   
       endclick NUMBER(38,0) not null,		   
       deltaclick NUMBER(38,0) not null,
       startreal NUMBER(38,0) not null,
       endreal NUMBER(38,0) not null,
       deltareal NUMBER(38,0) not null,
       logevent NUMBER(38,0) not null,
       logflag char(1) not null,
       userflag NUMBER(38,0) not null,
       len NUMBER(38,0) NOT NULL,
       payload_big BLOB not null
       );
--
-- We have a foreign key, so an index is needed here too
create index atrshmlog_log_ucs2_001_ind on atrshmlog_log_ucs2(head_id);
--
-- end of file
