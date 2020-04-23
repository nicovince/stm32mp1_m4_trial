#ifndef __FIFO_H__
#define __FIFO_H__
#include <stdbool.h>
#include <stdlib.h>

struct fifo_struct
{
    void * tab;
    size_t size_elt;
    size_t size_fifo;
    size_t rd_ptr;
    size_t wr_ptr;
    bool rd_flag;
    bool wr_flag;
};

void fifo_create(struct fifo_struct * fifo, void * tab, size_t size_elt, size_t size_fifo);
size_t fifo_level(struct fifo_struct * fifo);
bool fifo_is_empty(struct fifo_struct * fifo);
bool fifo_is_full(struct fifo_struct * fifo);
bool fifo_push(struct fifo_struct * fifo, void * elt);
bool fifo_pop(struct fifo_struct * fifo, void * elt);
#endif // __FIFO_H__
