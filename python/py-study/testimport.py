import sys
print("path:", sys.path)

from sys import path, argv
print("path=", path)
print("argv=", argv)

print(type(3))
print(type(3) == int)
print(type(1.1) == float)
print(isinstance(3, int))
print('-----------del a var')

a, b, c = 1, 2, 'abc'
del a
print(b, c)
del b, c
a=3
print(b, c)
