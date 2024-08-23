# Minimal Working Example of a Python/C Interface Shim

## Build

```Bash
# Compile the shared object that contains the definition of the C_NumberInterface and the code for the C consumer of this interface
cd c_project
make libcproject
cd ../
```

**Note**: I used `Ubuntu clang version 18.1.3 (1ubuntu1)` and `GNU Make 4.3`

```Bash
# Install the py_sum_numbers package in a virtual environment
python3 -m venv env
source env/bin/activate
pip install --editable .
```

**Note**: I used Python 3.12.3. The packages installed in my virtual environment after executing the code block above are listed in `package_versions.txt`

## Execute

```Bash
# Activate the virtual environment (not necessary if its already activated)
source env/bin/activate
# Run the python application
python src/main.py
```
