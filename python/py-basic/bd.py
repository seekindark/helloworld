#!/usr/bin/python3

'''
basic data
'''
counter = 0x100          # 整型变量
miles   = 1000.00       # 浮点型变量
name    = "runoob"     # 字符串
 
print (counter)
print (miles)
print (name)

print('===========')
a=b=c=1
print(a, b, c, sep="\n");

a,b,c=1, 2, "ddddd"
print(a, b, c, sep="\n");

#<class 'int'> <class 'float'> <class 'bool'> <class 'complex'>
a, b, c, d = 20, 5.5, True, 4+3j
print(type(a), type(b), type(c), type(d))
print('===========')
print(isinstance(a, int), \
      isinstance(b, float),\
      isinstance(c, bool),\
      isinstance(d, complex))

"""
class A:
    pass

class B(A):
    pass

isinstance(A(), A)  # returns True
type(A()) == A      # returns True
isinstance(B(), A)    # returns True
type(B()) == A        # returns False
区别就是:
 - type()不会认为子类是一种父类类型。
 - isinstance()会认为子类是一种父类类型。
"""
print('=======del删除的是变量，而不是数据。====')
a=1       # 对象 1 被 变量a引用，对象1的引用计数器为1  
b=a       # 对象1 被变量b引用，对象1的引用计数器加1  
c=a       #1对象1 被变量c引用，对象1的引用计数器加1  
del a     #删除变量a，解除a对1的引用  
del b     #删除变量b，解除b对1的引用  
print(c)  #最终变量c仍然引用1 
print('===========')
if __name__=='__main__': 
    li=[1,2,3,4,5]  #列表本身不包含数据1,2,3,4,5，而是包含变量：li[0] li[1] li[2] li[3] li[4]   
    first=li[0]     #拷贝列表，也不会有数据对象的复制，而是创建新的变量引用  
    del li[0]  
    print(li)      #输出[2, 3, 4, 5]  
    print(first)   #输出 1


    
