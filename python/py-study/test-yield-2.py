#!/usr/bin/python3
import sys


def f_yield(n):
    counter, a = 0, 100
    while True:
        if counter > n:
            break
        yield a
        print('a=', a)
        a -= 1
        counter += 1

# 每次遇到 yield 时函数会暂停并保存当前所有的运行信息，返回 yield 的值, 并在下一次执行 next() 方法时从当前位置继续运行。
f = f_yield(10)                  # 1.a 首次执行，运行到 "yield a" 后就停下了，
                                 #       a 的值被保持到了iteration。 
                                 #      返回值本身就是一个iteration的对象
while True:
    try:
        print(next(f), end=" ")  # 1.b, 这里先执行print， 首次的 a 被打印出来
                                 # 1.c, 然后继续执行 next(f), 就是继续运行 f_yield()， 从 之前停止的地方开始， 即从 print('a=', a) 开始
                                 # 2.a, f_yield() 循环后又回到 “yield a”  这句， 又被停下，a 的值被保持到了 iter的下一个位置
                                 # 2.b ...., 重复1.b, 1.c, 2.a...
    except StopIteration:
        sys.exit()



'''
output:
100 a= 100
99 a= 99
98 a= 98
97 a= 97
96 a= 96
95 a= 95
94 a= 94
93 a= 93
92 a= 92
91 a= 91
90 a= 90
'''