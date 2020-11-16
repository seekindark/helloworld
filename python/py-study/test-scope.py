# only function 'def, lambda', class, module 才会引入作用域
# 其它的代码块（如 if/elif/else/、try/except、for/while等）是不会引入新的作用域的，也就是说这些语句内定义的变量，外部也可以访问

def test():
    v1 = 0            # v1 is local in test() only
    return 1


if False:
    message1 = 'virable scope study 1 !!'
else:
    message2 = 'virable scope study 2 !!'

# print(message1) #NameError: name 'message1' is not defined
print(message2)

while True:
    v2 = 2
    break

print('v2 = ', v2)


def test2():
    global v2       # 需要使用 global 关键字声明, 然后再函数体里面可以 访问 了
    v2 += 2
    print('v2 = ', v2)


test2()


def test3():
    v3 = 3


    def test3_internal_f():
        nonlocal v3     # v3 是非本地变量，但也不是全局的变量，所以需要 nonlocal 关键字来声明，然后就可以再小范围内使用了
        v3 += 3
        print('in test3_internal_f(): v3 = ', v3)

    test3_internal_f()
    print('in test3(), v3 = ', v3)   # it's also changed

test3()