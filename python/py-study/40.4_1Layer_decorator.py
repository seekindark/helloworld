import time


def showTime(fn):
    def getTime():
        print('calling time : ', time.time())
        fn()  # 要在修饰符函数中调用传入的函数参数fn，否则function1/function2是不会被调用的，仅仅只输出了时间信息

    return getTime


@showTime
def function1():
    print('running function1 ')


@showTime
def function2():
    print('running function2 ')


function1()         # 只有 目标函数被运行时，其对应的修饰函数才能被运行
function2()
'''

这时经典的修饰函数的写法， 需要使用到 嵌入函数 , 比如这里的 getTime()
这样 编译运行时，读到@函数也无法直接执行， 只有真正的目标函数 function1, funciton2被运行时，其对应的修饰函数才被激活调用

注意：
1. 要在修饰符函数中调用传入的函数参数fn，否则function1/function2是不会被调用的，仅仅只输出了时间信息



=============
calling time :  1609127558.6699402
running function1
calling time :  1609127558.6709378
running function2
'''