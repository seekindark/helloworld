import sys

print('start to test')
try: 
    a = 1/0
except ZeroDivisionError:
    print('1/0 is of ZeroDivisionError, continue!')
    # sys.exit()

while True:
    try:
        x = int(input("please input a number: "))
        break
    except ValueError:          # if the exception is not ValueError, then it's passed to the higher layer
    # except TypeError:
        print("oops! it's not a valid number, please try again")
        raise                   # 自己的异常处理完毕后，继续抛给最高层
    