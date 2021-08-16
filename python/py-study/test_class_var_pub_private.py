class myClass:
    classVar1 = 1       # 公开的类变量，被所有的实例访问
    __classVar2 = 2     # 私有的类变量

    def __init__(self):
        self.x = 3
        self.__y = 4

    def add(self):
        # 注意： 这里的 classvar1 和 __classVar2 是类变量，所以必须使用 myClass 作为前缀来引用，否则失败！！
        myClass.classVar1 += 1
        myClass.__classVar2 + 1
        self.x += 1
        self.__y += 1
    
    def print(self):
        # 注意： 这里的 classvar1 和 __classVar2 是类变量，所以必须使用 myClass 作为前缀来引用，否则失败！！
        print(f'classVar1 = {myClass.classVar1}, __classVar2 = {myClass.__classVar2}, self.x = {self.x}, self.__y = {self.__y}')
    
    def add_var(self):
        self.a = 100
        self.__b = 200    ## 临时增加的 私有变量

    def add_var_plus(self):
        self.a += 1
        self.__b += 1

m = myClass()
m.print()       # classVar1 = 1, __classVar2 = 2, self.x = 3, self.__y = 4
m.add()         # classVar1 = 2, __classVar2 = 2, self.x = 4, self.__y = 5
m.print()

myClass.classVar1 += 1      
m.print()       # classVar1 = 3, __classVar2 = 2, self.x = 4, self.__y = 5


# myClass.__classVar2 + 1     #这是类的私有变量，无法在类的外部访问，执行失败 !!  ttributeError: type object 'myClass' has no attribute '__classVar2'
# m.print() 

m.x += 1
m.print()       # classVar1 = 3, __classVar2 = 2, self.x = 5, self.__y = 5

#m.__y += 1                  # 这是类实例 m 的私有变量，类的外部无法访问。 AttributeError: 'myClass' object has no attribute '__y'
#m.print()

m.z = 1                      #  给实例 m  增加了一个公开变量 z
print('m.z = ', m.z)            # m.z =  1

m.__z = 2                      #  给实例 m  增加了一个公开变量 __z; 注意： 虽然这里的变量之前有 __ 的前缀，但因为是在类的外部临时增加的，依然是公开变量，而不是私有的。
print('m.__z = ', m.__z)        # m.__z = 2 

m.add_var()
print('m.a = ', m.a, '  m.__b = ', m.__b)