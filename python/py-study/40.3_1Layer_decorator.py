#coding=UTF-8
import time

def showTime(fn):
    print('calling time : ', time.time())
    fn() #调用传入的参数

@showTime
def function1():
    print('running function1 ')

@showTime
def function2():
    print('running function2 ')


'''
这样的修饰函数，当碰到@，是可以直接运行的， 其实是直接执行了修饰函数
1. showTime(funciton1)
2. showTime(function2)

这样的写法是不满足要求的，没有起到修饰的作用。
修饰函数的作用是用来 辅助增加LOG，或是查问题时，写一个调式函数作为 目标函数的 修饰函数， 这样所有的目标函数被调用时可以 运行修饰函数，以达到调式的目的


========
C:\D\Exercise\helloworld\python\Qt>40.3_1Layer_decorator.py
calling time :  1609127072.3361893
running function1
calling time :  1609127072.3370233
running function2

'''
