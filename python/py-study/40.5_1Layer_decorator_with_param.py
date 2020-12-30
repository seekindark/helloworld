import time



def showTime(fn):
    def getTime(*args):  #*args获得fn的参数
        print('args is : ', args)  #查看传入的参数
        print('calling time : ', time.time())
        if len(args) > 0:
            fn(args[0])  # 要在修饰符函数中调用传入的函数参数fn，否则function1/function2是不会被调用的，仅仅只输出了时间信息
        else:
            fn()

    return getTime


@showTime
def function1(a):
    print('running function1 ')
    print('a = ', a)


@showTime
def function2():
    print('running function2 ')


function1(3)
function2()


'''
当目标函数需要携带参数时， 对应的修饰函数的嵌入式函数 也需要使用参数 *args。
注意： 但一个修饰函数对应 多个目标函数，而且它们的参数形式不一样时，需要在嵌入式函数里 对输入的参数个数做区分，以做到合理的调用


C:\D\Exercise\helloworld\python\Qt>40.5_1Layer_decorator_with_param.py
args is :  (3,)
calling time :  1609127745.6723034
running function1
a =  3
args is :  ()
calling time :  1609127745.6723034
running function2


'''