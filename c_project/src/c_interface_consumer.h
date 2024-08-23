#ifndef SUM_NUMBERS_H
#define SUM_NUMBERS_H
#include "c_number_interface.h"

// This is called from the py_c_num_inteface_shim.c
C_NumInterface *
c_sum_numbers(C_NumInterface *a, C_NumInterface *b, C_NumInterface *c);
#endif // SUM_NUMBERS_H
