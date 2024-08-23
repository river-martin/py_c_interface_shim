#ifndef SUM_NUMBERS_H
#define SUM_NUMBERS_H
#include "c_number_interface.h"

// This is called from the py_sum_numbers.py
C_NumInterface *
c_sum_numbers(C_NumInterface *a, C_NumInterface *b, C_NumInterface *c);
#endif // SUM_NUMBERS_H