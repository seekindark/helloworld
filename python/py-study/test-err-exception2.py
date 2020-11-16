#使用 else 子句比把所有的语句都放在 try 子句里面要好，这样可以避免一些意想不到的、而except又没有捕获的异常。
while True:
    try:
        x = int(input("please input a number: "))
        print('1 Good! Right number!')
        # break           
    except ValueError as e:          # if the exception is not ValueError, then it's passed to the higher layer
        print("oops! it's not a valid number, please try again")
        print("oops 2! ", e.with_traceback)
        raise                   # 自己的异常处理完毕后，继续抛给最高层
    else:
        print('2 Good! Right number!')
        break  
    finally:
        print("finally executing ....") # 无论如何最后都要处理的，Finally