#ifndef C_NUMBER_INTERFACE_H
#define C_NUMBER_INTERFACE_H

typedef struct c_number_interface {
    // This could be a pointer to a C implementation or a Python implementation
    void *impl;
    // Takes two instances of the interface and returns a new instance of the
    // interface
    void *(*add)(void *a, void *b);
    int (*get_value)(void *impl);
    void (*free)(void *impl);
} C_NumInterface;

#endif // C_NUMBER_INTERFACE_H
