#ifndef __FIFO_H__
#define __FIFO_H__
#include <stdbool.h>
#include <stdlib.h>

struct fifo
{
    void * tab;
    size_t size_elt;
    size_t size_fifo;
    size_t rd_ptr;
    size_t wr_ptr;
    bool rd_flag;
    bool wr_flag;
};

void fifo_create(struct fifo * fifo, void * tab, size_t size_elt, size_t size_fifo);
size_t fifo_level(struct fifo * fifo);
bool fifo_is_empty(struct fifo * fifo);
bool fifo_is_full(struct fifo * fifo);
bool fifo_push(struct fifo * fifo, const void * elt);
bool fifo_pop(struct fifo * fifo, void * elt);
#endif // __FIFO_H__
