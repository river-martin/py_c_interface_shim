#include "c_interface_consumer.h"
#include <stdio.h>
#include <stdlib.h>

// This file is a simple stand-in for bru.c / srvm.c

C_NumInterface *
c_sum_numbers(C_NumInterface *a, C_NumInterface *b, C_NumInterface *c)
{
    printf("a: %d\n", a->get_value(a));
    printf("b: %d\n", b->get_value(b));
    printf("c: %d\n", c->get_value(c));
    C_NumInterface *a_plus_b = a->add(a, b);
    C_NumInterface *result = a_plus_b->add(a_plus_b, c);
    a_plus_b->free(a_plus_b);
    printf("Sum: %d\n", result->get_value(result));
    return result;
}
