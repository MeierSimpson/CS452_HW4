#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "mtq.h"

typedef struct {
  Deq deq;
  int capacity;
  pthread_mutex_t mutex;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
} MtqRep;

static MtqRep *rep(Mtq q) {
  if (!q) ERROR("zero queue pointer");
  return (MtqRep *)q;
}

static void check(int rc, const char *operation) {
  if (rc) ERROR("%s failed: %s",operation,strerror(rc));
}

extern Mtq mtq_new(int capacity) {
  if (capacity < 0) ERROR("negative queue capacity");

  MtqRep *r=malloc(sizeof(*r));
  if (!r) ERROR("malloc() failed");
  r->deq=deq_new();
  r->capacity=capacity;
  check(pthread_mutex_init(&r->mutex,0),"pthread_mutex_init()");
  check(pthread_cond_init(&r->not_empty,0),"pthread_cond_init()");
  check(pthread_cond_init(&r->not_full,0),"pthread_cond_init()");
  return r;
}

extern void mtq_free(Mtq q) {
  MtqRep *r=rep(q);
  check(pthread_mutex_lock(&r->mutex),"pthread_mutex_lock()");
  if (deq_len(r->deq)) {
    check(pthread_mutex_unlock(&r->mutex),"pthread_mutex_unlock()");
    ERROR("freeing a non-empty queue");
  }
  check(pthread_mutex_unlock(&r->mutex),"pthread_mutex_unlock()");
  deq_del(r->deq,0);
  check(pthread_cond_destroy(&r->not_full),"pthread_cond_destroy()");
  check(pthread_cond_destroy(&r->not_empty),"pthread_cond_destroy()");
  check(pthread_mutex_destroy(&r->mutex),"pthread_mutex_destroy()");
  free(r);
}

extern int mtq_len(Mtq q) {
  MtqRep *r=rep(q);
  check(pthread_mutex_lock(&r->mutex),"pthread_mutex_lock()");
  int len=deq_len(r->deq);
  check(pthread_mutex_unlock(&r->mutex),"pthread_mutex_unlock()");
  return len;
}

extern void mtq_tail_put(Mtq q, Data d) {
  MtqRep *r=rep(q);
  check(pthread_mutex_lock(&r->mutex),"pthread_mutex_lock()");
  while (r->capacity && deq_len(r->deq) >= r->capacity)
    check(pthread_cond_wait(&r->not_full,&r->mutex),"pthread_cond_wait()");
  deq_tail_put(r->deq,d);
  check(pthread_cond_signal(&r->not_empty),"pthread_cond_signal()");
  check(pthread_mutex_unlock(&r->mutex),"pthread_mutex_unlock()");
}

extern Data mtq_head_get(Mtq q) {
  MtqRep *r=rep(q);
  check(pthread_mutex_lock(&r->mutex),"pthread_mutex_lock()");
  while (!deq_len(r->deq))
    check(pthread_cond_wait(&r->not_empty,&r->mutex),"pthread_cond_wait()");
  Data d=deq_head_get(r->deq);
  check(pthread_cond_signal(&r->not_full),"pthread_cond_signal()");
  check(pthread_mutex_unlock(&r->mutex),"pthread_mutex_unlock()");
  return d;
}
