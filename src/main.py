import py_package

def main():
    # PyNumberImpl is defined in py_package/py_number_impl.py
    a = py_package.PyNumberImpl(1)
    b = py_package.PyNumberImpl(2)
    c = py_package.PyNumberImpl(3)
    # sum_numbers is defined in py_package/py_c_num_interface_shim.c (line 165)
    py_package.sum_numbers(a, b, c)

if __name__ == '__main__':
    main()
