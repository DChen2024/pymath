from setuptools import setup, Extension
from distutils.command.build_ext import build_ext

module = Extension(
    "pymath.core",
    sources=["pymath/pymathmodule.c"],
)

# assign extra compile args based on compiler
# https://stackoverflow.com/a/5192738
class build_ext_subclass(build_ext):
    def build_extensions(self) -> None:
        if self.compiler.compiler_type == 'msvc':
            for ext in self.extensions:
                ext.extra_compile_args = ['/std:c11', '/W4', '/O2']
        else: # 'unix' or 'mingw32'
            for ext in self.extensions:
                ext.extra_compile_args = ['-std=c99', '-Wall', 'Wextra', '-s', '-O2']
        build_ext.build_extensions(self)

setup(
    name="pymath",
    version="1.0.0",
    description="Basic math functions not part of the Python standard library",
    ext_modules=[module],
    python_requires='>=3.9',
)
