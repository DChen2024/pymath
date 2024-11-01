import typing

def ilog2(n: typing.SupportsIndex, /) -> int:
    """Calculate integer base-2 logarithm of n, rounded down.
    #### Exceptions
    - Raises `ValueError` if argument is not positive"""
    if not isinstance(n, typing.SupportsIndex):
        raise TypeError(f"'{type(n).__name__}' object cannot be interpreted as an integer")
    if n <= 0:
        raise ValueError("ilog2() argument must be positive")
    # int(n) for types such as np.integer
    return n.__index__().bit_length()-1
