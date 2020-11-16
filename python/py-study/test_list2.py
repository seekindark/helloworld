# 列表推导式, 目标还式用来生产 列表 的元素
v = [1, 2, 3]
v = [3*x for x in v]
print(v)
v = [[x, x**x] for x in v]  # [[3, 27], [6, 46656], [9, 387420489]] 推到出list
print(v)

v = [1, 2, 3]
y = [(x, x*2) for x in v]  # 推导出 tuple
print(y)
y = [{x: x*2} for x in v]  # 如果有多个，只有最后的x:x 有效， 前面的都被覆盖了, 推导出 dict
print(y)
v = ['aaa', 'bbb', 'ccc']
y = [len(x) for x in v]
print(y)

v = [1, 2, 3, 4, 5, 6]
v2 = [1, 2, 3]
y = [x**2 for x in v if x % 2 == 0]
print(y)
y = [x*z for x in v for z in v2]  # 2 层循环，后面的在外围，前面的for循环在里层
print(y)
y = [x*z for x in v for z in v2 if x % 2 == 0 and z % 2 == 0 if x > 4]
print(y)


# 嵌套的推导式， 翻转矩阵的例子
def showmatrix(m):
    print('[')
    for x in m:
        print('  ', x, ',')


matrix = [
    [1, 2, 3, 'a'],
    [4, 5, 6, 'b'],
    [7, 8, 9, 'c']
]
y = [[x[i] for x in matrix] for i in range(4)]
print(y)

# 翻转矩阵的例子 改成2层循环
y = []
print(y)
for i in range(4):
    row = []
    for x in matrix:
        row.append(x[i])
    y.append(row)
print(y)

# 翻转矩阵的例子，改成1次循环 +  一次推导式
y = []
for i in range(4):
    row = []
    row = [x[i] for x in matrix]
    y.append(row)
showmatrix(y)
