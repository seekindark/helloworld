# {}
d = dict()
# {'a': 1, 'test': 'Monday', 'abc': 3}
d1 = dict(a=1, test='Monday', abc=3)
print(d)
print(d1)

# {'one': 1, 'two': 2, 'three': 3}  可迭代对象方式来构造字典
d3 = dict([('one', 1), ('two', 2), ('three', 3)])
print(d3)

# {'one': 1, 'two': 2, 'three': 3, 4: 4} 映射函数方式来构造字典
d4 = dict(zip(['one', 'two', 'three', 4], [1, 2, 3, 4, 5555]))
print(d4)
