SET ECHO OFF
--
set serveroutput on size unlimited
--
define TBS = COSAZ
--
define TMP = TEMP
--
define U = atrshmlog
--
define P = atrshmlog4711
--
create user &U.
identified by &P.
CONTAINER = CURRENT
DEFAULT TABLESPACE &TBS.
TEMPORARY TABLESPACE &TMP.
PROFILE DEFAULT ACCOUNT UNLOCK
/
--
grant connect,resource  to &U.
/
--
ALTER USER &U. QUOTA UNLIMITED ON &TBS.
/
--
-- xa tranasaction freischalten
GRANT SELECT ON sys.dba_pending_transactions TO &U.
/
grant  SELECT ON sys.pending_trans$ TO &U.
/
grant SELECT ON sys.dba_2pc_pending TO &U.
/
GRANT EXECUTE ON sys.dbms_xa TO  &U.
/

commit
/
--
exit;


