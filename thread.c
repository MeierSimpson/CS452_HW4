#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "thread.h"

extern ThreadGroup *thread_group_new(ThreadFunction f, void *arg, int n) {
  if (!f) ERROR("null thread function");
  if (n < 0) ERROR("negative thread count");

  ThreadGroup *group=malloc(sizeof(*group));
  if (!group) ERROR("malloc() failed");
  group->count=n;
  group->threads=n ? malloc((size_t)n*sizeof(*group->threads)) : 0;
  if (n && !group->threads) ERROR("malloc() failed");

  for (int i=0; i<n; i++) {
    int rc=pthread_create(&group->threads[i],0,f,arg);
    if (rc) ERROR("pthread_create() failed: %s",strerror(rc));
  }
  return group;
}

extern void thread_group_join(ThreadGroup *group) {
  if (!group) ERROR("null thread group");
  for (int i=0; i<group->count; i++) {
    int rc=pthread_join(group->threads[i],0);
    if (rc) ERROR("pthread_join() failed: %s",strerror(rc));
  }
  free(group->threads);
  free(group);
}
