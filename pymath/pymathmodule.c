#define PY_SSIZE_T_CLEAN
#include <Python.h>

/** 
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

static const char* version = "1.0.0";

static PyObject* math_ilog2(PyObject* module, PyObject* n) {
    // Convert the object to a Python int
    n = PyNumber_Index(n);
    if (PyErr_Occurred()) { // TypeError, MemoryError
        return NULL;
    }

    // Check if the Python int is not positive
    if (_PyLong_Sign(n) != 1) {
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

PyDoc_STRVAR(math_ilog2__doc__,
    "Calculate the integer base-2 logarithm of n, rounded down.\n\n"
    "Raises `ValueError` if argument is not positive");

static PyMethodDef math_methods[] = {
    {"ilog2", math_ilog2, METH_O, math_ilog2__doc__},
    {NULL, NULL, 0, NULL} // Sentinel
};

PyDoc_STRVAR(module_doc,
    "This project provides some basic math functions\n"
    "not part of the Python standard library.");

static struct PyModuleDef mathmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "pymath",
    .m_doc = module_doc,
    .m_size = -1,
    .m_methods = math_methods,
};

PyMODINIT_FUNC PyInit_core(void) {
    PyObject* module = PyModule_Create(&mathmodule);
    if (module == NULL) {
        return NULL;
    }
    PyObject_SetAttrString(module, "__version__", PyUnicode_FromString(version));
    return module;
}
