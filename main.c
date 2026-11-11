#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "error.h"
#include "lawn.h"
#include "mole.h"
#include "mtq.h"
#include "thread.h"
 
static void *produce(void *a) {
  void **arg=a;
  Mtq q=(Mtq)arg[0];
  Lawn l=(Lawn)arg[1];
  mtq_tail_put(q, mole_new(l,0,0));
  return 0;
}

static void *consume(void *a) {
  void **arg=a;
  Mtq q=(Mtq)arg[0];
  mole_whack(mtq_head_get(q));
  return 0;
}

static int parse_count(int argc, char **argv) {
  if (argc > 2)
    ERROR("usage: %s [mole-count]",argv[0]);
  if (argc == 1)
    return 10;

  char *end;
  errno=0;
  long value=strtol(argv[1],&end,10);
  if (errno || *argv[1]=='\0' || *end!='\0' || value < 0 || value > INT_MAX)
    ERROR("invalid mole count: %s",argv[1]);
  return (int)value;
}

int main(int argc, char **argv) {
  srandom(time(0));
  const int n=parse_count(argc,argv);
  Lawn lawn=lawn_new(0,0);
  Mtq q=mtq_new(4);
  void *produce_arg[]={q,lawn};
  void *consume_arg[]={q};
  ThreadGroup *producers=thread_group_new(produce,produce_arg,n);
  ThreadGroup *consumers=thread_group_new(consume,consume_arg,n);
  thread_group_join(producers);
  thread_group_join(consumers);
  mtq_free(q);
  lawn_free(lawn);
}
