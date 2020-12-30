import time
def sayHello(fn):
    print('Hello')

def showTime(fn):
    print('calling time : ', time.time())
    fn(3)

print(' start ...')

@sayHello
@showTime
def function1(a):
    print('running function1 ')
    print('a = ', a)


print(' end ...')      # 这一句是最后执行的，之前的代码在编译阶段就执行了。


'''
双重修饰符:

没有执行函数的情况下， 编译就运行，就能执行:
showTime -> function1 -> SayHello

被@修饰的函数直接运行，就想当于调用了 @的函数，即  sayHello, showtime, 但后引用的先执行


C:\D\Exercise\helloworld\python\Qt>40.1_2Layers_decorator_func.py
 start ...
calling time :  1609125778.3885412
running function1
a =  3
Hello
 end ...


'''