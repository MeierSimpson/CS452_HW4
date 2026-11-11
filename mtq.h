#ifndef MTQ_H
#define MTQ_H

#include "deq.h"

typedef void *Mtq;

extern Mtq mtq_new(int capacity);
extern void mtq_free(Mtq q);
extern int mtq_len(Mtq q);
extern void mtq_tail_put(Mtq q, Data d);
extern Data mtq_head_get(Mtq q);

#endif
