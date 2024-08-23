#include "../../c_project/src/c_interface_consumer.h"
#include "../../c_project/src/c_number_interface.h"

#include <Python.h>
#include <structmember.h>

// Python/C interface shim (core implementation is in `py_number_impl.py`)
static PyTypeObject PyC_NumInterfaceShimType;

typedef struct py_number_object {
    PyObject_HEAD // Sadly no semicolon here;
        C_NumInterface number;
    int                   value;
} PyC_NumInterfaceShimObject;

// ------------------------------------------
// Boiler plate code for the C interface shim

/**
 * This is the C function that will actually be pointed to by instances of the
 * C_NumInterface struct. It depends on the Python implementation of the
 * interface (see py_number_impl.py).
 */
static C_NumInterface *C_NumberInterface_add(C_NumInterface *a,
                                                C_NumInterface *b)
{
    PyC_NumInterfaceShimObject *py_obj_a, *py_obj_b;
    py_obj_a = (PyC_NumInterfaceShimObject *) a->impl;
    py_obj_b = (PyC_NumInterfaceShimObject *) b->impl;

    PyC_NumInterfaceShimObject *result =
        (PyC_NumInterfaceShimObject *) PyObject_CallMethod(
            (PyObject *) py_obj_a, "add", "O", (PyObject *) py_obj_b);
    if (result == NULL) {
        PyErr_Print();
        return NULL;
    }
    // XXX: The C code that called this function is responsible for this
    // reference
    result->number.impl = result;
    Py_INCREF(result->number.impl);
    return &result->number;
}

/**
 * This is the C function that will actually be pointed to by instances of the
 * C_NumInterface struct. It depends on the Python implementation of the
 * interface (see py_number_impl.py).
 */
static int C_NumberInterface_get_value(C_NumInterface *self)
{
    assert(self->impl);
    PyC_NumInterfaceShimObject *py_obj =
        (PyC_NumInterfaceShimObject *) self->impl;
    // py_obj->value is the underlying C value for the `value` attribute of this
    // instance of the PyNumberImpl class
    assert(py_obj->value ==
           PyLong_AsLong(PyObject_GetAttrString((PyObject *) py_obj, "value")));
    return py_obj->value;
}

/**
 * This is the C function that will actually be pointed to by instances of the
 * C_NumInterface struct. Note that the C code that owns the instance of the
 * C_NumInterface must call the `free` method when it is done,
 * otherwise the Python object will not be garbage collected (because of the
 * cyclic reference).
 */
static void C_NumberInterface_free(C_NumInterface *self)
{
    Py_DECREF(self->impl);
    self->impl = NULL;
}

// ------------------------------------------

static PyC_NumInterfaceShimObject *
PyNumberInterface_new(PyTypeObject *subtype, PyObject *args, PyObject *kwargs)
{
    PyC_NumInterfaceShimObject *self =
        (PyC_NumInterfaceShimObject *) subtype->tp_alloc(subtype, 0);
    self->number.add  = (void *) &C_NumberInterface_add;
    self->number.impl = self;
    // The C_NumInterface struct is responsible for releasing the cyclic
    // reference to the Python object
    Py_INCREF(self->number.impl);
    assert(Py_REFCNT(self) == 2);
    // Populate the C_NumInterface with references to the C functions
    self->number.get_value = (int (*)(void *)) & C_NumberInterface_get_value;
    self->number.free      = (void (*)(void *)) & C_NumberInterface_free;
    self->value            = 0;
    return self;
}

static int PyNumberInterface_init(PyC_NumInterfaceShimObject *self,
                                  PyObject                   *args,
                                  PyObject                   *kwargs)
{
    return 0;
}

static void PyNumberInterface_dealloc(PyC_NumInterfaceShimObject *self)
{
    // This function is only called when the reference count of the Python
    // object reaches zero. Therefore, the C code that owns the
    // C_NumInterface struct is responsible for releasing the cyclic
    // reference to the Python object.

    // If the PyC_NumInterfaceShimObject was instantiated from the Python side,
    // then we have a cyclic reference that needs to be broken. I will fix this
    // memory leak later.
    assert(self->number.impl == NULL);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

/**
 * This will be overridden by the subclass (see py_number_impl.py).
 */
static PyC_NumInterfaceShimObject *
PyNumberInterface_add(PyC_NumInterfaceShimObject *self, PyObject *args)
{
    PyErr_SetString(PyExc_NotImplementedError,
                    "Abstract method not implemented");
    return NULL;
}

static PyMethodDef PyNumberInterface_methods[] = {
    { "add", (PyCFunction) PyNumberInterface_add, METH_VARARGS,
      "Implement in subclass" },
    { NULL }
};

static PyMemberDef PyNumberInterface_members[] = {
    { "value", T_INT, offsetof(PyC_NumInterfaceShimObject, value), 0,
      "The value associated with the instance" },
    { NULL }
};

static PyTypeObject PyC_NumInterfaceShimType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name =
        "py_c_num_interface_shim.PyNumberInterface",
    .tp_basicsize = sizeof(PyC_NumInterfaceShimObject),
    .tp_itemsize  = 0,
    .tp_dealloc   = (destructor) PyNumberInterface_dealloc,
    .tp_flags     = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_doc       = "Python Interface for Number",
    .tp_methods   = PyNumberInterface_methods,
    .tp_members   = PyNumberInterface_members,
    .tp_new       = (newfunc) PyNumberInterface_new,
    .tp_init      = (initproc) PyNumberInterface_init,
    .tp_getattro  = PyObject_GenericGetAttr,
    .tp_setattro  = PyObject_GenericSetAttr,
    .tp_dict      = NULL,
};

// ---------------------------

// Python wrapper for c_interface_consumer.c

static PyObject *py_sum_numbers(PyObject *module, PyObject *args)
{
    PyC_NumInterfaceShimObject *a, *b, *c;
    if (!PyArg_ParseTuple(args, "O!O!O!", &PyC_NumInterfaceShimType, &a,
                          &PyC_NumInterfaceShimType, &b,
                          &PyC_NumInterfaceShimType, &c)) {
        return NULL;
    }
    C_NumInterface *result = c_sum_numbers(&a->number, &b->number, &c->number);
    PyC_NumInterfaceShimObject *result_py_obj =
        (PyC_NumInterfaceShimObject *) result->impl;
    return (PyObject *) result_py_obj;
}

static PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "py_c_num_interface_shim",
    .m_doc  = "Python/C interface shim",
    .m_size = -1,
};

static PyMethodDef module_methods[] = {
    { "sum_numbers", (PyCFunction) py_sum_numbers, METH_VARARGS,
      "Sum the values from three PyNumberImpl objects" },
    { NULL }
};

PyMODINIT_FUNC PyInit_py_c_num_interface_shim()
{
    PyObject *m;
    if (PyType_Ready(&PyC_NumInterfaceShimType) < 0) return NULL;
    m = PyModule_Create(&module);
    if (m == NULL) return NULL;
    Py_INCREF(&PyC_NumInterfaceShimType);
    PyModule_AddObjectRef(m, "PyC_NumInterfaceShim",
                          (PyObject *) &PyC_NumInterfaceShimType);
    PyModule_AddFunctions(m, module_methods);
    return m;
}