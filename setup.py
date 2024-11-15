from setuptools import setup, Extension

setup(
    name="pymath",
    version="1.0.0",
    ext_modules=[Extension(name="pymath", sources=["pymath/pymath.c"])],
    data_files=[("lib/site-packages",
        ["pymath/pymath.py", "pymath/pymath.pyi"])],
    python_requires=">=3.6",
)
