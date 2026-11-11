#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "deq.h"
#include "lawn.h"
#include "mole.h"
#include "thread.h"
 
static void *produce(void *a) {
  void **arg=a;
  Deq q=(Deq)arg[0];
  Lawn l=(Lawn)arg[1];
  deq_tail_put(q, mole_new(l,0,0));
  return 0;
}

static void *consume(void *a) {
  void **arg=a;
  Deq q=(Deq)arg[0];
  mole_whack(deq_head_get(q));
  return 0;
}

int main() {
  srandom(time(0));
  const int n=10;
  Lawn lawn=lawn_new(0,0);
  Deq q=deq_new();
  void *produce_arg[]={q,lawn};
  void *consume_arg[]={q};
  ThreadGroup *producers=thread_group_new(produce,produce_arg,n);
  thread_group_join(producers);
  ThreadGroup *consumers=thread_group_new(consume,consume_arg,n);
  thread_group_join(consumers);
  deq_del(q,0);
  lawn_free(lawn);
}
