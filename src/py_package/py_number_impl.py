from py_package.py_c_num_interface_shim import PyC_NumInterfaceShim  # type: ignore


class PyNumberImpl(PyC_NumInterfaceShim):
    def __init__(self, value: int):
        self.value = value
        super().__init__()

    def add(
        self: PyC_NumInterfaceShim, other: PyC_NumInterfaceShim
    ) -> PyC_NumInterfaceShim:
        return PyNumberImpl(self.value + other.value)
