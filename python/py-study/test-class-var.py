class A:
   # x = []       # 定义在这里，表示类变量， 被所有实例化的对象公共使用
   # y = 0
 
    def __init__(self):
        self.x = []
        self.y = 0
        pass
 
    def add(self):
        self.x.append('1')
        self.y+=1

a = A()
a.add()
print("第一次示例化 CLASS A:")
print('a.x, a.y = ', a.x, ', ', a.y)
#print('A.x, A.y = ', A.x, ', ', A.y)
print('id(a, a.x, a.y) = ', id(a), ', ', id(a.x), ', ', id(a.y))    #id() 打印变量所对应的 对象的地址， 其实变量就是 表示对象的引用
#print('id(A, A.x, A.y) = ', id(A), ', ', id(A.x), ', ', id(A.y))    # 

# 开始第2次实例化
b = A()
print("第二次示例化 CLASS A:")
print('b.x, b.y = ', b.x, ', ', b.y)
#print('A.x, A.y = ', A.x, ', ', A.y)
print('id(b, b.x, b.y) = ', id(b), ', ', id(b.x), ', ', id(b.y))    #id() 打印变量所对应的 对象的地址， 其实变量就是 表示对象的引用
#print('id(A, A.x, A.y) = ', id(A), ', ', id(A.x), ', ', id(A.y)) 

b.add()
print("修改 b:")
print('b.x, b.y = ', b.x, ', ', b.y)
#print('A.x, A.y = ', A.x, ', ', A.y)
print('id(a, a.x, a.y) = ', id(a), ', ', id(a.x), ', ', id(a.y))    #id() 打印变量所对应的 对象的地址， 其实变量就是 表示对象的引用
print('id(b, b.x, b.y) = ', id(b), ', ', id(b.x), ', ', id(b.y))    #id() 打印变量所对应的 对象的地址， 其实变量就是 表示对象的引用
#print('id(A, A.x, A.y) = ', id(A), ', ', id(A.x), ', ', id(A.y)) 