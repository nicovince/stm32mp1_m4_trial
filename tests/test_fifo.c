#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "fifo.h"

/* Fifo under test */
struct fut
{
    struct fifo_struct fifo;
    void *tab;
};

struct fut * create_test_fifo(size_t fifo_size, size_t fifo_elt_size)
{
    struct fut * fifo_ut;
    fifo_ut = malloc(sizeof(struct fut));
    if (fifo_ut == NULL) {
        return fifo_ut;
    }
    fifo_ut->tab = malloc(fifo_size * fifo_elt_size);

    if (fifo_ut->tab == NULL) {
        free(fifo_ut);
        return NULL;
    }
    fifo_create(&fifo_ut->fifo, fifo_ut->tab, fifo_elt_size, fifo_size);
    return fifo_ut;
}

void destroy_test_fifo(struct fut * fifo_ut)
{
    free(fifo_ut->tab);
    free(fifo_ut);
}

void force_fifo(struct fut *fifo_ut, size_t rd, size_t wr, bool rd_flag, bool wr_flag)
{
    assert(rd < fifo_ut->fifo.size_fifo);
    assert(wr < fifo_ut->fifo.size_fifo);
    fifo_ut->fifo.rd_ptr = rd;
    fifo_ut->fifo.wr_ptr = wr;
    fifo_ut->fifo.rd_flag = rd_flag;
    fifo_ut->fifo.wr_flag = wr_flag;
}

void force_fifo_full(struct fut *fifo_ut)
{
    force_fifo(fifo_ut, 0, 0, false, true);
}

bool test_fifo_empty(void)
{
    bool ret = false;
    size_t fifo_sz = 8;
    struct fut * fut = create_test_fifo(fifo_sz, 1);

    printf("Test Empty fifo:\n");
    printf(" fifo_is_empty: ");
    if (fifo_is_empty(&fut->fifo) != true) {
        goto out;
    }
    printf("PASS\n");

    printf(" fifo_is_full: ");
    if (fifo_is_full(&fut->fifo) != false) {
        goto out;
    }
    printf("PASS\n");
    ret = true;
out:
    if (ret == false) {
        printf("FAIL\n");
    }
    destroy_test_fifo(fut);
    return ret;
}

bool test_fifo_full(void)
{
    bool ret = false;
    size_t fifo_sz = 8;
    struct fut * fut = create_test_fifo(fifo_sz, 1);
    force_fifo_full(fut);

    printf("Test Full fifo:\n");
    printf(" fifo_is_empty: ");
    if (fifo_is_empty(&fut->fifo) == true) {
        goto out;
    }
    printf("PASS\n");

    printf(" fifo_is_full: ");
    if (fifo_is_full(&fut->fifo) == false) {
        goto out;
    }
    printf("PASS\n");
    ret = true;
out:
    if (ret == false) {
        printf("FAIL\n");
    }
    destroy_test_fifo(fut);
    return ret;
}

bool test_empty_pop(void)
{
    bool ret = false;
    size_t fifo_sz = 8;
    struct fut * fut = create_test_fifo(fifo_sz, 1);
    uint8_t elt;

    printf("Test Pop Empty Fifo:\n");
    printf(" fifo_pop: ");
    if (fifo_pop(&fut->fifo, &elt) == true) {
        goto out;
    }
    printf("PASS\n");

    ret = true;
out:
    if (ret == false) {
        printf("FAIL\n");
    }
    destroy_test_fifo(fut);
    return ret;
}

bool test_full_push(void)
{
    bool ret = false;
    size_t fifo_sz = 8;
    struct fut * fut = create_test_fifo(fifo_sz, 1);
    uint8_t elt;

    force_fifo_full(fut);
    printf("Test Push Full Fifo:\n");
    printf(" fifo_push: ");
    if (fifo_push(&fut->fifo, &elt) == true) {
        goto out;
    }
    printf("PASS\n");

    ret = true;
out:
    if (ret == false) {
        printf("FAIL\n");
    }
    destroy_test_fifo(fut);
    return ret;
}

bool test_push_until_full(struct fut *fut)
{
    bool ret = false;
    uint8_t elt;

    printf(" fifo_push: ");
    for (size_t i = 0; i < fut->fifo.size_fifo; ++i) {
        printf("%ld ", i);
        if (fifo_push(&fut->fifo, &elt) != true) {
            goto out;
        }
    }
    printf("PASS\n");

    printf(" is_full: ");
    if (fifo_is_full(&fut->fifo) == false) {
        goto out;
    }
    printf("PASS\n");

    ret = true;
out:
    if (ret == false) {
        printf("FAIL\n");
    }
    destroy_test_fifo(fut);
    return ret;
}

bool test_push_until_full_nowrap(void)
{
    size_t fifo_sz = 8;
    struct fut * fut = create_test_fifo(fifo_sz, 1);
    printf("Test Push Until Fifo Full (no wrap):\n");
    return test_push_until_full(fut);
}

bool test_push_until_full_wrap(void)
{
    size_t fifo_sz = 8;
    struct fut * fut = create_test_fifo(fifo_sz, 1);
    force_fifo(fut, fifo_sz -1, fifo_sz -1, true, true);
    printf("Test Push Until Fifo Full (wrap arround):\n");
    return test_push_until_full(fut);
}

bool test_pop_until_empty(void)
{
    bool ret = false;
    size_t fifo_sz = 8;
    struct fut * fut = create_test_fifo(fifo_sz, 1);
    uint8_t elt;

    force_fifo_full(fut);
    printf("Test Pop Until Fifo Empty:\n");
    printf(" fifo_pop: ");
    for (size_t i = 0; i < fifo_sz; ++i) {
        printf("%ld ", i);
        if (fifo_pop(&fut->fifo, &elt) != true) {
            goto out;
        }
    }
    printf("PASS\n");

    printf(" is_empty: ");
    if (fifo_is_empty(&fut->fifo) == false) {
        goto out;
    }
    printf("PASS\n");

    ret = true;
out:
    if (ret == false) {
        printf("FAIL\n");
    }
    destroy_test_fifo(fut);
    return ret;
}

typedef bool (*test_fn)(void);

void test_fifo_function(test_fn f)
{
    bool res = f();
    if (res == false) {
        printf("FAIL\n");
        assert(false);
    } else {
        printf("PASS\n");
    }
}

int main(int argc, char ** argv)
{
    test_fifo_function(&test_fifo_empty);
    test_fifo_function(&test_fifo_full);
    test_fifo_function(&test_empty_pop);
    test_fifo_function(&test_full_push);
    test_fifo_function(&test_push_until_full_nowrap);
    test_fifo_function(&test_push_until_full_wrap);
    test_fifo_function(&test_pop_until_empty);
    return 0;
}
