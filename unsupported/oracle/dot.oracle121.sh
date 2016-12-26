#
#
ORACLE_BASE=/opt/oracle
ORACLE_HOME=$ORACLE_BASE/product/12.1.0/dbhome_1

export ORACLE_BASE ORACLE_HOME

PATH=$PATH:$ORACLE_HOME/bin

export PATH

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ORACLE_HOME/lib

export LD_LIBRARY_PATH

# linking needs this, but we do it direct in the shm file
# LDFLAGS="-L/opt/oracle/product/12.1.0/dbhome_1/lib -L/opt/oracle/product/12.1.0/dbhome_1/rdbms/lib -lclntsh -lclntshcore -ldl -lm -lpthread -lnsl -lirc -limf -lirc -lrt -lsvml -m64"

# export LDFLAGS

# compile needs this
CFLAGS="-I/opt/oracle/product/12.1.0/dbhome_1/rdbms/public -I/opt/oracle/product/12.1.0/dbhome_1/plsql/public -I/opt/oracle/product/12.1.0/dbhome_1/network/public -I/opt/oracle/product/12.1.0/dbhome_1/precomp/public -DLINUX -DORAX86_64 -D_GNU_SOURCE -D_LARGEFILE64_SOURCE=1 -D_LARGEFILE_SOURCE=1 -DSLTS_ENABLE -DSLMXMX_ENABLE -D_REENTRANT -DNS_THREADS -DLONG_IS_64 -DSS_64BIT_SERVER -DLDAP_CM"

export CFLAGS

# the tnsnames.ora and sqlnet.ora files
TNS_ADMIN=$HOME/network/admin
export TNS_ADMIN


# end of file
