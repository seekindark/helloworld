
L = [
    lambda x: x**2,
    lambda x: x**3,
    lambda x: x**4]

for f in L:
    print(f(3)) # call the 3 lamnda expression, i.e. print 2**2, 2**3, 2**4

print('L[0]={0}'.format(L[0]))
print(L[0](2))

sum = lambda x, y : x + y
print('sum = ', sum(1, 2))
f = print
f('sum == ', sum(200,100))