#define PY_SSIZE_T_CLEAN
#include <Python.h>

#if PY_MAJOR_VERSION < 3 || (PY_MAJOR_VERSION == 3 && PY_MINOR_VERSION < 6)
#error "pymath requires at least Python 3.6"
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L
#error "pymath requires at least C99"
#endif

PyDoc_STRVAR(module_doc, "This extension module provides some basic "
    "math utility functions\nnot part of the Python standard library.");

PyDoc_STRVAR(version, "1.0.0");

/** 
 * Use PEP 7 as a guideline for writing C code.
 * https://peps.python.org/pep-0007/
 * 
 * Use the implementation of the Python standard math library as a reference.
 * - https://github.com/python/cpython/blob/main/Modules/mathmodule.c
 * - https://github.com/python/cpython/blob/main/Modules/clinic/mathmodule.c.h
 * 
 * This extension uses functions not part of the public Python/C API.
 * - _PyLong_Sign, _PyLong_NumBits have been available since Python 3.0
 *   - https://github.com/python/cpython/blob/v3.0/Objects/longobject.c
 * 
 * Use the internal functions instead of reimplementing them.
 * - In Python 3.12, the internal representation of integers has changed.
 * - Compare the implementation of _PyLong_NumBits between 3.11 and 3.12.
 *   - https://github.com/python/cpython/blob/v3.11.0/Objects/longobject.c
 *   - https://github.com/python/cpython/blob/v3.12.0/Objects/longobject.c
 */

PyDoc_STRVAR(ilog2_doc, "ilog2($module, n, /)\n--\n\n"
    "Calculate the integer base-2 logarithm of n, rounded down.\n\n"
    "Raises ValueError if argument is not positive.");

static PyObject* ilog2(PyObject* module, PyObject* n) {
    // Convert the object to a Python int
    n = PyNumber_Index(n);
    if (PyErr_Occurred()) { // TypeError, MemoryError
        return NULL;
    }

    // Check if the Python int is not positive
    if (_PyLong_Sign(n) <= 0) {
        PyErr_SetString(PyExc_ValueError,
            "ilog2() argument must be positive");
        Py_DECREF(n);
        return NULL;
    }

    // Get the bit length of the Python int
    size_t bit_length = _PyLong_NumBits(n);
    if (PyErr_Occurred()) { // OverflowError
        Py_DECREF(n);
        return NULL;
    }

    Py_DECREF(n);
    return PyLong_FromSize_t(bit_length-1); // MemoryError
}

static int pymath_exec(PyObject *module) {
    if (PyModule_AddStringConstant(module, "__version__", version) != 0)
        return -1;
    return 0;
}

static PyMethodDef pymath_methods[] = {
    {"ilog2", ilog2, METH_O, ilog2_doc},
    {NULL, NULL, 0, NULL} // Sentinel
};

static PyModuleDef_Slot pymath_slots[] = {
    {Py_mod_exec, pymath_exec},
    {0, NULL} // Sentinel
};

static struct PyModuleDef pymathmodule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "pymath",
    .m_doc = module_doc,
    .m_size = -1,
    .m_methods = pymath_methods,
    // .m_slots = pymath_slots,
};

PyMODINIT_FUNC PyInit_pymath(void) {
    // return PyModuleDef_Init(&pymathmodule);
    PyObject* module = PyModule_Create(&pymathmodule);
    if (module == NULL)
        return NULL;
    if (PyModule_AddStringConstant(module, "__version__", version) != 0)
        return NULL;
    return module;
}
