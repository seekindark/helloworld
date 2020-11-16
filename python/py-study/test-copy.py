'''
There 2 kinds of data-type in python
1) unchangable: number, string and tuple
2) changable: list, set, dict

For the changable variable, there're 3 copy-method
1) direct assignment
2) copy
3) deepcopy
'''
import copy


def showab(a, b):
    print("a = ", a, end='   ')
    print("b = ", b, end='   ')
    print("(a is b : a==b)", (a is b), ":", a==b)


# direct assignment
a = [1, 2, 3]
b = a
showab(a, b)
b.append(4)
showab(a, b)

# copy
a = {1, 2, 3, 4}
b = a
showab(a, b)
a.add(5)
showab(a, b)
c = a.copy()
showab(a,c)
d = copy.copy(b)
showab(b, d)
d.remove(1)
showab(b, d)

# deepcopy
print("test deepcopy:")
a = {1: 'a', 2: 'b', 3: 'c', 4: ['alice', 'bob']}
b = a
showab(a, b)
b[100] = 'xxx'
showab(a, b)
b = copy.copy(a) # copy
showab(a, b)
b[4].append('cat')
showab(a, b)
c = copy.deepcopy(b) #deepcopy, all the depper-layer memory are copied
showab(b, c)
b[4].remove('alice')
showab(b, c)

