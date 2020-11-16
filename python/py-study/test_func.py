def show(a, b, c):
    print('a=', a, "b=", b, "c=", c)
    return 1


x, y, z = 1, 2, 3
show(1, 2, 3)           # 必须参数调用，需要顺序正确
show(a=1, c=3, b=2)     # 关键字参数调用


def showdef(a, b, c=200):  # 有默认值的参数必须放在后面
    print('a=', a, "b=", b, "c=", c)
    return 1


print(showdef(1, 2))


# 不定长参数
def showvars(a, b, *vars):  # *vars  以tuple的形式传入参数
    print('a=', a, "b=", b)
    print('vars num=', len(vars), "：", vars)


showvars(1, 2, 'a', 'b', 'c')


# 不定长参数，带 2个星号 **, 以字典形式传入
def showvars2(a, **argv):
    print ('a=', a)
    print("argv=",argv)


showvars2(1, x=3, name='Tom', age=40)