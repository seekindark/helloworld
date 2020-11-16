# generate a fibonacci sequence

a, b = 0, 1
while b < 100:
    print(b, end=', ')
    c = a + b
    a = b
    b = c