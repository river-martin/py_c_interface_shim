# Import the parts of the package that should be exposed to the user
from .py_c_num_interface_shim import ( # type: ignore
    PyC_NumInterfaceShim,
    sum_numbers as _sum_numbers,
)
from .py_number_impl import PyNumberImpl


# Add type hints to the function signature
def sum_numbers(a: PyNumberImpl, b: PyNumberImpl, c: PyNumberImpl) -> PyNumberImpl:
    return _sum_numbers(a, b, c)