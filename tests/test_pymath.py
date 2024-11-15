from timeit import timeit
import pymath_py
import pymath

n = 1000000

def ilog2_python():
    for i in range(1, n):
        pymath_py.ilog2(i)

def ilog2_c_impl():
    for i in range(1, n):
        pymath.ilog2(i)

print('python vs c_impl')
print(f'python: {timeit(ilog2_python, number=1):.7f}')
print(f'c_impl: {timeit(ilog2_c_impl, number=1):.7f}')
