#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

typedef void *(*ThreadFunction)(void *arg);

typedef struct {
  pthread_t *threads;
  int count;
} ThreadGroup;

extern ThreadGroup *thread_group_new(ThreadFunction f, void *arg, int n);
extern void thread_group_join(ThreadGroup *group);

#endif
