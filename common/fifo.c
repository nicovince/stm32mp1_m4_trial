#include <string.h>
#include "fifo.h"

static void toggle_flag(bool *flag)
{
    if (*flag == false) {
        *flag = true;
    } else {
        *flag = false;
    }
}

void fifo_create(struct fifo * fifo, void * tab, size_t size_elt, size_t size_fifo)
{
    fifo->tab = tab;
    fifo->size_elt = size_elt;
    fifo->size_fifo = size_fifo;
    fifo->rd_ptr = 0;
    fifo->wr_ptr = 0;
    fifo->rd_flag = false;
    fifo->wr_flag = false;
}

size_t fifo_level(struct fifo * fifo)
{
    if (fifo->wr_ptr == fifo->rd_ptr) {
        if (fifo->wr_flag == fifo->rd_flag) {
            return fifo->size_fifo;
        } else {
            return 0;
        }
    } else if (fifo->wr_ptr > fifo->rd_ptr) {
        return fifo->wr_ptr - fifo->rd_ptr;
    } else {
        return fifo->size_fifo + fifo->wr_ptr - fifo->rd_ptr;
    }
}

bool fifo_is_empty(struct fifo * fifo)
{
    return ((fifo->wr_ptr == fifo->rd_ptr) && (fifo->rd_flag == fifo->wr_flag));
}

bool fifo_is_full(struct fifo * fifo)
{
    return ((fifo->wr_ptr == fifo->rd_ptr) && (fifo->rd_flag != fifo->wr_flag));
}

bool fifo_push(struct fifo * fifo, void * elt)
{
    bool ret_fifo_push = false;

    if (fifo_is_full(fifo) == false)
    {
        memcpy((fifo->tab + (fifo->wr_ptr * fifo->size_elt)), elt, fifo->size_elt);
        fifo->wr_ptr++;
        if (fifo->wr_ptr == fifo->size_fifo) {
            fifo->wr_ptr = 0;
            toggle_flag(&fifo->wr_flag);
        }
        ret_fifo_push = true;
    }

    return ret_fifo_push;
}

bool fifo_pop(struct fifo * fifo, void * elt)
{
    bool ret_fifo_pop = false;

    if (fifo_is_empty(fifo) == false)
    {
        memcpy(elt, (fifo->tab + (fifo->rd_ptr * fifo->size_elt)), fifo->size_elt);
        fifo->rd_ptr++;
        if (fifo->rd_ptr == fifo->size_fifo) {
            fifo->rd_ptr = 0;
            toggle_flag(&fifo->rd_flag);
        }
        ret_fifo_pop = true;
    }

    return ret_fifo_pop;
}

