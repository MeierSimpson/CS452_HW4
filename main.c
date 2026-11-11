#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "deq.h"
#include "lawn.h"
#include "mole.h"
 
static void produce(Deq q, Lawn l) {
  deq_tail_put(q, mole_new(l,0,0));
}

static void consume(Deq q) {
  mole_whack(deq_head_get(q));
}

int main() {
  srandom(time(0));
  const int n=10;
  Lawn lawn=lawn_new(0,0);
  Deq q=deq_new();
  for (int i=0; i<n; i++)
    produce(q,lawn);
  for (int i=0; i<n; i++)
    consume(q);
  deq_del(q,0);
  lawn_free(lawn);
}
