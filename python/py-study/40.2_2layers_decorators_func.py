#coding:utf-8
import time


def sayHello(fn):
    def hello(*args):
        print('Hello')
        fn(*args)  # 2

    return hello  # 1


def showTime(fn):
    def getTime(*args):  #4  *args获得fn的参数
        print('args is : ', args)  #查看传入的参数
        print('calling time : ', time.time())
        if len(args) > 0:
            n = args[0]
            n *= 2
            fn(n)   #5  要在修饰符函数中调用传入的函数参数fn，否则function1/function2是不会被调用的，仅仅只输出了时间信息
        else:
            fn()

    return getTime  # 3


@sayHello
@showTime
def function1(a):  # 5
    print('running function1 ')
    print('a = ', a)


function1(3)


'''
如果想要以sayHello -> showTime -> function1的顺序，写法就如上了。
并且是从  function1（3） 开始运行。
1 . function1（3）, then @sayHello, return 嵌入式函数 hello() 代替 function1(), so hello() run firstly
2.  @sayHello.hello()里也有执行 fn(*args), 就是执行 function1（3）， then 引用 @showtime
3.  @showtime 直接返回 gettime()替换 function1()， so gettime() run secondly
4.  @showtime.gettime() 里也有fn(n),就是它的本体了
5.  执行 function1()

========================
C:\D\Exercise\helloworld\python\Qt>40.2_2layers_decorators_func.py
Hello
args is :  (3,)
calling time :  1609126029.3285344
running function1
a =  6


'''