"""
This extension module provides some basic math utility functions
not part of the Python standard library.
"""

# This file exists solely for language servers such as Pylance.

def ilog2(n, /):
    """Calculate integer base-2 logarithm of n, rounded down.
    #### Exceptions
    - Raises `ValueError` if argument is not positive."""
