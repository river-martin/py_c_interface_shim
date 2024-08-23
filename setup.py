from setuptools import Extension, setup

# Note: the MANIFEST.in specifies the header files included in the build

import os

os.environ["CC"] = "clang"

setup(
    ext_modules=[
        Extension(
            name="py_package.py_c_num_interface_shim",
            sources=["src/py_package/py_c_num_interface_shim.c"],
            extra_objects=["c_project/bin/libcproject.so"],
            extra_compile_args=[
                # No optimizations
                #"-O0",
                # Generate GDB debug information
                #"-ggdb",
                # Generate DWARF-4 debug information
                #"-gdwarf-4",
                # Ensure that assertions are enabled
                "-UNDEBUG",
                # Enable AddressSanitizer
                #"-fsanitize=address",
                # Reference the shared library for AddressSanitizer
                #"-shared-libasan",
                # Enable code coverage
                #"-fprofile-instr-generate",
                #"-fcoverage-mapping",
                # Define the DEBUG macro
                "-DDEBUG",
            ],
            extra_link_args=[
                # Link the shared library for AddressSanitizer
                #"-fsanitize=address",
                #"-shared-libasan",
                # Link the shared library for code coverage
                #"-fprofile-instr-generate",
            ],
        )
    ]
)
