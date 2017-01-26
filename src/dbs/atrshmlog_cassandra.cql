--
-- we create the cassandra db
--
CREATE KEYSPACE atrshmlog_ks
WITH replication = { 'class' :'SimpleStrategy',
      'replication_factor' : 2 };
--
USE atrshmlog_ks;
--
CREATE TABLE theSeq (
    self INT PRIMARY KEY,
    head_id counter
)
;
UPDATE theSeq
SET
    head_id = head_id + 1;
where
    self = 1
;
--
CREATE TABLE atrshmlog_head(
    head_id BIGINT PRIMARY KEY,
    version INT,
    totallen INT,
    pid BIGINT,
    tid BIGINT,
    bnr INT,
    fnr INT,
    inittime_sec BIGINT,
    inittime_nsec INT,
    inittime_before BIGINT,
    inittime_after BIGINT,
    lasttime_sec BIGINT,
    lasttime_nsec INT,
    lasttime_before BIGINT,
    lasttime_after BIGINT,
    difftimetransfer BIGINT,
    starttransfer BIGINT,
    acquiretime BIGINT,
    bid INT,
    number_dispatched  INT ,
    counter_write0 INT ,
    counter_write0_discard INT ,
    counter_write0_wait INT ,
    counter_write0_adaptive INT ,
    counter_write0_adaptive_fast INT ,
    counter_write0_adaptive_very_fast INT ,
    counter_write1 INT ,
    counter_write1_discard INT ,
    counter_write1_wait INT ,
    counter_write1_adaptive INT ,
    counter_write1_adaptive_fast INT ,
    counter_write1_adaptive_very_fast INT ,
    counter_write2 INT ,
    counter_write2_discard INT ,
    counter_write2_wait INT ,
    counter_write2_adaptive INT ,
    counter_write2_adaptive_fast INT ,
    counter_write2_adaptive_very_fast INT 
)
;
--
--
create table atrshmlog_log(head_id bigint , -- link to head
       rowid bigint,
       pid bigint ,	   
       tid bigint ,
       startclick bigint ,
       endclick bigint ,
       deltaclick bigint ,
       startreal bigint ,
       endreal bigint ,
       deltareal bigint ,
       logevent int ,
       logflag ASCII ,
       userflag int ,
       payload ASCII,
       PRIMARY KEY (head_id, rowid)
       )
       ;
--
--
create table atrshmlog_log_ucs2(head_id bigint , -- link to head
       rowid  bigint,
       pid bigint ,			   
       tid bigint ,			   
       startclick bigint ,		   
       endclick bigint ,		   
       deltaclick bigint ,
       startreal bigint ,
       endreal bigint ,
       deltareal bigint ,
       logevent int ,
       logflag ASCII ,
       userflag int ,
       payload blob,
       PRIMARY KEY (head_id, rowid)
       )
       ;
--
--
-- end of file

