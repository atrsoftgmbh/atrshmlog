#include <cassandra.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  /* Setup and connect to cluster */
  CassCluster* cluster = cass_cluster_new();
  CassSession* session = cass_session_new();

  /* Add contact points */
  cass_cluster_set_contact_points(cluster, argv[1]);

  /* Provide the cluster object as configuration to connect the session */
  CassFuture* connect_future = cass_session_connect(session, cluster);

  /* This operation will block until the result is ready */
  CassError rc = cass_future_error_code(connect_future);

  printf("%ld\n", (long)rc);
  
  printf("Connect result: %s\n", cass_error_desc(rc));

  /* Run queries... */

  cass_future_free(connect_future);
  cass_session_free(session);
  cass_cluster_free(cluster);

  return 0;
}
